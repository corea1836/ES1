#include "ES1PickupItem.h"

#include "Components/ES1InteractableComponent.h"

AES1PickupItem::AES1PickupItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Interactable = CreateDefaultSubobject<UES1InteractableComponent>(TEXT("Interactable"));
	Interactable->SetupAttachment(RootComponent);
}

void AES1PickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AES1PickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

