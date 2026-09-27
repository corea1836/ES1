#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "Data/ES1DirectionalAnimations.h"
#include "Tags/ES1WeaponTags.h"
#include "ES1LayerAnimInstance.generated.h"

class UES1AnimInstance;
enum class EES1MovementGate : uint8;
enum class EES1LocomotionDirection : uint8;

UCLASS()
class ES1_API UES1LayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
protected:
	TObjectPtr<UES1AnimInstance> MainAnimInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Anim|Weapon")
	FGameplayTag CurrentWeaponType = ES1WeaponTags::Weapon_Type_Unarmed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	EES1MovementGate MovementGate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector Velocity2D;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool bAccelerating;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector Acceleration2D;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector PivotAcceleration2D;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float BrakingDecelerationWalking;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float BrakingFrictionFactor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float BrakingFriction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool bUseSeparateBrakingFriction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float GroundFriction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	float MaxWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bCrouching;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bLastFrameIsCrouching;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bCrouchStateChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location Data")
	float DeltaLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	float VelocityLocomotionAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	float AccelerationLocomotionAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	EES1LocomotionDirection LastFrameVelocityLocomotionDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	EES1LocomotionDirection VelocityLocomotionDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	EES1LocomotionDirection AccelerationLocomotionDirection;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations WalkCycleAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations JogCycleAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations CrouchCycleAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations WalkStopAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations JogStopAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations CrouchStopAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations SprintAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations WalkStartAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations JogStartAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations CrouchStartAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations WalkPivotAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations JogPivotAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	FES1DirectionalAnimations CrouchPivotAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion Data")
	float LeanAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Root Yaw Data")
	float RootYawOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Root Yaw Data")
	float VelocityLocomotionAngleWithOffset;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	float GroundDistance;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	bool bOnAir;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	float TimeFalling;
	
	UPROPERTY(BlueprintReadWrite, Category = "Rotation Data")
	float AimPitch;
	
	
public:
	UES1LayerAnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UAnimSequence* GetDirectionalAnim(const FES1DirectionalAnimations& Anims, EES1LocomotionDirection Direction) const;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UAnimSequence* GetCycleAnimForMovementGate() const;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UAnimSequence* GetStopAnimForMovementGate() const;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UAnimSequence* GetStartAnimForMovementGate() const;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	UAnimSequence* GetPivotAnimForMovementGate() const;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	float GetStopDistance() const;
	
};
