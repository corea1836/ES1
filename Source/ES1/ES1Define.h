#pragma once

#include "CoreMinimal.h"
#include "ES1Define.generated.h"

namespace ES1TraceChannel
{
	constexpr ECollisionChannel ECC_Weapon = ECC_GameTraceChannel1;
}

UENUM(BlueprintType)
enum class EES1MovementGate : uint8
{
	Walking,
	Jogging,
	Crouching,
};

UENUM(BlueprintType)
enum class EES1LocomotionDirection : uint8
{
	Forward,
	Backward,
	Right,
	Left,
};

UENUM(BlueprintType)
enum class EES1RootYawOffsetMode : uint8
{
	Accumulate,
	BlendOut,
	Hold,
};

USTRUCT(BlueprintType)
struct FReticleParams
{
	GENERATED_BODY()
	
	// Shape Cut Factor 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ShapeCutFactor_RoundFired = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ShapeCutFactor_Aiming = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ShapeCutFactor_NotAiming = 0.f;
	
	// Scale Factor
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ScaleFactor_RoundFired = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ScaleFactor_Aiming = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ScaleFactor_NotAiming = 0.f;
	
	// Interp Speed 
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RoundFiredInterpSpeed = 20.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AimingInterpSpeed = 15.f;
};


