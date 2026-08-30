#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ES1ReserveAmmo.generated.h"

class AES1Weapon;
class UImage;
class UTextBlock;

UCLASS()
class ES1_API UES1ReserveAmmo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_WeaponIcon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Ammo;
	
private:
	UFUNCTION()
	void OnPossessedPawnChaned(APawn* OldPawn, APawn* NewPawn);
	
	UFUNCTION()
	void OnCurrentReservedAmmoChanged(int32 RoundsInReserve, int32 RoundsInWeapon, UMaterialInterface* WeaponIconMaterial);

	UFUNCTION()
	void OnRoundFired(int32 RoundsCurrent, int32 RoundsMax, int32 RoundsInReserve);
	
	UFUNCTION()
	void OnWeaponFirstReplicated(AES1Weapon* Weapon);
};
