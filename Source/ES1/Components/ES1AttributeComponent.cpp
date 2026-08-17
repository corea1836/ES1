#include "Components/ES1AttributeComponent.h"

#include "Characters/ES1Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


UES1AttributeComponent::UES1AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UES1AttributeComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UES1AttributeComponent, CurrentMovementGate);
}

void UES1AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UES1AttributeComponent::OnRep_MovementGate()
{
	SwitchGate(CurrentMovementGate);
}

void UES1AttributeComponent::SwitchGate(const EES1MovementGate MovementGate)
{
	AES1Character* Owner = Cast<AES1Character>(GetOwner());
	if (!IsValid(Owner)) return;
	
	CurrentMovementGate = MovementGate;
	
	UCharacterMovementComponent* CMC = Owner->GetCharacterMovement();
	if (!IsValid(CMC)) return;
	
	CMC->MaxWalkSpeed = GetMaxWalkSpeed(MovementGate);
	CMC->MaxAcceleration = GetMaxAcceleration(MovementGate);
	CMC->BrakingDecelerationWalking = GetBrakingDeceleration(MovementGate);
	CMC->BrakingFrictionFactor = GetBrakingFrictionFactor(MovementGate);
	CMC->BrakingFriction = GetBrakingFriction(MovementGate);
	CMC->bUseSeparateBrakingFriction = GetbUseSeperateBrakingFriction(MovementGate);
}

void UES1AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


