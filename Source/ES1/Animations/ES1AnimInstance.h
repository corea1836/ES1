#pragma once

#include "CoreMinimal.h"
#include "ES1Define.h"
#include "Animation/AnimInstance.h"
#include "Data/ES1WeaponLocomotionData.h"
#include "ES1AnimInstance.generated.h"

class UES1WeaponLocomotionData;

UCLASS()
class ES1_API UES1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	friend class UES1LayerAnimInstance;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	TObjectPtr<AES1Character> Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Data")
	EES1EquipmentType SelectedEquipment;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	EES1MovementGate MovementGate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	EES1MovementGate LastFrameMovementGate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	bool bIsMovementGateChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bIsCrouching;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bLastFrameIsCrouching;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bIsCrouchStateChanged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Crouch Data")
	bool bUseCrouchRifleUpperBody;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Locomotion", meta=(AllowPrivateAccess=true))
	TObjectPtr<UES1WeaponLocomotionData> EquippedWeaponLocomotionData;
	
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
	bool bIsAccelerating;
	
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
	bool bIsJumping;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	bool bIsOnAir;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	float TimeToApex;
	
	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	float GroundDistance;
	
	UPROPERTY(BlueprintReadWrite, Category = "Jump Data")
	float TimeFalling;
	
	UPROPERTY(BlueprintReadWrite, Category = "Rotation Data")
	float AimPitch;
	
public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void ProcessTurnCurveYaw();
	
public:
	UES1AnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	FORCEINLINE TSubclassOf<UAnimInstance> GetEquippedWeaponLocomotion(const EES1EquipmentType weaponEnum)
	{
		return EquippedWeaponLocomotionData->GetAnimInstanceGroup(weaponEnum)->AnimInstance;
	}
	
	void CalculateLocomotionDirection();
	float CalculateDirectionFactor();
	void CalculateAccelerationLocomotionDirection();
	void UpdateMovementGate();
	void UpdateRootYawOffset(float DeltaSeconds);
	
	void ReceiveGroundDistance(float IncomingGroundDistance);
};
