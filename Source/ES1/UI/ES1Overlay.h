#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ES1Overlay.generated.h"

class UES1HealthComponent;
class UProgressBar;

UCLASS()
class ES1_API UES1Overlay : public UUserWidget
{
	GENERATED_BODY()

public:
	// Functions
	virtual void NativeOnInitialized() override;

	// Variables
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

private:
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void OnHpChanged(UES1HealthComponent* HealthComponent,float OldValue, float NewValue,AActor* Instigator);
};
