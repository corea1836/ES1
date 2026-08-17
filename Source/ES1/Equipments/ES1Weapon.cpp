#include "Equipments/ES1Weapon.h"

#include "ES1GameplayTags.h"
#include "GameFramework/Character.h"
#include "Interfaces/ES1PlayerInterface.h"

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


