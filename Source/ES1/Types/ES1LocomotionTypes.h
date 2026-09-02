#pragma once

#include "CoreMinimal.h"
#include "ES1LocomotionTypes.generated.h"

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

