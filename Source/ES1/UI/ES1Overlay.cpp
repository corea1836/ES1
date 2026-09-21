#include "UI/ES1Overlay.h"

#include "Characters/ES1Character.h"
#include "Components/ES1HealthComponent.h"
#include "Components/ProgressBar.h"

void UES1Overlay::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetOwningPlayer()->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);

	AES1Character* Character = Cast<AES1Character>(GetOwningPlayer()->GetPawn());
	if (!IsValid(Character)) return;

	OnPossessedPawnChanged(nullptr, Character);

	UES1HealthComponent* HealthComponent = UES1HealthComponent::FindHealthComponent(Character);
	OnHpChanged(HealthComponent, 0.f, 0.f, Character);
}

void UES1Overlay::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	UES1HealthComponent* OldHealthComponent = UES1HealthComponent::FindHealthComponent(OldPawn);
	if (IsValid(OldHealthComponent))
	{
		OldHealthComponent->OnHealthChanged.RemoveDynamic(this, &ThisClass::OnHpChanged);
	}

	UES1HealthComponent* NewHealthComponent = UES1HealthComponent::FindHealthComponent(NewPawn);
	if (IsValid(NewHealthComponent))
	{
		NewHealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHpChanged);
	}
}

void UES1Overlay::OnHpChanged(UES1HealthComponent* HealthComponent,float OldValue, float NewValue,AActor* Instigator)
{
	if (!HealthBar) return;
	const float CurrentHp = HealthComponent->GetHealthNormalize();
	HealthBar->SetPercent(CurrentHp);
}
