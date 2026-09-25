#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Types/ES1InteractionTypes.h"
#include "ES1InteractableComponent.generated.h"


class UWidgetComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ES1_API UES1InteractableComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	// Functions
	UES1InteractableComponent();
	
	UFUNCTION(BlueprintPure, Category="ES1|Component")
	static UES1InteractableComponent* Find(const AActor* Actor) { return IsValid(Actor) ? Actor->FindComponentByClass<UES1InteractableComponent>() : nullptr; }

	void SetFocusState(ES1FocusState NewState);
	
	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Interact")
	FText Prompt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Interact")
	bool bEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Interact")
	bool bShowWidgetWhenNearby;

	UPROPERTY(EditAnywhere, Category="ES1|Interact|Widget")
	TSubclassOf<UUserWidget> PromptWidgetClass;

	UPROPERTY(EditAnywhere, Category="ES1|Interact|Widget")
	FVector WidgetOffset = FVector(0.f, 0.f, 30.f);
	
protected:
	virtual void BeginPlay() override;

private:
	// Functions
	void ApplyWidget(ES1FocusState State);
	
	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|Interact|Widget", meta=(AllowPrivateAccess=true))
	TObjectPtr<UWidgetComponent> InteractWidget;

	ES1FocusState FocusState;

};
