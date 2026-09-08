#pragma once

#include "CoreMinimal.h"
#include "ES1InteractionTypes.generated.h"

UENUM(BlueprintType)
enum class ES1FocusState :uint8
{
	None,
	Nearby,
	Focused
};

USTRUCT(BlueprintType)
struct FES1InteractionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled = false;
};
