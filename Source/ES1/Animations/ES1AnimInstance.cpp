#include "Animations/ES1AnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Characters/ES1Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UES1AnimInstance::UES1AnimInstance()
{
}

void UES1AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Character = Cast<AES1Character>(GetOwningActor());
	
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UES1AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (Character == nullptr || MovementComponent == nullptr) return;
	
	SelectedEquipment = Character->GetSelectedEquipmentType();
	MovementGate = Character->GetMovementGate();
	
	bUseCrouchRifleUpperBody = bIsCrouching && (SelectedEquipment == EES1EquipmentType::Rifle);
	
	UpdateMovementGate();
	
	Velocity = MovementComponent->Velocity;
	Velocity2D = FVector(Velocity.X, Velocity.Y, 0);
	Acceleration = MovementComponent->GetCurrentAcceleration();
	Acceleration2D = FVector(Acceleration.X, Acceleration.Y, 0);
	bIsAccelerating = Acceleration.Size() > 0.f;
	
	BrakingDecelerationWalking = Character->GetCharacterMovement()->BrakingDecelerationWalking;
	BrakingFrictionFactor = Character->GetCharacterMovement()->BrakingFrictionFactor;
	BrakingFriction = Character->GetCharacterMovement()->BrakingFriction;
	bUseSeparateBrakingFriction = Character->GetCharacterMovement()->bUseSeparateBrakingFriction;
	GroundFriction = Character->GetCharacterMovement()->GroundFriction;
	MaxWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	
	WorldRotation = Character->GetActorRotation();
	
	LastFrameWorldLocation = WorldLocation;
	WorldLocation = Character->GetActorLocation();
	DeltaLocation = (WorldLocation - LastFrameWorldLocation).Length();
	
	VelocityLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(Velocity2D, WorldRotation);
	VelocityLocomotionAngleWithOffset = FRotator::NormalizeAxis(VelocityLocomotionAngle - RootYawOffset);
	LastFrameActorYaw = ActorYaw;
	ActorYaw = WorldRotation.Yaw;
	AccelerationLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(Acceleration2D, WorldRotation);

	AccelerationLocomotionAngleWithOffset = FRotator::NormalizeAxis(AccelerationLocomotionAngle - RootYawOffset);
		
	DeltaActorYaw = FRotator::NormalizeAxis(ActorYaw - LastFrameActorYaw);
	const float YawPerSecond = DeltaActorYaw / DeltaSeconds;	
	
	LeanAngle = FMath::Clamp(YawPerSecond / 2, -90.f, 90.f) * CalculateDirectionFactor();
	
	CalculateLocomotionDirection();
	CalculateAccelerationLocomotionDirection();
	
	UpdateRootYawOffset(DeltaSeconds);
	
	bIsOnAir = MovementComponent->MovementMode == MOVE_Falling;
	if (bIsOnAir)
	{
		bIsJumping = Velocity.Z > 0.f;
		bIsFalling = Velocity.Z < 0.f;
	}
	else
	{
		bIsJumping = false;
		bIsFalling = false;
	}
	
	if (bIsJumping)
	{
		float GravityFactor = MovementComponent->GetGravityZ() * MovementComponent->GravityScale;
		TimeToApex = (-Velocity.Z) / GravityFactor;
	}
	else
	{
		TimeToApex = 0.f;
	}
	
	if (bIsFalling)
	{
		TimeFalling += DeltaSeconds;
	}
	else
	{
		if (bIsJumping) TimeFalling = 0.f;
		
	}
	
	AimPitch = FRotator::NormalizeAxis(TryGetPawnOwner()->GetBaseAimRotation().Pitch);

	FString AccelerationString = FString::Printf(TEXT("Acceleration: %s"), *Acceleration2D.ToString());
	// GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Green, AccelerationString);
	
	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Yellow,
	FString::Printf(TEXT("Offset: %.1f | Angle: %.1f | WithOffset: %.1f"),
		RootYawOffset, VelocityLocomotionAngle, VelocityLocomotionAngleWithOffset));
}

void UES1AnimInstance::CalculateLocomotionDirection()
{
	LastFrameVelocityLocomotionDirection = VelocityLocomotionDirection;
	
	const float Angle = VelocityLocomotionAngleWithOffset;
	
	float ForwardMin = -40.f;
	float ForwardMax = 40.f;
	float BackwardMin = -140.f;
	float BackwardMax = 140.f;

	switch (VelocityLocomotionDirection)
	{
		case EES1LocomotionDirection::Forward:
			ForwardMin -= DeadZone;
			ForwardMax += DeadZone;
			break;
		case EES1LocomotionDirection::Backward:
			BackwardMin += DeadZone;
			BackwardMax -= DeadZone;
			break;
		case EES1LocomotionDirection::Right:
			ForwardMax -= DeadZone;
			BackwardMax += DeadZone;
			break;
		case EES1LocomotionDirection::Left:
			ForwardMin += DeadZone;
			BackwardMin -= DeadZone;
			break;
	}
	
	if (Angle < BackwardMin || Angle > BackwardMax)
	{
		VelocityLocomotionDirection = EES1LocomotionDirection::Backward;
	}
	else if (Angle >= ForwardMin && Angle <= ForwardMax)
	{
		VelocityLocomotionDirection = EES1LocomotionDirection::Forward;
	}
	else if (Angle > ForwardMax && Angle <= BackwardMax)
	{
		VelocityLocomotionDirection = EES1LocomotionDirection::Right;
	}
	else
	{
		VelocityLocomotionDirection = EES1LocomotionDirection::Left;
	}
}

float UES1AnimInstance::CalculateDirectionFactor()
{
	switch (VelocityLocomotionDirection)
	{
		case EES1LocomotionDirection::Forward:
			return 1.f;
		case EES1LocomotionDirection::Backward:
			return -1.f;
		default:
			return 0.f;
	}
}

void UES1AnimInstance::CalculateAccelerationLocomotionDirection()
{
	const float Angle = AccelerationLocomotionAngleWithOffset;
	
	float ForwardMin = -50.f;
	float ForwardMax = 50.f;
	float BackwardMin = -130.f;
	float BackwardMax = 130.f;

	switch (AccelerationLocomotionDirection)
	{
	case EES1LocomotionDirection::Forward:
		ForwardMin -= DeadZone;
		ForwardMax += DeadZone;
		break;
	case EES1LocomotionDirection::Backward:
		BackwardMin += DeadZone;
		BackwardMax -= DeadZone;
		break;
	case EES1LocomotionDirection::Right:
		ForwardMax -= DeadZone;
		BackwardMax += DeadZone;
		break;
	case EES1LocomotionDirection::Left:
		ForwardMin += DeadZone;
		BackwardMin -= DeadZone;
		break;
	}
	
	if (Angle < BackwardMin || Angle > BackwardMax)
	{
		AccelerationLocomotionDirection = EES1LocomotionDirection::Backward;
	}
	else if (Angle >= ForwardMin && Angle <= ForwardMax)
	{
		AccelerationLocomotionDirection = EES1LocomotionDirection::Forward;
	}
	else if (Angle > ForwardMax && Angle <= BackwardMax)
	{
		AccelerationLocomotionDirection = EES1LocomotionDirection::Right;
	}
	else
	{
		AccelerationLocomotionDirection = EES1LocomotionDirection::Left;
	}
}

void UES1AnimInstance::UpdateMovementGate()
{
	LastFrameMovementGate = MovementGate;
	bLastFrameIsCrouching = bIsCrouching;
	MovementGate = Character->GetMovementGate();
	bIsMovementGateChanged = MovementGate != LastFrameMovementGate;
	
	bIsCrouching = MovementGate == EES1MovementGate::Crouching;
	bIsCrouchStateChanged = bLastFrameIsCrouching != bIsCrouching;
}

void UES1AnimInstance::UpdateRootYawOffset(float DeltaSeconds)
{
	if (RootYawOffsetMode == EES1RootYawOffsetMode::Accumulate) 
		RootYawOffset = FRotator::NormalizeAxis(RootYawOffset + (DeltaActorYaw * - 1));
	
	if (RootYawOffsetMode == EES1RootYawOffsetMode::BlendOut)
		RootYawOffset = FMath::FInterpTo(RootYawOffset, 0.f, DeltaSeconds, 1.f);
	
	RootYawOffsetMode = EES1RootYawOffsetMode::BlendOut;
}

void UES1AnimInstance::ProcessTurnCurveYaw()
{
	LastFrameTurnYawCurveValue = TurnYawCurveValue;
	
	const float IsTurning = GetCurveValue("IsTurning");
	
	if (IsTurning < 1)
	{
		TurnYawCurveValue = 0.f;
		LastFrameTurnYawCurveValue = 0.f;
	}
	else
	{
		const float RootRotationZ = GetCurveValue("root_rotation_Z");
		TurnYawCurveValue = RootRotationZ / IsTurning;
		
		if (LastFrameTurnYawCurveValue != 0)
		{
			const float Delta = TurnYawCurveValue - LastFrameTurnYawCurveValue;
			RootYawOffset -= Delta;
			
		}
	}
}

void UES1AnimInstance::ReceiveGroundDistance(float IncomingGroundDistance)
{
	GroundDistance = IncomingGroundDistance;
}
