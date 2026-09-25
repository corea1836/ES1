#pragma once

#include "CoreMinimal.h"
#include "ES1CoreTypes.h"

namespace ES1TraceChannel
{
	constexpr ECollisionChannel ECC_Weapon = ECC_GameTraceChannel1;
	constexpr ECollisionChannel ECC_Interact = ECC_GameTraceChannel2;
	constexpr ECollisionChannel ECC_SkeletalMesh = ECC_GameTraceChannel3;
}
