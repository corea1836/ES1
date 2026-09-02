#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "UI/ES1Reticle.h"
#include "ES1Weapon.generated.h"

UENUM(BlueprintType)
enum class ES1FireType : uint8
{
	Auto UMETA(DisplayName = "Automatic"),
	SemiAuto UMETA(DisplayName = "SemiAutomatic"),
};

UENUM(BlueprintType)
enum class ES1WeaponStatus : uint8
{
	Idle, // Weapon doing nothing, can fire/reload/cycle
	Firing, // Currently firing, can't reload/cycle
	Reloading, // Currently reloading, can't fire/cycle
	Cycling, // Currently cycling to the next weapon, can't fire/reload/ cycle
	Unequipped // On our person, but can't do anything
};

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

	UPROPERTY(EditDefaultsOnly, Category="ES1|UI|Reticle")
	FES1ReticleParams ReticleParams;
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1 | Mesh",  meta=(AllowPrivateAccess=true))
	TObjectPtr<USkeletalMeshComponent> Mesh;
		
	// Variables
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