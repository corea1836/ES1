#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ES1HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHealthChanged,
	UES1HealthComponent*, HealthComponent,
	float, OldValue,
	float, NewValue,
	AActor*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathStarted);


UENUM(BlueprintType)
enum class EDeathState : uint8
{
	NotDead,
	DeathStarted,
	DeathFinished
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ES1_API UES1HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Functions
	UES1HealthComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintPure, Category="ES1|Health")
	static UES1HealthComponent* FindHealthComponent(const AActor* Actor)
	{
		return IsValid(Actor) ? Actor->FindComponentByClass<UES1HealthComponent>() : nullptr;
	}
	
	UFUNCTION(BlueprintCallable)
	float GetHealthNormalize() const;
	
	bool ChangeHealthByAmount(float Amount, AActor* Instigator);
	void ChangeMaxHealthByAmount(float Amount, AActor* Instigator);
	
	// Variables
	UPROPERTY(ReplicatedUsing=OnRep_DeathState)
	EDeathState DeathState;
	
	UPROPERTY(ReplicatedUsing=OnRep_Health, EditDefaultsOnly, Category="ES1|Health")
	float Health;
	
	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, EditDefaultsOnly, Category="ES1|Health")
	float MaxHealth;
	
	UPROPERTY(BlueprintAssignable)
	FHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FHealthChanged OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FDeathStarted OnDeathStarted;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_DeathState(EDeathState OldDeathState);
	
	UFUNCTION()
	void OnRep_Health(float OldValue);
	
	UFUNCTION()
	void OnRep_MaxHealth(float OldValue);
	
private:
	// Functions
	void StartDeath();
};
