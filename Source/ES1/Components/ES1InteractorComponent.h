#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ES1InteractorComponent.generated.h"


class UES1InteractableComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ES1_API UES1InteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UES1InteractorComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Variables
	UPROPERTY(EditDefaultsOnly, Category="ES1|Interact")
	float InteractDistance;

	UPROPERTY(EditDefaultsOnly, Category="ES1|Interact")
	float CandidateRadius;

	UPROPERTY(EditDefaultsOnly, Category="ES1|Interact")
	float FocusRadius;

	UPROPERTY(EditDefaultsOnly, Category="ES1|Interact")
	float StickyScale;
	
private:
	// Functions
	void Refresh();
	bool IsVisibleFrom(const FVector& EyeLocation, UES1InteractableComponent* Interactable) const;

	// Variables
	UPROPERTY(Transient)
	TSet<UES1InteractableComponent*> Candidates;
	
	UPROPERTY(Transient)
	TObjectPtr<UES1InteractableComponent> FocusedComponent;
};
