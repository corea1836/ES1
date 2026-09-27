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
	// Functions
	UES1AttributeComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SwitchGate(const EES1MovementGate MovementGate);
	
	void Local_Sprinting(bool bIsSprinting);
	
	void RequestGate(EES1MovementGate NewGate);
	
	UFUNCTION(Server, Reliable)
	void Server_SwitchGate(EES1MovementGate NewGate);
	
	FORCEINLINE EES1MovementGate GetCurrentMovementGate() const { return CurrentMovementGate; }
	FORCEINLINE bool GetbSprinting() const { return bSprinting; }
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement Data")
	TObjectPtr<UES1MovementGateData> MovementGateData;
	
private:
	// Functions
	UFUNCTION()
	void OnRep_MovementGate();
	
	FORCEINLINE float GetMaxWalkSpeed(const EES1MovementGate MovementGate) const
	{
		return MovementGateData->GetMovementGroup(MovementGate)->MaxWalkSpeed;
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
		
	// Variables
	UPROPERTY(Transient, BlueprintReadOnly, ReplicatedUsing=OnRep_MovementGate, meta=(AllowPrivateAccess=true))
	EES1MovementGate CurrentMovementGate;
	
	bool bSprinting;
};
