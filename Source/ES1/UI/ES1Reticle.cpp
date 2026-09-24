#include "ES1Reticle.h"

#include "Characters/ES1Character.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace Reticle
{
	const FName RoundedCornerScale = FName("RoundedCornerScale");
	const FName ShapeCutThickness = FName("ShapeCutThickness");
}

void UES1Reticle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Image_Reticle->SetRenderOpacity(0.f);
	_BaseCornerScaleFactor_RoundedFired = 0.f;
	_BaseShapeCutFactor_RoundedFired = 0.f;
	_BaseCornerScaleFactor_Aiming = 0.f;
	_BaseShapeCutFactor_Aiming = 0.;
	bAiming = false;

	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);

	AES1Character* Character = Cast<AES1Character>(GetOwningPlayer()->GetPawn());
	if (!IsValid(Character)) return;

	OnPossessedPawnChanged(nullptr, Character);

	if (Character->HasWeaponFirstReplicated())
	{
		AES1BaseWeapon* Weapon = IES1PlayerInterface::Execute_GetCurrentWeapon(Character);
		if (IsValid(Weapon))
		{
			OnReticleChanged(Weapon->GetReticleDynamicMaterialInstance(), Weapon->ReticleParams);
		}
	}
	else
	{
		Character->OnWeaponFirstReplicated.AddDynamic(this, &ThisClass::OnWeaponFirstReplicated);
	}

	if (Character->HasAuthority())
	{
		AES1BaseWeapon* Weapon = IES1PlayerInterface::Execute_GetCurrentWeapon(Character);
		if (IsValid(Weapon))
		{
			OnReticleChanged(Weapon->GetReticleDynamicMaterialInstance(), Weapon->ReticleParams);
		}	
	}
	
}

void UES1Reticle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	_BaseCornerScaleFactor_RoundedFired = FMath::FInterpTo(_BaseCornerScaleFactor_RoundedFired, 0.f, InDeltaTime, CurrentReticleParams.RoundFiredInterpSpeed);
	_BaseShapeCutFactor_RoundedFired = FMath::FInterpTo(_BaseShapeCutFactor_RoundedFired, 0.f, InDeltaTime, CurrentReticleParams.RoundFiredInterpSpeed);

	_BaseCornerScaleFactor_Aiming = FMath::FInterpTo(_BaseCornerScaleFactor_Aiming, bAiming ? CurrentReticleParams.ScaleFactor_Aiming : CurrentReticleParams.ScaleFactor_NotAiming, InDeltaTime, CurrentReticleParams.AimingInterpSpeed);
	_BaseShapeCutFactor_Aiming = FMath::FInterpTo(_BaseShapeCutFactor_Aiming, bAiming ? CurrentReticleParams.ShapeCutFactor_Aiming : CurrentReticleParams.ShapeCutFactor_NotAiming, InDeltaTime, CurrentReticleParams.AimingInterpSpeed);
	BaseCornerScaleFactor = _BaseCornerScaleFactor_RoundedFired + _BaseCornerScaleFactor_Aiming;
	BaseShapeCurFactor = _BaseShapeCutFactor_RoundedFired - _BaseShapeCutFactor_Aiming;

	if (CurrentReticle_DynMatInst.IsValid())
	{
		CurrentReticle_DynMatInst->SetScalarParameterValue(Reticle::RoundedCornerScale, BaseCornerScaleFactor);
		CurrentReticle_DynMatInst->SetScalarParameterValue(Reticle::ShapeCutThickness, BaseShapeCurFactor);
	}
}

void UES1Reticle::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	UES1CombatComponent* OldCombatComponent = UES1CombatComponent::FindCombatComponent(OldPawn);
	if (IsValid(OldCombatComponent))
	{
		OldCombatComponent->OnReticleChanged.RemoveDynamic(this, &ThisClass::OnReticleChanged);
		OldCombatComponent->OnFired.RemoveDynamic(this, &ThisClass::OnFired);
		OldCombatComponent->OnAimingStatusChanged.RemoveDynamic(this, &ThisClass::OnAimingStatusChaged);
	}

	UES1CombatComponent* NewCombatComponent = UES1CombatComponent::FindCombatComponent(NewPawn);
	if (IsValid(NewCombatComponent))
	{
		Image_Reticle->SetRenderOpacity(1.f);
		NewCombatComponent->OnReticleChanged.AddDynamic(this, &ThisClass::OnReticleChanged);
		NewCombatComponent->OnFired.AddDynamic(this, &ThisClass::OnFired);
		NewCombatComponent->OnAimingStatusChanged.AddDynamic(this, &ThisClass::OnAimingStatusChaged);
	}
}

void UES1Reticle::OnWeaponFirstReplicated(AES1BaseWeapon* Weapon)
{
	OnReticleChanged(Weapon->GetReticleDynamicMaterialInstance(), Weapon->ReticleParams);
}

void UES1Reticle::OnReticleChanged(UMaterialInstanceDynamic* ReticleDynMatInst, const FES1ReticleParams& ReticleParams)
{
	CurrentReticleParams = ReticleParams;
	CurrentReticle_DynMatInst = ReticleDynMatInst;

	FSlateBrush Brush;
	Brush.SetResourceObject(ReticleDynMatInst);
	Brush.ImageSize = FVector2D(75.f, 75.f);
	if (IsValid(Image_Reticle))
	{
		Image_Reticle->SetBrush(Brush);
	}
	
	if (CurrentReticle_DynMatInst.IsValid())
	{
		CurrentReticle_DynMatInst->SetVectorParameterValue(FName("Color"), FLinearColor::White);	
	}
}

void UES1Reticle::OnFired()
{
	_BaseCornerScaleFactor_RoundedFired += CurrentReticleParams.ScaleFactor_RoundFired;
	_BaseShapeCutFactor_RoundedFired += CurrentReticleParams.ShapeCutFactor_RoundFired;
}

void UES1Reticle::OnAimingStatusChaged(bool bIsAiming)
{
	bAiming = bIsAiming;
}
