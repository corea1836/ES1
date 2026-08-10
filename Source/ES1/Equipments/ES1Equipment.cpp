#include "Equipments/ES1Equipment.h"

#include "ES1GameplayTags.h"
#include "GameFramework/Character.h"

AES1Equipment::AES1Equipment()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AES1Equipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void AES1Equipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AES1Equipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

TObjectPtr<UAnimMontage> AES1Equipment::GetMontage(const FGameplayTag& GroupTag) const
{
	return AnimationData->GetMontageGroup(GroupTag)->AnimMontage;
}

TObjectPtr<UAnimationAsset> AES1Equipment::GetAnimation(const FGameplayTag& GroupTag) const
{
	return AnimationData->GetAnimationGroup(GroupTag)->Animation;
}

void AES1Equipment::EquipItem()
{
}

void AES1Equipment::UnequipItem()
{
}

void AES1Equipment::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}

void AES1Equipment::Use()
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

void AES1Equipment::ToggleUse()
{
	bCanUse = true;
}

