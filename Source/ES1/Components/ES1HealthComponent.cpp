#include "ES1HealthComponent.h"

#include "Net/UnrealNetwork.h"

UES1HealthComponent::UES1HealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = true;
	DeathState = EDeathState::NotDead;
	SetIsReplicatedByDefault(true);
	Health = 100.f;
	MaxHealth = 100.f;
}

void UES1HealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UES1HealthComponent, DeathState);
	DOREPLIFETIME_CONDITION(UES1HealthComponent, Health, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UES1HealthComponent, MaxHealth, COND_OwnerOnly);
}

float UES1HealthComponent::GetHealthNormalize() const
{
	return (MaxHealth > 0.f) ? (Health / MaxHealth) : 0.f;
}

bool UES1HealthComponent::ChangeHealthByAmount(float Amount, AActor* Instigator)
{
	float OldValue = Health;
	Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(this, OldValue, Health, Instigator);
	
	if (Health <= 0.f)
	{
		StartDeath();
	}
	
	return false;
}

void UES1HealthComponent::ChangeMaxHealthByAmount(float Amount, AActor* Instigator)
{
	float OldValue = MaxHealth;
	MaxHealth += Amount;
	OnMaxHealthChanged.Broadcast(this, OldValue, MaxHealth, Instigator);
}

void UES1HealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UES1HealthComponent::OnRep_DeathState(EDeathState OldDeathState)
{
	if (DeathState == EDeathState::DeathStarted)
	{
		OnDeathStarted.Broadcast();
	}
}

void UES1HealthComponent::OnRep_Health(float OldValue)
{
	OnHealthChanged.Broadcast(this, OldValue, Health, nullptr);
}

void UES1HealthComponent::OnRep_MaxHealth(float OldValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, MaxHealth, nullptr);
}

void UES1HealthComponent::StartDeath()
{
	if (DeathState != EDeathState::NotDead) return;
	
	DeathState = EDeathState::DeathStarted;
	OnDeathStarted.Broadcast();
	GetOwner()->ForceNetUpdate();
}
