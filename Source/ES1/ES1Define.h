#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EES1EquipmentType : uint8
{
	None,
	UnArmed,
	Pistol,
	Rifle,
};

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
