#include "Components/ES1CombatComponent.h"

#include "Characters/ES1Character.h"
#include "Data/ES1WeaponData.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

UES1CombatComponent::UES1CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceLength = 20'000;
	bAiming = false;
	bFireTriggerPressed = false;
	Local_WeaponIndex = 0;
	TraceLength = 80000.f;
}

void UES1CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
}

void UES1CombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UES1CombatComponent, WeaponInventory);
	DOREPLIFETIME(UES1CombatComponent, CurrentWeapon);
	DOREPLIFETIME_CONDITION(UES1CombatComponent, bAiming, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UES1CombatComponent, bFireTriggerPressed, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UES1CombatComponent, CurrentReserveAmmo, COND_OwnerOnly);
}

void UES1CombatComponent::Initiate_SwitchWeapon()
{
	if (!IsValid(CurrentWeapon)) return;
		
	if (CurrentWeapon->WeaponStatus == ES1WeaponStatus::Cycling) return;
	AdvancedWeaponIndex();
	Local_CycleWeapon(Local_WeaponIndex);
	
}

void UES1CombatComponent::Initiate_FireWeapon_Pressed()
{
	if (!IsValid(CurrentWeapon)) return;

	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
	Server_FireWeaponPressed(HitResult.ImpactPoint);
}

void UES1CombatComponent::Initiate_FireWeapon_Released()
{
	if (!IsValid(CurrentWeapon)) return;

	Local_FireWeaponReleased();
}

void UES1CombatComponent::Initiate_ReloadWeapon()
{
	if (!IsValid(CurrentWeapon)) return;
	
	if (CurrentWeapon->WeaponStatus == ES1WeaponStatus::Cycling || 
		CurrentWeapon->WeaponStatus == ES1WeaponStatus::Reloading) return;
	
	// if (CurrentWeapon->Ammo == CurrentWeapon->MagCapacity) return;
	if (CurrentReserveAmmo == 0) return;
	
	Local_ReloadWeapon();
	Server_ReloadWeapon();
}

void UES1CombatComponent::Initiate_Aim_Pressed()
{
	Local_Aim(true);
	Server_Aim(true);
}

void UES1CombatComponent::Initiate_Aim_Released()
{
	Local_Aim(false);
	Server_Aim(false);
}

void UES1CombatComponent::Notify_CycleWeapon()
{
	if (!IsValid(CurrentWeapon)) return;
	AES1BaseWeapon* NewWeapon = WeaponInventory[Local_WeaponIndex];
	if (IsValid(NewWeapon))
	{
		EquipWeapon(NewWeapon);
	}
}

void UES1CombatComponent::Notify_ReloadWeapon()
{
	if (!IsValid(CurrentWeapon)) return;
	
	// if (GetNetMode() == NM_ListenServer ||
	// 	GetNetMode() == NM_DedicatedServer ||
	// 	GetNetMode() == NM_Standalone)
	// {
	// 	const int32 EmptySpace = CurrentWeapon->MagCapacity - CurrentWeapon->Ammo;
	// 	const int32 AmountToRefill = FMath::Min(EmptySpace, CurrentReserveAmmo);
	// 	CurrentWeapon->Ammo += AmountToRefill;
	// 	ReserveAmmo[CurrentWeapon->WeaponType] = ReserveAmmo[CurrentWeapon->WeaponType] - AmountToRefill;
	// 	CurrentReserveAmmo = ReserveAmmo[CurrentWeapon->WeaponType];
	// 	Client_ReloadWeapon(CurrentWeapon->Ammo, CurrentReserveAmmo);
	// }
	//
	// CurrentWeapon->WeaponStatus = ES1WeaponStatus::Idle;
	// if (bFireTriggerPressed && CurrentWeapon->Ammo > 0)
	// {
	// 	Local_FireWeaponPressed();
	// }
}

void UES1CombatComponent::OnRep_CurrentWeapon(AES1BaseWeapon* LastWeapon)
{
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
	 
	IES1PlayerInterface::Execute_WeaponReplicated(GetOwner());
	InitializeWeaponWidgets();
}

void UES1CombatComponent::OnRep_CurrentReserveAmmo()
{
	// if (IsValid(CurrentWeapon))
	// {
	// 	OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo, CurrentWeapon->Ammo, CurrentWeapon->WeaponIcon);
	// }
}

void UES1CombatComponent::Server_Aim_Implementation(bool bIsPressed)
{
	Local_Aim(bIsPressed);
}

void UES1CombatComponent::Equip(AES1BaseWeapon* Weapon)
{
	CurrentWeapon = Weapon;
	CurrentWeapon->AttachToOwningPawn(Cast<APawn>(GetOwner()));
	//
	// CurrentReserveAmmo = ReserveAmmo.FindChecked(CurrentWeapon->WeaponType);
	// OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo, Weapon->Ammo, CurrentWeapon->WeaponIcon);
}

void UES1CombatComponent::EquipWeapon(AES1BaseWeapon* Weapon)
{
	if (!IsValid(Weapon) || !IsValid(GetOwner())) return;
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		SetCurrentWeapon(Weapon, CurrentWeapon);
	}
	else
	{
		Server_EquipWeapon(Weapon);
	}
}

void UES1CombatComponent::Server_EquipWeapon_Implementation(AES1BaseWeapon* Weapon)
{
	EquipWeapon(Weapon);
}

void UES1CombatComponent::SpawnInventory()
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) return;
	
	for (TSubclassOf<AES1BaseWeapon>& WeaponClass : DefaultWeaponClasses)
	{
		AES1BaseWeapon* Weapon = SpawnWeapon(WeaponClass);
		WeaponInventory.AddUnique(Weapon);
		// ReserveAmmo.Add(Weapon->WeaponType, Weapon->StartingCarriedAmmo);
	}
	
	if (WeaponInventory.Num() > 0)
	{
		Equip(WeaponInventory[0]);
		InitializeWeaponWidgets();
	}
}

void UES1CombatComponent::DestroyInventory()
{
	for (AES1BaseWeapon* Weapon : WeaponInventory)
	{
		if (IsValid(Weapon))
		{
			Weapon->Destroy();
		}
	}
}

void UES1CombatComponent::InitializeWeaponWidgets() const
{
	// if (IsValid(CurrentWeapon))
	// {
	// 	OnReticleChanged.Broadcast(CurrentWeapon->GetReticleDynamicMaterialInstance(), CurrentWeapon->ReticleParams);
	// 	OnAmmoCounterChanged.Broadcast(CurrentWeapon->GetAmmoCounterDynamicMaterialInstance(),
	// 									CurrentWeapon->Ammo,
	// 									CurrentWeapon->MagCapacity);
	// }
}

void UES1CombatComponent::AddAmmo(const FGameplayTag& WeaponType, int32 AmmoAmount)
{
	// if (GetOwner()->HasAuthority() && !IsValid(CurrentWeapon)) return;
	//
	// if (!ReserveAmmo.Contains(WeaponType))
	// {
	// 	ReserveAmmo.Add(WeaponType, AmmoAmount);
	// }
	// else
	// {
	// 	const int32 NewAmmo = ReserveAmmo.FindChecked(WeaponType) + AmmoAmount;
	// 	ReserveAmmo[WeaponType] = NewAmmo;
	// 	
	// 	if (CurrentWeapon->WeaponType.MatchesTagExact(WeaponType))
	// 	{
	// 		CurrentReserveAmmo = NewAmmo;
	// 		
	// 		if (CurrentWeapon->Ammo && NewAmmo > 0)
	// 		{
	// 			Server_ReloadWeapon();
	// 		}
	// 		
	// 		OnAmmoCounterChanged.Broadcast(CurrentWeapon->GetAmmoCounterDynamicMaterialInstance(), CurrentWeapon->Ammo, CurrentWeapon->MagCapacity);
	// 		OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo, CurrentWeapon->Ammo, CurrentWeapon->WeaponIcon); 
	// 	}
	// }
}

void UES1CombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TraceLength;

		GWorld->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECC_Visibility
			);
		
		DrawDebugSphere(
			GetWorld(),
			TraceHitResult.ImpactPoint,
			12.f,
			12.f,
			FColor::Red
			);
	}
}

TSubclassOf<UAnimInstance> UES1CombatComponent::GetCurrentWeaponAnimLayer() const
{
	if (!IsValid(CurrentWeapon)) return nullptr;
	
	if (const FES1PlayerAnimInstance* AnimLayer = WeaponData->PlayerAnims.Find(CurrentWeapon->WeaponType))
		return AnimLayer->AnimInstance;
	
	return nullptr;
}

void UES1CombatComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UES1CombatComponent::BlendOut_CycleWeapon(UAnimMontage* Montage, bool bInterrupted)
{
	UAnimInstance* AnimInstance = IES1PlayerInterface::Execute_GetPlayerMesh(GetOwner())->GetAnimInstance();
	if (IsValid(AnimInstance) && AnimInstance->OnMontageBlendingOut.IsAlreadyBound(this, &ThisClass::BlendOut_CycleWeapon))
	{
		AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ThisClass::BlendOut_CycleWeapon);
	}
	CurrentWeapon->WeaponStatus = ES1WeaponStatus::Idle;
	
	// OnReticleChanged.Broadcast(CurrentWeapon->GetReticleDynamicMaterialInstance(), CurrentWeapon->ReticleParams);
	// OnAmmoCounterChanged.Broadcast(CurrentWeapon->GetAmmoCounterDynamicMaterialInstance(), CurrentWeapon->Ammo, CurrentWeapon->MagCapacity);
	// OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo, CurrentWeapon->Ammo, CurrentWeapon->WeaponIcon);
	
	// if (bFireTriggerPressed && CurrentWeapon->FireType == ES1FireType::Auto && CurrentWeapon->Ammo > 0)
	// {
	// 	Local_FireWeaponPressed();
	// }
}

void UES1CombatComponent::Server_FireWeaponPressed_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwningPawn) || !IsValid(CurrentWeapon) || !IsValid(WeaponData)) return;

	bFireTriggerPressed = true;
	HitTarget = TraceHitTarget;

	Multicast_FireWeaponPressed(HitTarget);
}

void UES1CombatComponent::Multicast_FireWeaponPressed_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	//
	// if (OwningPawn->IsLocallyControlled())
	// {
	// 	CurrentWeapon->Rep_Fire(AuthAmmo);
	// }
	// else
	// {
	bFireTriggerPressed = true;
	HitTarget = TraceHitTarget;
	RemoteFireFXWaitTIme = 0.f;

	PlayFireWeapon(TraceHitTarget);
}

void UES1CombatComponent::PlayFireWeapon(const FVector_NetQuantize& TraceHitTarget)
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwningPawn) || !IsValid(CurrentWeapon) || !IsValid(WeaponData)) return;
	
	USkeletalMeshComponent* PlayerMesh = IES1PlayerInterface::Execute_GetPlayerMesh(GetOwner());
	UAnimInstance* AnimInstance = IsValid(PlayerMesh) ? PlayerMesh->GetAnimInstance() : nullptr;

	if (!IsValid(PlayerMesh) || !IsValid(AnimInstance)) return;

	const float Raised = AnimInstance->GetCurveValue("WeaponRaised");

	if (Raised > 0.8f || RemoteFireFXWaitTIme > 0.3f)
	{
		RemoteFireFXWaitTIme = 0.f;

		UAnimMontage* PlayerFireMontage = WeaponData->WeaponAnims.FindChecked(CurrentWeapon->WeaponType).PlayerFireMontage;
		
		if (IsValid(PlayerFireMontage))
		{
			AnimInstance->Montage_Play(PlayerFireMontage);
			CurrentWeapon->PlayFire(TraceHitTarget);
		}
	}
	else
	{
		RemoteFireFXWaitTIme += GetWorld()->GetDeltaSeconds();
		FVector_NetQuantize Captured = TraceHitTarget;
		GetWorld()->GetTimerManager().SetTimerForNextTick(
			[this, Captured]() { PlayFireWeapon(Captured); });   // 값 캡처
	}
	
}

void UES1CombatComponent::Server_FireWeaponReleased_Implementation()
{
	bFireTriggerPressed = false;
}

AES1BaseWeapon* UES1CombatComponent::SpawnWeapon(TSubclassOf<AES1BaseWeapon> WeaponClass)
{
	AActor* OwningActor = GetOwner();
	if (!IsValid(OwningActor)) return nullptr;
	if (OwningActor->GetLocalRole() < ROLE_Authority) return nullptr;
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Cast<APawn>(OwningActor);
	SpawnInfo.Owner = OwningActor;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	return GetWorld()->SpawnActor<AES1BaseWeapon>(WeaponClass, SpawnInfo);
}

void UES1CombatComponent::FireTimerFinished()
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(CurrentWeapon) || !IsValid(OwningPawn)) return;
	
	// if (CurrentWeapon->Ammo == 0 && CurrentReserveAmmo > 0 && OwningPawn->IsLocallyControlled())
	// {
	// 	Local_ReloadWeapon();
	// 	Server_ReloadWeapon();
	// 	return;
	// }
	
	if (CurrentWeapon->WeaponStatus == ES1WeaponStatus::Firing)
	{
		CurrentWeapon->WeaponStatus = ES1WeaponStatus::Idle;
	}
	
	if (bFireTriggerPressed
		&& CurrentWeapon->FireType == ES1FireType::Auto)
		// && CurrentWeapon->Ammo > 0)
	{
		Local_FireWeaponPressed();
	}
}

void UES1CombatComponent::Local_Aim(bool bIsPressed)
{
	bAiming = bIsPressed;
	OnAimingStatusChanged.Broadcast(bAiming);
	if (AES1Character* Owner = Cast<AES1Character>(GetOwner()))
		Owner->RefreshMovementGate();
}

void UES1CombatComponent::Local_FireWeaponPressed()
{
	if (!IsValid(CurrentWeapon)) return;
	if (!IsValid(WeaponData)) return;
	// bFireTriggerPressed = true;
	
	CurrentWeapon->WeaponStatus = ES1WeaponStatus::Firing;
		
	FHitResult Hit;
	TraceUnderCrosshairs(Hit);

	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ThisClass::FireTimerFinished, CurrentWeapon->FireTime);
	
	Server_FireWeaponPressed(Hit.ImpactPoint);
}

void UES1CombatComponent::Local_FireWeaponReleased()
{
	if (!IsValid(CurrentWeapon)) return;
	bFireTriggerPressed = false;
	Server_FireWeaponReleased();
	
}

int32 UES1CombatComponent::AdvancedWeaponIndex()
{
	int32 Old = Local_WeaponIndex;
	if (WeaponInventory.Num() >= 2)
	{
		Local_WeaponIndex = (Local_WeaponIndex + 1) % WeaponInventory.Num();
	}
	return Local_WeaponIndex;
}

void UES1CombatComponent::Local_CycleWeapon(int32 WeaponIndex)
{
	AES1BaseWeapon* NextWeapon = WeaponInventory[WeaponIndex];
	if (!IsValid(NextWeapon) || !IsValid(WeaponData)) return;
	CurrentWeapon->WeaponStatus = ES1WeaponStatus::Cycling;
	NextWeapon->WeaponStatus = ES1WeaponStatus::Cycling;
	
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	const bool bIsLocal = IsValid(OwningPawn) && OwningPawn->IsLocallyControlled();
	
	const FES1WeaponAnim& WeaponAnim = WeaponData->WeaponAnims.FindChecked(NextWeapon->WeaponType);
	USkeletalMeshComponent* Mesh = IES1PlayerInterface::Execute_GetPlayerMesh(GetOwner());
	if (IsValid(Mesh) && IsValid(WeaponAnim.PlayerEquipMontage))
	{
		 Mesh->GetAnimInstance()->Montage_Play(WeaponAnim.PlayerEquipMontage);
	}
	
	if (bIsLocal)
	{
		Server_CycleWeapon(WeaponIndex);
		Mesh->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &ThisClass::BlendOut_CycleWeapon);
	}
}

void UES1CombatComponent::Server_CycleWeapon_Implementation(int32 WeaponIndex)
{
	Local_WeaponIndex = WeaponIndex;
	Multicast_CycleWeapon(WeaponIndex);
}

void UES1CombatComponent::Multicast_CycleWeapon_Implementation(int32 WeaponIndex)
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwningPawn)) return;
	
	if (!OwningPawn->IsLocallyControlled())
	{
		Local_WeaponIndex = WeaponIndex;
		Local_CycleWeapon(WeaponIndex);
	}
}

void UES1CombatComponent::SetCurrentWeapon(AES1BaseWeapon* NewWeapon, AES1BaseWeapon* LastWeapon)
{
	AES1BaseWeapon* LocalLastWeapon = nullptr;
	
	if (IsValid(LastWeapon))
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}
	
	if (IsValid(LocalLastWeapon))
	{
		LocalLastWeapon->DetachFromOwningPawn();
		LocalLastWeapon->WeaponStatus = ES1WeaponStatus::Unequipped;
	}
	
	CurrentWeapon = NewWeapon;
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwningPawn)) return;
	// if (OwningPawn->HasAuthority() && IsValid(CurrentWeapon))
	// {
	// 	CurrentReserveAmmo = ReserveAmmo.FindChecked(CurrentWeapon->WeaponType);
	// }
	
	if (!IsValid(CurrentWeapon)) return;
	
	CurrentWeapon->AttachToOwningPawn(OwningPawn);
	if (AES1Character* Owner = Cast<AES1Character>(OwningPawn))
		Owner->LinkAnimLayer();
	
	// if (CurrentWeapon->Ammo == 0 && CurrentReserveAmmo > 0 && OwningPawn->IsLocallyControlled())
	// {
	// 	Local_ReloadWeapon();
	// 	Server_ReloadWeapon();
	// }
}

void UES1CombatComponent::Local_ReloadWeapon()
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(CurrentWeapon) || !IsValid(OwningPawn)) return;
	ensure(WeaponData);
	
	UAnimMontage* ReloadMontage = WeaponData->WeaponAnims.FindChecked(CurrentWeapon->WeaponType).PlayerReloadMontage;
	USkeletalMeshComponent* Mesh = IES1PlayerInterface::Execute_GetPlayerMesh(GetOwner());
	if (IsValid(ReloadMontage) && IsValid(Mesh))
	{
		Mesh->GetAnimInstance()->Montage_Play(ReloadMontage);
	}
	
	UAnimationAsset* WeaponReloadAnimation =	WeaponData->WeaponAnims.FindChecked(CurrentWeapon->WeaponType).WeaponReloadAnim; 
	USkeletalMeshComponent* WeaponMesh = CurrentWeapon->GetMesh();
	if (IsValid(WeaponMesh) && IsValid(WeaponReloadAnimation))
	{
		WeaponMesh->PlayAnimation(WeaponReloadAnimation, 0.f);
	}
	
	CurrentWeapon->WeaponStatus = ES1WeaponStatus::Reloading;
}

void UES1CombatComponent::Client_ReloadWeapon_Implementation(int32 NewWeaponAmmo, int32 NewCarriedAmmo)
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(CurrentWeapon) || !IsValid(OwningPawn)) return;
	
	// if (OwningPawn->IsLocallyControlled())
	// {
	// 	CurrentWeapon->Ammo = NewWeaponAmmo;
	// 	CurrentReserveAmmo = NewCarriedAmmo;
	// 	
	// 	OnAmmoCounterChanged.Broadcast(CurrentWeapon->GetAmmoCounterDynamicMaterialInstance(), CurrentWeapon->Ammo, CurrentWeapon->MagCapacity);
	// 	OnCurrentReserveAmmoChanged.Broadcast(CurrentReserveAmmo, CurrentWeapon->Ammo, CurrentWeapon->WeaponIcon);
	// }
}

void UES1CombatComponent::Server_ReloadWeapon_Implementation()
{
	Multicast_ReloadWeapon();
}

void UES1CombatComponent::Multicast_ReloadWeapon_Implementation()
{
	Local_ReloadWeapon();
}
	

