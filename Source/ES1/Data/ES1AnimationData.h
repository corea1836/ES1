#pragma once

#include "CoreMinimal.h"
#include "ES1Define.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ES1AnimationData.generated.h"

USTRUCT(BlueprintType)
struct FES1MontageGroup
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> AnimMontage;
};

USTRUCT(BlueprintType)
struct FES1MAnimationGroup
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimationAsset> Animation;
};

UCLASS()
class ES1_API UES1AnimationData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Montage Group")
	TMap<FGameplayTag, FES1MontageGroup> MontageGroupMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Animation Group")
	TMap<FGameplayTag, FES1MAnimationGroup> AnimationGroupMap;
public:
	const FES1MontageGroup* GetMontageGroup(const FGameplayTag& GroupTag) const;
	const FES1MAnimationGroup* GetAnimationGroup(const FGameplayTag& GroupTag) const;
};
