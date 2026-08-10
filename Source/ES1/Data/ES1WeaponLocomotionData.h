#pragma once

#include "CoreMinimal.h"
#include "Characters/ES1Character.h"
#include "Engine/DataAsset.h"
#include "ES1WeaponLocomotionData.generated.h"

USTRUCT(BlueprintType)
struct FES1WeaponAnimInstanceGroup
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimInstance;
	
};

UCLASS()
class ES1_API UES1WeaponLocomotionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Weapon Group")
	TMap<EES1EquipmentType, FES1WeaponAnimInstanceGroup> AnimInstanceGroupMap;
	
public:
	const FES1WeaponAnimInstanceGroup* GetAnimInstanceGroup(const EES1EquipmentType WeaponEnum);
	
};
