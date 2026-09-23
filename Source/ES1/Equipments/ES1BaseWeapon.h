#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ES1BaseWeapon.generated.h"

class AES1BulletShell;

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
class ES1_API AES1BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Functions
	AES1BaseWeapon();
	virtual void Tick(float DeltaTime) override;

	void AttachToOwningPawn(APawn* Pawn) const;
	void DetachFromOwningPawn();

	virtual void PlayFire(const FVector& HitTarget);

	virtual void Local_Fire(const FVector& HitTarget);
	virtual void Auth_Fire(const FVector& HitTarget);
	void Rep_Fire(int32 AuthAmmo);

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|WeaponType")
	FGameplayTag WeaponType;
	
	UPROPERTY(VisibleAnywhere, Category="ES1|Status")
	ES1WeaponStatus WeaponStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ES1|Camera|Aming")
	float AimFieldOfView;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|FireType")
	ES1FireType FireType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|FireTime")
	float FireTime;

	UPROPERTY(EditDefaultsOnly, Category="ES1|Trace")
	float TraceLength;

	UPROPERTY(EditDefaultsOnly, Category="ES1|Trace")
	float TraceRadius;

	UPROPERTY(EditAnywhere, Category="ES1|Ammo")
	int32 MagCapacity;

	UPROPERTY(EditAnywhere, Category="ES1|Ammo")
	int32 Ammo;

	UPROPERTY(EditAnywhere, Category="ES1|Ammo")
	int32 StartingCarriedAmmo;
	
protected:
	// Functions
	virtual void BeginPlay() override;

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|BulletShell", meta=(AllowPrivateAccess=true))
	TSubclassOf<AES1BulletShell> BulletShellClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Animation", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimationAsset> FireAnimation;
	
private:
	// Variables
	int32 Sequence;
};
