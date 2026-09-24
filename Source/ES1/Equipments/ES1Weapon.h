#pragma once

#include "CoreMinimal.h"
#include "ES1BaseWeapon.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ES1Weapon.generated.h"

UCLASS()
class ES1_API AES1Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Functions
	AES1Weapon();
	virtual void OnConstruction(const FTransform& Transform) override;
	
	USkeletalMeshComponent* GetMesh() const;
	UMaterialInstanceDynamic* GetReticleDynamicMaterialInstance();
	UMaterialInstanceDynamic* GetAmmoCounterDynamicMaterialInstance();
		
	void AttachToOwningPawn(APawn* Pawn) const;
	void DetachFromOwningPawn();
	void WeaponTrace(FHitResult& OutHit, float TraceLength);
	
	void Local_Fire(const FVector& ImpactPoint, const FVector& ImpactNormal, TEnumAsByte<EPhysicalSurface> ImpactSurfaceType);
	void Auth_Fire();
	void Rep_Fire(int32 AuthAmmo);
	
	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|WeaponType")
	FGameplayTag WeaponType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ES1|Camera|Aming")
	float AimFieldOfView;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ES1|Trace")
	float TraceRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|FireType")
	ES1FireType FireType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|FireType")
	float FireTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Damage")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Ammo")
	int32 MagCapacity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Ammo")
	int32 Ammo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Ammo")
	int32 StartingCarriedAmmo;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|Icon")
	TObjectPtr<UMaterialInterface> WeaponIcon;
	
	ES1WeaponStatus WeaponStatus;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void FireEffects(const FVector& ImpactPoint, const FVector& ImpactNormal, EPhysicalSurface ImpactSurfaceType);

private:
	// Functions
	void SetMeshVisibilities(APawn* OwningPawn) const;
	
	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1 | Mesh",  meta=(AllowPrivateAccess=true))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	int32 Sequence;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ES1|UI", meta=(AllowPrivateAccess=true))
	TObjectPtr<UMaterialInterface> ReticleMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ES1|UI", meta=(AllowPrivateAccess=true))
	TObjectPtr<UMaterialInterface> AmmoCounterMaterial;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMatInst_Reticle;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMatInst_AmmoCounter;
};