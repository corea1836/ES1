#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/ES1LocomotionTypes.h"
#include "ES1MovementGateData.generated.h"

USTRUCT(BlueprintType)
struct FES1MovementGroup
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAcceleration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingDeceleration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFrictionFactor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFriction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseSeparateBrakingFriction;
	
};

UCLASS()
class ES1_API UES1MovementGateData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Movement Group")
	TMap<EES1MovementGate, FES1MovementGroup> MovementGroupMap;
	
public:
	const FES1MovementGroup *GetMovementGroup(const EES1MovementGate movementEnum) const;
	
};
