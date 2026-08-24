#pragma once

#include "CoreMinimal.h"
#include "ES1Define.h"
#include "Blueprint/UserWidget.h"
#include "ES1Reticle.generated.h"

class AES1Weapon;
class UImage;

UCLASS()
class ES1_API UES1Reticle : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void  NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Reticle;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_AmmoCounter;
	
private:
	// Functions
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	UFUNCTION()
	void OnWeaponFirstReplicated(AES1Weapon* Weapon);
	
	UFUNCTION()
	void OnReticleChanged(UMaterialInstanceDynamic* ReticleDynMatInst, const FReticleParams& ReticleParams);
	
	UFUNCTION()
	void OnAmmoCounterChanged(UMaterialInstanceDynamic* AmmoCounterDynMatInst, int32 RoundsCurrent, int32 RoundsMax);
	
	UFUNCTION()
	void OnRoundFired(int32 RoundsCurrent, int32 RoundsMax, int32 RoundsInReserve);
	
	UFUNCTION()
	void OnAimingStatusChanged(bool bIsAiming);
	
	// Variables
	TWeakObjectPtr<UMaterialInstanceDynamic> CurrentReticle_DynMatInst;
	TWeakObjectPtr<UMaterialInstanceDynamic> CurrentAmmoCounter_DynMatInst;
	
	FReticleParams CurrentReticleParams;
	float BaseCornerScaleFactor;
	float BaseShapeCutFactor;
	float _BaseCornerScaleFactor_RoundFired;
	float _BaseShapeCutFactor_RoundFired;
	float _BaseCornerScaleFactor_Aiming;
	float _BaseShapeCutFactor_Aiming;
	bool bAiming;
};
