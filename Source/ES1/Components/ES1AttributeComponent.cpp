#include "Components/ES1AttributeComponent.h"


UES1AttributeComponent::UES1AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UES1AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UES1AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


