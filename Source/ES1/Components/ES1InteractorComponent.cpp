#include "ES1InteractorComponent.h"

#include "ES1InteractableComponent.h"
#include "Engine/OverlapResult.h"
#include "Types/ES1CoreTypes.h"

UES1InteractorComponent::UES1InteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}


void UES1InteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const APawn* OwningPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwningPawn) || !OwningPawn->IsLocallyControlled()) return;

	Refresh();
}

void UES1InteractorComponent::Refresh()
{
	APawn* OwningPawn = Cast<APawn>(GetOwner());

	FVector EyeLocation;
	FRotator ViewRotation;
	OwningPawn->GetActorEyesViewPoint(EyeLocation, ViewRotation);

	const FVector Direction = ViewRotation.Vector();
	const FVector End = EyeLocation + Direction * InteractDistance;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	TArray<FHitResult> Hits;
	GetWorld()->SweepMultiByChannel(
		Hits,
		EyeLocation,
		End,
		FQuat::Identity,
		ES1TraceChannel::ECC_Interact,
		FCollisionShape::MakeSphere(CandidateRadius),
		Params);

	TSet<UES1InteractableComponent*> NewCandidates;
	UES1InteractableComponent* NewFocus = nullptr;

	float BestDistSquared = FLT_MAX;

	for (const FHitResult& Hit : Hits)
	{
		UES1InteractableComponent* Interactable = UES1InteractableComponent::Find(Hit.GetActor());
		if (!IsValid(Interactable) || !Interactable->bEnabled) continue;
		if (NewCandidates.Contains(Interactable)) continue;
		
		const FVector ToTarget = Interactable->GetComponentLocation() - EyeLocation;
		
		if (FVector::DotProduct(ToTarget, Direction) < 0.f) continue;
		
		NewCandidates.Add(Interactable);

		const float OffAxis = FVector::CrossProduct(ToTarget, Direction).Size();
		const float NewFocusRadius = (Interactable == FocusedComponent) ? FocusRadius * StickyScale : FocusRadius;
		const float DistSquared = ToTarget.SizeSquared();

		if (OffAxis <= NewFocusRadius && DistSquared < BestDistSquared)
		{
			BestDistSquared = DistSquared;
			NewFocus = Interactable;
		}
	}

	for (UES1InteractableComponent* OldCandidates : Candidates)
	{
		if (IsValid(OldCandidates) && !NewCandidates.Contains(OldCandidates))
		{
			OldCandidates->SetFocusState(ES1FocusState::None);
		}
	}

	for (UES1InteractableComponent* Interactable : NewCandidates)
	{
		if (!IsValid(Interactable)) continue;

		Interactable->SetFocusState(Interactable == NewFocus ? ES1FocusState::Focused : ES1FocusState::Nearby);
	}

	const bool bFocusChanged = (FocusedComponent != NewFocus);
	Candidates = MoveTemp(NewCandidates);
	FocusedComponent = NewFocus;

	const FQuat Q = ViewRotation.Quaternion();
	// DrawDebugLine(GetWorld(), EyeLocation, End, FColor::Silver, false, 0.06f, 0, 1.f);
	// DrawDebugCircle(GetWorld(), End, CandidateRadius, 24, FColor::Blue, false, 0.06f, 0, 1.f,
	// 				Q.GetRightVector(), Q.GetUpVector(), false);
	// DrawDebugCircle(GetWorld(), End, FocusRadius, 16, FColor::Green, false, 0.06f, 0, 1.f,
	// 				Q.GetRightVector(), Q.GetUpVector(), false);
}

bool UES1InteractorComponent::IsVisibleFrom(const FVector& EyeLocation, UES1InteractableComponent* Interactable) const
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Interactable->GetOwner());

	FHitResult VisibleHit;
	const bool bBlocked = GetWorld()->LineTraceSingleByChannel(
		VisibleHit,
		EyeLocation,
		Interactable->GetComponentLocation(),
		ECC_Visibility,
		Params);

	return !bBlocked;
}

