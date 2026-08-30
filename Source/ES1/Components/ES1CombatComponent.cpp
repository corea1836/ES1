#include "Components/ES1CombatComponent.h"

#include "Characters/ES1Character.h"
#include "Data/ES1WeaponData.h"
#include "Net/UnrealNetwork.h"

UES1CombatComponent::UES1CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceLength = 20'000;
	bAiming = false;
	bTriggerPressed = false;
	Local_WeaponIndex = 0;
}

void UES1CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UES1CombatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UES1CombatComponent, WeaponInventory);
	DOREPLIFETIME(UES1CombatComponent, CurrentWeapon);
	DOREPLIFETIME_CONDITION(UES1CombatComponent, bAiming, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UES1CombatComponent, bFiring, COND_SkipOwner);
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
	bTriggerPressed = true;
	
	if (CurrentWeapon->WeaponStatus == ES1WeaponStatus::Idle && CurrentWeapon->Ammo > 0)
	{
		Local_FireWeapon();
	}
}

void UES1CombatComponent::Initiate_FireWeapon_Released()
{
	bTriggerPressed = false;
}

void UES1CombatComponent::Initiate_ReloadWeapon()
{
	if (!IsValid(CurrentWeapon)) return;
	
	if (CurrentWeapon->WeaponStatus == ES1WeaponStatus::Cycling || 
		CurrentWeapon->WeaponStatus == ES1WeaponStatus::Reloading) return;
	
	if (CurrentWeapon->Ammo == CurrentWeapon->MagCapacity) return;
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
	AES1Weapon* NewWeapon = WeaponInventory[Local_WeaponIndex];
	if (IsValid(NewWeapon))
	{
		EquipWeapon(NewWeapon);
	}
}

void UES1CombatComponent::Notify_ReloadWeapon()
{
	if (!IsValid(CurrentWeapon)) return;
	
	if (GetNetMode() == NM_ListenServer ||
		GetNetMode() == NM_DedicatedServer ||
		GetNetMode() == NM_Standalone)
	{
		const int32 EmptySpace = CurrentWeapon->MagCapacity - CurrentWeapon->Ammo;
		const int32 AmountToRefill = FMath::Min(EmptySpace, CurrentReserveAmmo);
		CurrentWeapon->Ammo += AmountToRefill;
		ReserveAmmo[CurrentWeapon->WeaponType] = ReserveAmmo[CurrentWeapon->WeaponType] - AmountToRefill;
		CurrentReserveAmmo = ReserveAmmo[CurrentWeapon->WeaponType];
		Client_ReloadWeapon(CurrentWeapon->Ammo, CurrentReserveAmmo);
	}
	
	CurrentWeapon->WeaponStatus = ES1WeaponStatus::Idle;
	if (bTriggerPressed && CurrentWeapon->Ammo > 0)
	{
		Local_FireWeapon();
	}
}

void UES1CombatComponent::OnRep_CurrentWeapon(AES1Weapon* LastWeapon)
{
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
	 
	IES1PlayerInterface::Execute_WeaponReplicated(GetOwner());
	InitializeWeaponWidgets();
	
	AES1Character* Owner = Cast<AES1Character>(GetOwner());
	if (!IsValid(Owner)) return;
	Owner->LinkAnimLayer();
}

void UES1CombatComponent::OnRep_CurrentReserveAmmo()
{
	if (IsValid(CurrentWeapon))
	{
		OnCurrentReserveeAmmoChanged.Broadcast(CurrentReserveAmmo, CurrentWeapon->Ammo, CurrentWeapon->WeaponIcon);
	}
}

void UES1CombatComponent::Server_Aim_Implementation(bool bIsPressed)
{
	Local_Aim(bIsPressed);
}

void UES1CombatComponent::Equip(AES1Weapon* Weapon)
{
	CurrentWeapon = Weapon;
	CurrentWeapon->AttachToOwningPawn(Cast<APawn>(GetOwner()));
	
	CurrentReserveAmmo = ReserveAmmo.FindChecked(CurrentWeapon->WeaponType);
	OnCurrentReserveeAmmoChanged.Broadcast(CurrentReserveAmmo, Weapon->Ammo, CurrentWeapon->WeaponIcon);
}

void UES1CombatComponent::EquipWeapon(AES1Weapon* Weapon)
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

void UES1CombatComponent::Server_EquipWeapon_Implementation(AES1Weapon* Weapon)
{
	EquipWeapon(Weapon);
}

void UES1CombatComponent::SpawnInventory()
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) return;
	
	for (TSubclassOf<AES1Weapon>& WeaponClass : DefaultWeaponClasses)
	{
		AES1Weapon* Weapon = SpawnWeapon(WeaponClass);
		WeaponInventory.AddUnique(Weapon);
		ReserveAmmo.Add(Weapon->WeaponType, Weapon->StartingCarriedAmmo);
	}
	
	if (WeaponInventory.Num() > 0)
	{
		Equip(WeaponInventory[0]);
		InitializeWeaponWidgets();
	}
}

void UES1CombatComponent::DestroyInventory()
{
	for (AES1Weapon* Weapon : WeaponInventory)
	{
		if (IsValid(Weapon))
		{
			Weapon->Destroy();
		}
	}
}

void UES1CombatComponent::InitializeWeaponWidgets() const
{
	if (IsValid(CurrentWeapon))
	{
		OnReticleChanged.Broadcast(CurrentWeapon->GetReticleDynamicMaterialInstance(), CurrentWeapon->ReticleParams);
		OnAmmoCounterChanged.Broadcast(CurrentWeapon->GetAmmoCounterDynamicMaterialInstance(),
										CurrentWeapon->Ammo,
										CurrentWeapon->MagCapacity);
	}
}

void UES1CombatComponent::AddAmmo(const FGameplayTag& WeaponType, int32 AmmoAmount)
{
	if (GetOwner()->HasAuthority() && !IsValid(CurrentWeapon)) return;
	
	if (!ReserveAmmo.Contains(WeaponType))
	{
		ReserveAmmo.Add(WeaponType, AmmoAmount);
	}
	else
	{
		const int32 NewAmmo = ReserveAmmo.FindChecked(WeaponType) + AmmoAmount;
		ReserveAmmo[WeaponType] = NewAmmo;
		
		if (CurrentWeapon->WeaponType.MatchesTagExact(WeaponType))
		{
			CurrentReserveAmmo = NewAmmo;
			
			if (CurrentWeapon->Ammo && NewAmmo > 0)
			{
				Server_ReloadWeapon();
			}
			
			OnAmmoCounterChanged.Broadcast(CurrentWeapon->GetAmmoCounterDynamicMaterialInstance(), CurrentWeapon->Ammo, CurrentWeapon->MagCapacity);
			OnCurrentReserveeAmmoChanged.Broadcast(CurrentReserveAmmo, CurrentWeapon->Ammo, CurrentWeapon->WeaponIcon); 
		}
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
	FString P = GetOwner()->HasAuthority() ? TEXT("[SVR]") : TEXT("[CLI]");

	UAnimInstance* AnimInstance = IES1PlayerInterface::Execute_GetPlayerMesh(GetOwner())->GetAnimInstance();
	if (IsValid(AnimInstance) && AnimInstance->OnMontageBlendingOut.IsAlreadyBound(this, &ThisClass::BlendOut_CycleWeapon))
	{
		AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ThisClass::BlendOut_CycleWeapon);
	}
	CurrentWeapon->WeaponStatus = ES1WeaponStatus::Idle;
	
	OnReticleChanged.Broadcast(CurrentWeapon->GetReticleDynamicMaterialInstance(), CurrentWeapon->ReticleParams);
	OnAmmoCounterChanged.Broadcast(CurrentWeapon->GetAmmoCounterDynamicMaterialInstance(), CurrentWeapon->Ammo, CurrentWeapon->MagCapacity);
	OnCurrentReserveeAmmoChanged.Broadcast(CurrentReserveAmmo, CurrentWeapon->Ammo, CurrentWeapon->WeaponIcon);
	
	if (bTriggerPressed && CurrentWeapon->FireType == ES1FireType::Auto && CurrentWeapon->Ammo > 0)
	{
		Local_FireWeapon();
	}
}

void UES1CombatComponent::Server_FireWeapon_Implementation(const FHitResult& Hit)
{
	if (!IsValid(CurrentWeapon)) return;
	if (CurrentWeapon->Ammo <= 0) return;
	
	if (IsValid(Hit.GetActor()) && Hit.GetActor()->Implements<UES1PlayerInterface>())
	{
		IES1PlayerInterface::Execute_DoDamage(Hit.GetActor(), CurrentWeapon->Damage, GetOwner());
	}
	
	if (GetNetMode() != NM_ListenServer || !Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		CurrentWeapon->Auth_Fire();
	}
	Multicast_FireWeapon(Hit, CurrentWeapon->Ammo);
}

void UES1CombatComponent::Multicast_FireWeapon_Implementation(const FHitResult& Hit, int32 AuthAmmo)
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwningPawn)) return;
	
	if (OwningPawn->IsLocallyControlled())
	{
		CurrentWeapon->Rep_Fire(AuthAmmo);
	}
	else
	{
		if (!IsValid(WeaponData)) return;
		
		EPhysicalSurface ImpactSurfaceType = Hit.PhysMaterial.IsValid(false) ? Hit.PhysMaterial->SurfaceType.GetValue() : SurfaceType1;
		
		UAnimationAsset* WeaponFireAnimation =	WeaponData->WeaponAnims.FindChecked(CurrentWeapon->WeaponType).WeaponFireAnim; 
		USkeletalMeshComponent* WeaponMesh = CurrentWeapon->GetMesh();
		if (IsValid(WeaponMesh) && IsValid(WeaponFireAnimation))
		{
			WeaponMesh->PlayAnimation(WeaponFireAnimation, 0.f);
		}

		CurrentWeapon->Local_Fire(Hit.ImpactPoint, Hit.ImpactNormal, ImpactSurfaceType);
	
		UAnimMontage* Montage = WeaponData->WeaponAnims.FindChecked(CurrentWeapon->WeaponType).PlayerFireMontage;
		USkeletalMeshComponent* PlayerMesh = IES1PlayerInterface::Execute_GetPlayerMesh(GetOwner());
		if (IsValid(Montage) && IsValid(PlayerMesh))
		{
			PlayerMesh->GetAnimInstance()->Montage_Play(Montage);
		}
		
		
	}
}

AES1Weapon* UES1CombatComponent::SpawnWeapon(TSubclassOf<AES1Weapon> WeaponClass)
{
	AActor* OwningActor = GetOwner();
	if (!IsValid(OwningActor)) return nullptr;
	if (OwningActor->GetLocalRole() < ROLE_Authority) return nullptr;
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = Cast<APawn>(OwningActor);
	SpawnInfo.Owner = OwningActor;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	return GetWorld()->SpawnActor<AES1Weapon>(WeaponClass, SpawnInfo);
}

void UES1CombatComponent::FireTimerFinished()
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(CurrentWeapon) || !IsValid(OwningPawn)) return;
	
	if (CurrentWeapon->Ammo == 0 && CurrentReserveAmmo > 0 && OwningPawn->IsLocallyControlled())
	{
		Local_ReloadWeapon();
		Server_ReloadWeapon();
		return;
	}
	
	if (CurrentWeapon->WeaponStatus == ES1WeaponStatus::Firing)
	{
		CurrentWeapon->WeaponStatus = ES1WeaponStatus::Idle;
	}
	
	if (bTriggerPressed && 
		CurrentWeapon->FireType == ES1FireType::Auto && 
		CurrentWeapon->Ammo > 0)
		
	{
		Local_FireWeapon();
	}
}

void UES1CombatComponent::Local_Aim(bool bIsPressed)
{
	bAiming = bIsPressed;
	OnAimingStatusChanged.Broadcast(bAiming);
	if (AES1Character* Owner = Cast<AES1Character>(GetOwner()))
		Owner->RefreshMovementGate();
}

void UES1CombatComponent::Local_FireWeapon()
{
	if (!IsValid(CurrentWeapon)) return;
	if (!IsValid(WeaponData)) return;
	
	CurrentWeapon->WeaponStatus = ES1WeaponStatus::Firing;
	
	UAnimMontage* Montage = WeaponData->WeaponAnims.FindChecked(CurrentWeapon->WeaponType).PlayerFireMontage;
	USkeletalMeshComponent* PlayerMesh = IES1PlayerInterface::Execute_GetPlayerMesh(GetOwner());
	if (IsValid(Montage) && IsValid(PlayerMesh))
	{
		PlayerMesh->GetAnimInstance()->Montage_Play(Montage);
	}
	
	FHitResult Hit;
	CurrentWeapon->WeaponTrace(Hit, TraceLength);
	
	EPhysicalSurface ImpactSurfaceType = Hit.PhysMaterial.IsValid(false) ? Hit.PhysMaterial->SurfaceType.GetValue() : SurfaceType1;
	UAnimationAsset* WeaponFireAnimation =	WeaponData->WeaponAnims.FindChecked(CurrentWeapon->WeaponType).WeaponFireAnim; 
	USkeletalMeshComponent* WeaponMesh = CurrentWeapon->GetMesh();
	
	if (IsValid(WeaponMesh) && IsValid(WeaponFireAnimation))
	{
		WeaponMesh->PlayAnimation(WeaponFireAnimation, 0.f);
	}
	CurrentWeapon->Local_Fire(Hit.ImpactPoint, Hit.ImpactNormal, ImpactSurfaceType);
	
	OnRoundFired.Broadcast(CurrentWeapon->Ammo, CurrentWeapon->MagCapacity, CurrentReserveAmmo);
	
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ThisClass::FireTimerFinished, CurrentWeapon->FireTime);
	
	Server_FireWeapon(Hit);
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
	AES1Weapon* NextWeapon = WeaponInventory[WeaponIndex];
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

void UES1CombatComponent::SetCurrentWeapon(AES1Weapon* NewWeapon, AES1Weapon* LastWeapon)
{
	AES1Weapon* LocalLastWeapon = nullptr;
	
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
	if (OwningPawn->HasAuthority() && IsValid(CurrentWeapon))
	{
		CurrentReserveAmmo = ReserveAmmo.FindChecked(CurrentWeapon->WeaponType);
	}
	
	if (!IsValid(CurrentWeapon)) return;
	
	CurrentWeapon->AttachToOwningPawn(OwningPawn);
	
	if (CurrentWeapon->Ammo == 0 && CurrentReserveAmmo > 0 && OwningPawn->IsLocallyControlled())
	{
		Local_ReloadWeapon();
		Server_ReloadWeapon();
	}
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
	
	if (OwningPawn->IsLocallyControlled())
	{
		CurrentWeapon->Ammo = NewWeaponAmmo;
		CurrentReserveAmmo = NewCarriedAmmo;
		
		OnAmmoCounterChanged.Broadcast(CurrentWeapon->GetAmmoCounterDynamicMaterialInstance(), CurrentWeapon->Ammo, CurrentWeapon->MagCapacity);
		OnCurrentReserveeAmmoChanged.Broadcast(CurrentReserveAmmo, CurrentWeapon->Ammo, CurrentWeapon->WeaponIcon);
	}
}

void UES1CombatComponent::Server_ReloadWeapon_Implementation()
{
	Multicast_ReloadWeapon();
}

void UES1CombatComponent::Multicast_ReloadWeapon_Implementation()
{
	Local_ReloadWeapon();
}
	

