#include "ES1InteractableComponent.h"

#include "Components/WidgetComponent.h"
#include "Types/ES1CoreTypes.h"
#include "Types/ES1InteractionTypes.h"
#include "UI/ES1InteractPromptWidget.h"

UES1InteractableComponent::UES1InteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InitSphereRadius(SphereRadius);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECC_WorldDynamic);          
	SetCollisionResponseToAllChannels(ECR_Ignore);         
	SetCollisionResponseToChannel(ES1TraceChannel::ECC_Interact, ECR_Overlap); 
    
	SetGenerateOverlapEvents(true);   // ← 반드시 true! (네 코드는 false라 감지 안 됨)


	bEnabled = true;
	bShowWidgetWhenNearby = true;
	FocusState = ES1FocusState::None;
}

void UES1InteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!IsValid(Owner) || !IsValid(PromptWidgetClass)) return;

	USceneComponent* AttachTarget = Owner->GetRootComponent();
	if (!IsValid(AttachTarget)) return;

	InteractWidget = NewObject<UWidgetComponent>(Owner); 
	InteractWidget->SetWidgetClass(PromptWidgetClass);
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractWidget->SetDrawAtDesiredSize(true);
	InteractWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractWidget->SetHiddenInGame(false);
	InteractWidget->SetVisibility(false);

	InteractWidget->SetupAttachment(AttachTarget);    
	InteractWidget->SetRelativeLocation(WidgetOffset);
	InteractWidget->RegisterComponent();
	
	if (!IsValid(InteractWidget->GetUserWidgetObject()))
	{
		InteractWidget->InitWidget();
	}

	if (UES1InteractPromptWidget* W =
			Cast<UES1InteractPromptWidget>(InteractWidget->GetUserWidgetObject()))
	{
		W->SetPrompt(Prompt);
	}
}

void UES1InteractableComponent::SetFocusState(ES1FocusState NewState)
{
	if (FocusState == NewState) return;
	FocusState = NewState;

	ApplyWidget(FocusState);
}

void UES1InteractableComponent::ApplyWidget(ES1FocusState State)
{
	if (!IsValid(InteractWidget)) return;;

	const bool bShow = bShowWidgetWhenNearby
		? (State != ES1FocusState::None)
		: (State == ES1FocusState::Focused);
	
	InteractWidget->SetVisibility(bShow);
}

