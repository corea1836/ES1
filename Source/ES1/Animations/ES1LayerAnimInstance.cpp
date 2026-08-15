#include "Animations/ES1LayerAnimInstance.h"
#include "ES1AnimInstance.h"
#include "AnimCharacterMovementLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UES1LayerAnimInstance::UES1LayerAnimInstance()
{
}

void UES1LayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (USkeletalMeshComponent* Mesh = GetOwningComponent())
	{
		MainAnimInstance = Cast<UES1AnimInstance>(Mesh->GetAnimInstance());
	}
}

void UES1LayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (MainAnimInstance)
	{
		CurrentWeaponType = MainAnimInstance->CurrentWeaponType;
		
		MovementGate = MainAnimInstance->MovementGate;
		VelocityLocomotionAngle = MainAnimInstance->VelocityLocomotionAngle;
		AccelerationLocomotionAngle = MainAnimInstance->AccelerationLocomotionAngle;
		LastFrameVelocityLocomotionDirection = MainAnimInstance->LastFrameVelocityLocomotionDirection;
		VelocityLocomotionDirection = MainAnimInstance->VelocityLocomotionDirection;
		AccelerationLocomotionDirection = MainAnimInstance->AccelerationLocomotionDirection;
		Velocity2D = MainAnimInstance->Velocity2D;
		Acceleration2D = MainAnimInstance->Acceleration2D;
		bIsAccelerating = MainAnimInstance->bIsAccelerating;
		PivotAcceleration2D = MainAnimInstance->PivotAcceleration2D;
		LeanAngle = MainAnimInstance->LeanAngle;
		MaxWalkSpeed = MainAnimInstance->MaxWalkSpeed;
		
		bIsCrouching = MainAnimInstance->bIsCrouching;
		bLastFrameIsCrouching = MainAnimInstance->bLastFrameIsCrouching;
		bIsCrouchStateChanged = MainAnimInstance->bIsCrouchStateChanged;
		
		DeltaLocation = MainAnimInstance->DeltaLocation;
		
		BrakingDecelerationWalking = MainAnimInstance->BrakingDecelerationWalking;
		BrakingFrictionFactor = MainAnimInstance->BrakingFrictionFactor;
		BrakingFriction = MainAnimInstance->BrakingFriction;
		bUseSeparateBrakingFriction = MainAnimInstance->bUseSeparateBrakingFriction;
		GroundFriction = MainAnimInstance->GroundFriction;
		
		RootYawOffset = MainAnimInstance->RootYawOffset;
		
		VelocityLocomotionAngleWithOffset = MainAnimInstance->VelocityLocomotionAngleWithOffset;
		
		GroundDistance = MainAnimInstance->GroundDistance;
		bIsOnAir = MainAnimInstance->bIsOnAir;
		TimeFalling = MainAnimInstance->TimeFalling;
		
		AimPitch = MainAnimInstance->AimPitch;
		
		GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Cyan,
	FString::Printf(TEXT("TimeFalling: %.1f"), TimeFalling));
		
		
		FString VelocityString = FString::Printf(TEXT("Velocity Length: %f"), Velocity2D.Length());
		GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Cyan, VelocityString);
		
		GEngine->AddOnScreenDebugMessage(4, 0.f, FColor::Cyan,
FString::Printf(TEXT("MaxWalkSpeed: %.1f"), MainAnimInstance->Character->GetCharacterMovement()->MaxWalkSpeed));
	}
}

UAnimSequence* UES1LayerAnimInstance::GetDirectionalAnim(const FES1DirectionalAnimations& Anims,
	EES1LocomotionDirection Direction) const
{
	switch (Direction)
	{
		case EES1LocomotionDirection::Forward: return Anims.Forward;
		case EES1LocomotionDirection::Backward: return Anims.Backward;
		case EES1LocomotionDirection::Left: return Anims.Left;
		case EES1LocomotionDirection::Right: return Anims.Right;
		default: return Anims.Forward;
	}
}

UAnimSequence* UES1LayerAnimInstance::GetCycleAnimForMovementGate() const
{
	// switch (MovementGate)
	// {
	// case EES1MovementGate::Walking:
	// 	return GetDirectionalAnim(WalkCycleAnimations, VelocityLocomotionDirection);
	// case EES1MovementGate::Jogging:
	// 	return GetDirectionalAnim(JogCycleAnimations, VelocityLocomotionDirection);
	// case EES1MovementGate::Crouching:
	// 	return GetDirectionalAnim(CrouchCycleAnimations, VelocityLocomotionDirection);
	// default:
	// 	return GetDirectionalAnim(WalkCycleAnimations, VelocityLocomotionDirection);
	// }
	if (MovementGate == EES1MovementGate::Crouching)
	{
		return GetDirectionalAnim(CrouchCycleAnimations, VelocityLocomotionDirection);
	}
	
	const float CurrentSpeed = Velocity2D.Size();
	
	const float WalkToJogThreshold = 450.f;
	
	if (CurrentSpeed >= WalkToJogThreshold)
	{
		return GetDirectionalAnim(JogCycleAnimations, VelocityLocomotionDirection);
	}
	else
	{
		return GetDirectionalAnim(WalkCycleAnimations, VelocityLocomotionDirection);
	}
}

UAnimSequence* UES1LayerAnimInstance::GetStopAnimForMovementGate() const
{
	switch (MovementGate)
	{
	case EES1MovementGate::Walking:
		return GetDirectionalAnim(WalkStopAnimations, VelocityLocomotionDirection);
	case EES1MovementGate::Jogging:
		return GetDirectionalAnim(JogStopAnimations, VelocityLocomotionDirection);
	case EES1MovementGate::Crouching:
		return GetDirectionalAnim(CrouchStopAnimations, VelocityLocomotionDirection);
	default:
		return GetDirectionalAnim(WalkStopAnimations, VelocityLocomotionDirection);
	}
}

UAnimSequence* UES1LayerAnimInstance::GetStartAnimForMovementGate() const
{
	switch (MovementGate)
	{
	case EES1MovementGate::Walking:
		return GetDirectionalAnim(WalkStartAnimations, VelocityLocomotionDirection);
	case EES1MovementGate::Jogging:
		return GetDirectionalAnim(JogStartAnimations, VelocityLocomotionDirection);
	case EES1MovementGate::Crouching:
		return GetDirectionalAnim(CrouchStartAnimations, VelocityLocomotionDirection);
	default:
		return GetDirectionalAnim(WalkStartAnimations, VelocityLocomotionDirection);
	}
}

UAnimSequence* UES1LayerAnimInstance::GetPivotAnimForMovementGate() const
{
	switch (MovementGate)
	{
	case EES1MovementGate::Walking:
		return GetDirectionalAnim(WalkPivotAnimations, AccelerationLocomotionDirection);
	case EES1MovementGate::Jogging:
		return GetDirectionalAnim(JogPivotAnimations, AccelerationLocomotionDirection);
	case EES1MovementGate::Crouching:
		return GetDirectionalAnim(CrouchPivotAnimations, AccelerationLocomotionDirection);
	default:
		return GetDirectionalAnim(WalkPivotAnimations, AccelerationLocomotionDirection);
	}
}

float UES1LayerAnimInstance::GetStopDistance() const
{
	return UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation
	(
		Velocity2D,
		bUseSeparateBrakingFriction,
		BrakingFriction,
		GroundFriction,
		BrakingFrictionFactor,
		BrakingDecelerationWalking
	).Size2D();
	
}
