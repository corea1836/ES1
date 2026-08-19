#include "Equipments/ES1Weapon.h"

#include "ES1GameplayTags.h"
#include "KismetTraceUtils.h"
#include "GameFramework/Character.h"
#include "Interfaces/ES1PlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"

AES1Weapon::AES1Weapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	Mesh->bReceivesDecals = false;
	Mesh->CastShadow = true;
	SetRootComponent(Mesh);
	Mesh->SetHiddenInGame(true);
	
	AimFieldOfView = 200.f;
	TraceRadius = 5.f;
	FireTime = 0.1f;
}

void AES1Weapon::OnRep_Instigator()
{
	Super::OnRep_Instigator();
	AttachToOwningPawn();
}

void AES1Weapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

USkeletalMeshComponent* AES1Weapon::GetMesh() const
{
	return Mesh;
}

void AES1Weapon::AttachToOwningPawn() const
{
	APawn* OwningPawn = GetInstigator();
	if (!IsValid(OwningPawn) || !OwningPawn->Implements<UES1PlayerInterface>()) return;
	
	SetMeshVisibilities(OwningPawn);
	
	const FName EquippedSocket = IES1PlayerInterface::Execute_GetWeaponEquippedSocket(OwningPawn, WeaponType);
	USkeletalMeshComponent* PawnMesh = IES1PlayerInterface::Execute_GetPlayerMesh(OwningPawn);
	
	Mesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, EquippedSocket);
}

void AES1Weapon::WeaponTrace(FHitResult& OutHit, float TraceLength)
{
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(GetOwner());
	
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_WorldStatic, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_WorldDynamic, ECR_Block);
	ResponseParams.CollisionResponse.SetResponse(ECC_PhysicsBody, ECR_Block);

	ensure(GetInstigator());
	if (APlayerController* PC = Cast<APlayerController>(GetInstigator()->GetController()); IsValid(PC))
	{
		FVector EyesWorldLocation;
		FRotator EyesWorldRotation;
		PC->GetActorEyesViewPoint(EyesWorldLocation, EyesWorldRotation);
		
		const FVector EyesWorldDirection = UKismetMathLibrary::GetForwardVector(EyesWorldRotation);
		
		const FVector Start = EyesWorldLocation + 15;
		const FVector End = Start + EyesWorldDirection * TraceLength;
		
		const bool bHit = GetWorld()->SweepSingleByChannel(
			OutHit,
			Start,
			End,
			FQuat::Identity,
			ES1TraceChannel::ECC_Weapon,
			FCollisionShape::MakeSphere(TraceRadius),
			QueryParams,
			ResponseParams);
		
		if (!bHit)
		{
			OutHit.ImpactPoint = End;
		}
		
		// DrawDebugSphereTraceSingle(
		// 	GetWorld(),
		// 	Start,
		// 	End,
		// 	TraceRadius,
		// 	EDrawDebugTrace::ForDuration,
		// 	bHit,
		// 	OutHit,
		// 	FColor::Green,
		// 	FColor::Red,
		// 	5.f);
	}
}

void AES1Weapon::Local_Fire(const FVector& ImpactPoint, const FVector& ImpactNormal,
	TEnumAsByte<EPhysicalSurface> ImpactSurfaceType)
{
	FireEffects(ImpactPoint, ImpactNormal, ImpactSurfaceType);
}

void AES1Weapon::BeginPlay()
{
	Super::BeginPlay();
}

TObjectPtr<UAnimMontage> AES1Weapon::GetMontage(const FGameplayTag& GroupTag) const
{
	return AnimationData->GetMontageGroup(GroupTag)->AnimMontage;
}

TObjectPtr<UAnimationAsset> AES1Weapon::GetAnimation(const FGameplayTag& GroupTag) const
{
	return AnimationData->GetAnimationGroup(GroupTag)->Animation;
}

void AES1Weapon::EquipItem()
{
}

void AES1Weapon::UnequipItem()
{
}

void AES1Weapon::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}

void AES1Weapon::Use()
{
	if (bCanUse)
	{
		bCanUse = false;
		
		GetWorld()->GetTimerManager().SetTimer(UseTimerHandle, this, &ThisClass::ToggleUse, UseInterval, false);
		UAnimationAsset* UseAnimation = GetAnimation(ES1GameplayTags::Equipment_Weap_Fire);
		if (UseAnimation)
		{
			Mesh->PlayAnimation(UseAnimation, 0.f);
		}
	}
}

void AES1Weapon::ToggleUse()
{
	bCanUse = true;
}

void AES1Weapon::SetMeshVisibilities(APawn* OwningPawn) const
{
		Mesh->SetHiddenInGame(false);
}


