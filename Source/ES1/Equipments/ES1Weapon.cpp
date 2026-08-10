#include "Equipments/ES1Weapon.h"

#include "ES1GameplayTags.h"

AES1Weapon::AES1Weapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(Mesh);
}

void AES1Weapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (MeshAsset)
	{
		Mesh->SetSkeletalMesh(MeshAsset);
	}
}

void AES1Weapon::EquipItem()
{
	Super::EquipItem();
	
	FName AttachSocketName = GetEquipSocketName();
	AttachToOwner(AttachSocketName);
}

void AES1Weapon::UnequipItem()
{
	Super::UnequipItem();
	
	FName AttachSocketName = GetUnequipSocketName();
	AttachToOwner(AttachSocketName);
}

void AES1Weapon::Reload()
{
	UAnimationAsset* ReloadAnimation = GetAnimation(ES1GameplayTags::Equipment_Weap_Reload);
	if (ReloadAnimation)
	{
		Mesh->PlayAnimation(ReloadAnimation, 0.f);
	}
}
