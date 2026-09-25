#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/ES1ReticleTypes.h"
#include "ES1Reticle.generated.h"

class UImage;
class AES1Weapon;

UCLASS()
class ES1_API UES1Reticle : public UUserWidget
{
	GENERATED_BODY()

public:
	// Functions
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Variables
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Reticle;

private:
	// Functions
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void OnWeaponFirstReplicated(AES1Weapon* Weapon);

	UFUNCTION()
	void OnReticleChanged(UMaterialInstanceDynamic* ReticleDynMatInst, const FES1ReticleParams& ReticleParams);

	UFUNCTION()
	void OnRoundFired(int32 RoundsCurrent, int32 RoundsInReserve);

	UFUNCTION()
	void OnAimingStatusChaged(bool bIsAiming);

	// Variables
	TWeakObjectPtr<UMaterialInstanceDynamic> CurrentReticle_DynMatInst;

	FES1ReticleParams CurrentReticleParams;
	float BaseCornerScaleFactor;
	float BaseShapeCurFactor;
	float _BaseCornerScaleFactor_RoundedFired;
	float _BaseShapeCutFactor_RoundedFired;
	float _BaseCornerScaleFactor_Aiming;
	float _BaseShapeCutFactor_Aiming;
	bool bAiming;
};
