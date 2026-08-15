#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ES1WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FES1PlayerAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimInstance;
};

UCLASS()
class ES1_API UES1WeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="ES1|WeaponData|Weapon")
	TMap<FGameplayTag, FName> EquippedSocketData;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|WeaponData|Weapon")
	TMap<FGameplayTag, FName> UnequippedSocketData;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|WeaponData|Animation")
	TMap<FGameplayTag, FES1PlayerAnimInstance> PlayerAnims;
};
