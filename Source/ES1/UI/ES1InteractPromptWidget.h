#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ES1InteractPromptWidget.generated.h"

class UTextBlock;

UCLASS()
class ES1_API UES1InteractPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="ES1|Interact")
	void SetPrompt(const FText& InText);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PromptText;
};
