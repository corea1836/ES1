#include "UI/ES1ReserveAmmo.h"

#include "Characters/ES1Character.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UES1ReserveAmmo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Image_WeaponIcon->SetRenderOpacity(0.f);
	Text_Ammo->SetRenderOpacity(0.f);
	
	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChaned);
	
	AES1Character* Character = Cast<AES1Character>(GetOwningPlayer()->GetPawn());
	if (!IsValid(Character)) return;
	
	OnPossessedPawnChaned(nullptr, Character);
	
	if (Character->HasWeaponFirstReplicated())
	{
		AES1Weapon* Weapon = IES1PlayerInterface::Execute_GetCurrentWeapon(Character);
		if (IsValid(Weapon))
		{
			OnCurrentReservedAmmoChanged(IES1PlayerInterface::Execute_GetReserveAmmo(Character), Weapon->Ammo, Weapon->WeaponIcon);
		}
	}
	else
	{
		Character->OnWeaponFirstReplicated.AddDynamic(this, &ThisClass::OnWeaponFirstReplicated);
	}
	
	if (Character->HasAuthority())
	{
		AES1Weapon* Weapon = IES1PlayerInterface::Execute_GetCurrentWeapon(Character);
		if (!IsValid(Weapon)) return;
		
		OnCurrentReservedAmmoChanged(IES1PlayerInterface::Execute_GetReserveAmmo(Character), Weapon->Ammo, Weapon->WeaponIcon);
	}
}

void UES1ReserveAmmo::OnPossessedPawnChaned(APawn* OldPawn, APawn* NewPawn)
{
	UES1CombatComponent* OldCombatComponent = UES1CombatComponent::FindCombatComponent(OldPawn);
	if (IsValid(OldCombatComponent))
	{
		OldCombatComponent->OnCurrentReserveeAmmoChanged.RemoveDynamic(this, &ThisClass::OnCurrentReservedAmmoChanged);
		OldCombatComponent->OnRoundFired.RemoveDynamic(this, &ThisClass::OnRoundFired);
	}
	
	UES1CombatComponent* NewCombatComponent = UES1CombatComponent::FindCombatComponent(NewPawn);
	if (IsValid(NewCombatComponent))
	{
		Image_WeaponIcon->SetRenderOpacity(1.f);
		Text_Ammo->SetRenderOpacity(1.f);
		
		NewCombatComponent->OnCurrentReserveeAmmoChanged.AddDynamic(this, &ThisClass::OnCurrentReservedAmmoChanged);
		NewCombatComponent->OnRoundFired.AddDynamic(this, &ThisClass::OnRoundFired);
	}
}

void UES1ReserveAmmo::OnCurrentReservedAmmoChanged(int32 RoundsInReserve, int32 RoundsInWeapon, UMaterialInterface* WeaponIconMaterial)
{
	if (IsValid(WeaponIconMaterial))
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(WeaponIconMaterial);
		if (IsValid(Image_WeaponIcon))
		{
			Image_WeaponIcon->SetBrush(Brush);
		}
	}
	
	if (IsValid(Text_Ammo))
	{
		FText AmmoText = FText::Format(NSLOCTEXT("AmmoText", "AmmoKey", "{0}/{1}"), RoundsInWeapon, RoundsInReserve);
		Text_Ammo->SetText(AmmoText);
	}
}

void UES1ReserveAmmo::OnRoundFired(int32 RoundsCurrent, int32 RoundsMax, int32 RoundsInReserve)
{
	if (IsValid(Text_Ammo))
	{
		FText AmmoText = FText::Format(NSLOCTEXT("AmmoText", "AmmoKey", "{0}/{1}"), RoundsCurrent, RoundsInReserve);
		Text_Ammo->SetText(AmmoText);
	}
}

void UES1ReserveAmmo::OnWeaponFirstReplicated(AES1Weapon* Weapon)
{
	AES1Character* Character = Cast<AES1Character>(GetOwningPlayer()->GetPawn());
	if (!IsValid(Character)) return;
	
	OnCurrentReservedAmmoChanged(IES1PlayerInterface::Execute_GetReserveAmmo(Character), Weapon->Ammo, Weapon->WeaponIcon);
}
