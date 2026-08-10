#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ES1MovementGateData.h"
#include "ES1AttributeComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ES1_API UES1AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UES1AttributeComponent();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	TObjectPtr<UES1MovementGateData> MovementGateData;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE float GetMaxWalkSpeed(const EES1MovementGate movementTag) const
	{
		return MovementGateData->GetMovementGroup(movementTag)->MaxWalkSpeed;
	}
	
	FORCEINLINE float GetMaxAcceleration(const EES1MovementGate movementTag) const
	{
		return MovementGateData->GetMovementGroup(movementTag)->MaxAcceleration;
	}
	
	FORCEINLINE float GetBrakingDeceleration(const EES1MovementGate movementTag) const
	{
		return MovementGateData->GetMovementGroup(movementTag)->BrakingDeceleration;
	}
	
	FORCEINLINE float GetBrakingFrictionFactor(const EES1MovementGate movementTag) const
	{
		return MovementGateData->GetMovementGroup(movementTag)->BrakingFrictionFactor;
	}
	
	FORCEINLINE float GetBrakingFriction(const EES1MovementGate movementTag) const
	{
		return MovementGateData->GetMovementGroup(movementTag)->BrakingFriction;
	}
	
	FORCEINLINE float GetbUseSeperateBrakingFriction(const EES1MovementGate movementTag) const
	{
		return MovementGateData->GetMovementGroup(movementTag)->bUseSeparateBrakingFriction;
	}
};
