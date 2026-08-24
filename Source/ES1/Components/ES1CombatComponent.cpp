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
}

void UES1CombatComponent::Initiate_FireWeapon_Pressed()
{
	if (!IsValid(CurrentWeapon)) return;
	bTriggerPressed = true;
	
	if (CurrentWeapon->Ammo > 0)
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

void UES1CombatComponent::OnRep_CurrentWeapon(AES1Weapon* LastWeapon)
{
	if (!IsValid(CurrentWeapon)) return;
	CurrentWeapon->AttachToOwningPawn();
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
	CurrentWeapon->AttachToOwningPawn();
	
	CurrentReserveAmmo = ReserveAmmo.FindChecked(CurrentWeapon->WeaponType);
	OnCurrentReserveeAmmoChanged.Broadcast(CurrentReserveAmmo, Weapon->Ammo, CurrentWeapon->WeaponIcon);
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
		OnAmmoCounterChanged.Broadcast(CurrentWeapon->GetReticleDynamicMaterialInstance(),
										CurrentWeapon->Ammo,
										CurrentWeapon->MagCapacity);
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

void UES1CombatComponent::Server_FireWeapon_Implementation(const FHitResult& Hit)
{
	if (!IsValid(CurrentWeapon)) return;
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
	if (!IsValid(CurrentWeapon)) return;
	
	if (bTriggerPressed && CurrentWeapon->FireType == ES1FireType::Auto && CurrentWeapon->Ammo > 0)
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
	
	UAnimMontage* Montage = WeaponData->WeaponAnims.FindChecked(CurrentWeapon->WeaponType).PlayerFireMontage;
	USkeletalMeshComponent* PlayerMesh = IES1PlayerInterface::Execute_GetPlayerMesh(GetOwner());
	if (IsValid(Montage) && IsValid(PlayerMesh))
	{
		PlayerMesh->GetAnimInstance()->Montage_Play(Montage);
	}
	
	FHitResult Hit;
	CurrentWeapon->WeaponTrace(Hit, TraceLength);
	
	EPhysicalSurface ImpactSurfaceType = Hit.PhysMaterial.IsValid(false) ? Hit.PhysMaterial->SurfaceType.GetValue() : SurfaceType1;
	CurrentWeapon->Local_Fire(Hit.ImpactPoint, Hit.ImpactNormal, ImpactSurfaceType);
	
	OnRoundFired.Broadcast(CurrentWeapon->Ammo, CurrentWeapon->MagCapacity, CurrentReserveAmmo);
	
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ThisClass::FireTimerFinished, CurrentWeapon->FireTime);
	
	Server_FireWeapon(Hit);
}
	

