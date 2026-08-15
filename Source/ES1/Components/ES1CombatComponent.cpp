#include "Components/ES1CombatComponent.h"

#include "ES1GameplayTags.h"
#include "Characters/ES1Character.h"
#include "Data/ES1WeaponData.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UES1CombatComponent::UES1CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
}

void UES1CombatComponent::Initiate_SwitchWeapon()
{
}

void UES1CombatComponent::Initiate_FireWeapon_Pressed()
{
}

void UES1CombatComponent::Initiate_FireWeapon_Released()
{
}

void UES1CombatComponent::Initiate_ReloadWeapon()
{
}

void UES1CombatComponent::Initiate_Aim_Pressed()
{
}

void UES1CombatComponent::Initiate_Aim_Released()
{
}

void UES1CombatComponent::OnRep_CurrentWeapon(AES1Weapon* LastWeapon)
{
	if (!IsValid(CurrentWeapon)) return;
	CurrentWeapon->AttachToOwningPawn();
	
	AES1Character* Owner = Cast<AES1Character>(GetOwner());
	if (!IsValid(Owner)) return;
	Owner->LinkAnimLayer();
}

void UES1CombatComponent::Equip(AES1Weapon* Weapon)
{
	CurrentWeapon = Weapon;
	CurrentWeapon->AttachToOwningPawn();
}

void UES1CombatComponent::SpawnInventory()
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority) return;
	
	for (TSubclassOf<AES1Weapon>& WeaponClass : DefaultWeaponClasses)
	{
		AES1Weapon* Weapon = SpawnWeapon(WeaponClass);
		WeaponInventory.AddUnique(Weapon);
	}
	
	if (WeaponInventory.Num() > 0)
	{
		Equip(WeaponInventory[0]);
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


UAnimMontage* UES1CombatComponent::GetSelectedEquipmentMontage(const FGameplayTag& GroupTag) const
{
	// if (SelectedEquipment)
	// {
	// 	return SelectedEquipment->GetMontage(GroupTag);
	// }
	
	return nullptr;
}

UAnimationAsset* UES1CombatComponent::GetSelectedEquipmentAnimation(const FGameplayTag& GroupTag) const
{
	// if (SelectedEquipment)
	// {
	// 	return SelectedEquipment->GetAnimation(GroupTag);
	// }
	
	return nullptr;
}

bool UES1CombatComponent::UseSelectedEquipment()
{
	// if (SelectedEquipment)
	// {
	// 	if (SelectedEquipment->bCanUse)
	// 	{
	// 		SelectedEquipment->Use();
	// 		return true;
	// 	}
	// }
	
	return false;
}
	

