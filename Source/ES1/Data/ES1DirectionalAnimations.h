#pragma once

#include "CoreMinimal.h"
#include "ES1DirectionalAnimations.generated.h"

USTRUCT(BlueprintType)
struct FES1DirectionalAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	TObjectPtr<UAnimSequence> Forward = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	TObjectPtr<UAnimSequence> Backward = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	TObjectPtr<UAnimSequence> Right = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	TObjectPtr<UAnimSequence> Left = nullptr;
};