#include "ES1InteractPromptWidget.h"

#include "Components/TextBlock.h"

void UES1InteractPromptWidget::SetPrompt(const FText& InText)
{
	if (IsValid(PromptText)) PromptText->SetText(InText);
}
