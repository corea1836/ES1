#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ES1ReserveAmmo.generated.h"

class AES1Weapon;
class UTextBlock;
class UImage;

UCLASS()
class ES1_API UES1ReserveAmmo : public UUserWidget
{
	GENERATED_BODY()

public:
	// Functions
	virtual void NativeOnInitialized() override;

	// Variables
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_WeaponIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Ammo;

private:
	// Functions
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void OnCurrentReserveAmmoChanged(int32 RoundsInReserve, int32 RoundsInWeapon, UMaterialInterface* WeaponIconMaterial);

	UFUNCTION()
	void OnRoundFired(int32 RoundsCurrent, int32 RoundsInReserve);

	UFUNCTION()
	void OnWeaponFirstReplicated(AES1Weapon* Weapon);
	
};
