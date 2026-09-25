#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/ES1Character.h"
#include "Tags/ES1WeaponTags.h"
#include "Types/ES1LocomotionTypes.h"
#include "ES1AnimInstance.generated.h"

class UES1WeaponLocomotionData;

UCLASS()
class ES1_API UES1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	friend class UES1LayerAnimInstance;
	
public:
	// Functions
	UFUNCTION()
	void AnimNotify_CycleWeapon();
	
	UFUNCTION()
	void AnimNotify_ReloadWeapon();

	void SetFireTriggerPressed(bool bPressed) { bFireTriggerPressed = bPressed; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Reference")
	TObjectPtr<AES1Character> Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Reference")
	TObjectPtr<UCharacterMovementComponent> CMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Reference")
	TObjectPtr<UES1CombatComponent> CombatComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Reference")
	TObjectPtr<AES1Weapon> CurrentWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Reference")
	TObjectPtr<UES1AttributeComponent> AttributeComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Reference")
	EES1MovementGate CurrentMovementGate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Combat|Weapon")
	FGameplayTag CurrentWeaponType = ES1WeaponTags::Weapon_Type_Unarmed;

	UPROPERTY(BlueprintReadWrite, Category="ES1|Combat")
	bool bFireTriggerPressed;
	
	UPROPERTY(BlueprintReadWrite, Category="ES1|Combat")
	TMap<FGameplayTag, TObjectPtr<UAnimSequence>> FireBlendAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	EES1MovementGate LastFrameMovementGate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool bMovementGateChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bCrouching;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bLastFrameIsCrouching;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bCrouchStateChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bUseCrouchRifleUpperBody;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector Velocity2D;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector Acceleration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector Acceleration2D;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	FVector PivotAcceleration2D;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool bAccelerating;
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation Data")
	FRotator WorldRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location Data")
	FVector LastFrameWorldLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location Data")
	FVector WorldLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Location Data")
	float DeltaLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	float VelocityLocomotionAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	float AccelerationLocomotionAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	EES1LocomotionDirection VelocityLocomotionDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	EES1LocomotionDirection LastFrameVelocityLocomotionDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	EES1LocomotionDirection AccelerationLocomotionDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	float DeadZone = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	float ActorYaw;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	float LastFrameActorYaw;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	float DeltaActorYaw;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion Data")
	float LeanAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Root Yaw Data")
	float RootYawOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Root Yaw Data")
	EES1RootYawOffsetMode RootYawOffsetMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Root Yaw Data")
	float VelocityLocomotionAngleWithOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turn In Place Data")
	float TurnYawCurveValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turn In Place Data")
	float LastFrameTurnYawCurveValue;
	
	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	float AccelerationLocomotionAngleWithOffset;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	bool bJumping;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	bool bFalling;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	bool bOnAir;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	float TimeToApex;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	float GroundDistance;
	
	UPROPERTY(BlueprintReadWrite, Category = "Jump Data")
	float TimeFalling;
	
	UPROPERTY(BlueprintReadWrite, Category = "Rotation Data")
	float AimPitch;

	UPROPERTY(BlueprintReadWrite, Category = "Rotation Data")
	float SmoothedActorYaw;
public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void ProcessTurnCurveYaw();
	
public:
	UES1AnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	
	void CalculateLocomotionDirection();
	float CalculateDirectionFactor();
	void CalculateAccelerationLocomotionDirection();
	void UpdateMovementGate();
	void UpdateRootYawOffset(float DeltaSeconds);
	
	void ReceiveGroundDistance(float IncomingGroundDistance);
};
