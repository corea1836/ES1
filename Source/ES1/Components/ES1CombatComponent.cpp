#include "Components/ES1CombatComponent.h"

#include "ES1GameplayTags.h"
#include "Equipments/ES1Weapon.h"
#include "GameFramework/Character.h"

UES1CombatComponent::UES1CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UES1CombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryWeapon = Spawn(PrimaryWeaponClass);
	SecondaryWeapon = Spawn(SecondaryWeaponClass);
	SideWeapon = Spawn(SideWeaponClass);
	
}

void UES1CombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AES1Equipment* UES1CombatComponent::GetSelectedEquipment() const
{
	switch (SelectedSlot)
	{
	case EES1SelectedWeaponSlot::PrimaryWeapon:
		return PrimaryWeapon;
	case EES1SelectedWeaponSlot::SecondaryWeapon:
		return SecondaryWeapon;
	case EES1SelectedWeaponSlot::SideWeapon:
		return SideWeapon;
	default:
		return nullptr;
	}
}

void UES1CombatComponent::SwitchEquipment(EES1SelectedWeaponSlot IncomingSlot)
{
	AES1Equipment* Equipment = GetSelectedEquipment();
	if (Equipment != nullptr)
	{
		Equipment->UnequipItem();
	}
	
	SelectedSlot = IncomingSlot;
	AES1Equipment* IncomingEquipment = GetSelectedEquipment();
	if (IncomingEquipment != nullptr)
	{
		IncomingEquipment->EquipItem();
		SelectedEquipment = IncomingEquipment;
	}
}

UAnimMontage* UES1CombatComponent::GetSelectedEquipmentMontage(const FGameplayTag& GroupTag) const
{
	if (SelectedEquipment)
	{
		return SelectedEquipment->GetMontage(GroupTag);
	}
	
	return nullptr;
}

UAnimationAsset* UES1CombatComponent::GetSelectedEquipmentAnimation(const FGameplayTag& GroupTag) const
{
	if (SelectedEquipment)
	{
		return SelectedEquipment->GetAnimation(GroupTag);
	}
	
	return nullptr;
}

bool UES1CombatComponent::UseSelectedEquipment()
{
	if (SelectedEquipment)
	{
		if (SelectedEquipment->bCanUse)
		{
			SelectedEquipment->Use();
			return true;
		}
	}
	
	return false;
}

AES1Weapon* UES1CombatComponent::Spawn(TSubclassOf<AES1Weapon> WeaponClass)
{
	if (!WeaponClass) return nullptr;
    
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	SpawnParams.Owner = GetOwner();
    
	AES1Weapon* SpawnedWeapon = GetWorld()->SpawnActor<AES1Weapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
    
	if (SpawnedWeapon)
	{
		FName SpawnedSocket = SpawnedWeapon->GetUnequipSocketName();
       
		if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
		{
			SpawnedWeapon->AttachToOwner(SpawnedSocket);
		}
	}
    
	return SpawnedWeapon;
}
	
	

