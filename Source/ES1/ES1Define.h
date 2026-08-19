#pragma once

#include "CoreMinimal.h"

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

UENUM(BlueprintType)
enum class EES1SelectedWeaponSlot : uint8
{
	None,
	PrimaryWeapon,
	SecondaryWeapon,
	SideWeapon,
	MeleeWeapon,
	Unarmed,
};

UENUM(BlueprintType)
enum class EES1MontageGroup : uint8
{
	Fire,
};
