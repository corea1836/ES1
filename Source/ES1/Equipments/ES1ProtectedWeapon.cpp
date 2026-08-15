#include "Equipments/ES1ProtectedWeapon.h"

#include "ES1GameplayTags.h"

AES1ProtectedWeapon::AES1ProtectedWeapon()
{
	// Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentMesh"));
	// SetRootComponent(Mesh);
}

void AES1ProtectedWeapon::OnConstruction(const FTransform& Transform)
{
	// Super::OnConstruction(Transform);
	//
	// if (MeshAsset)
	// {
	// 	Mesh->SetSkeletalMesh(MeshAsset);
	// }
}

void AES1ProtectedWeapon::EquipItem()
{
	Super::EquipItem();

}

void AES1ProtectedWeapon::UnequipItem()
{
	Super::UnequipItem();

}

void AES1ProtectedWeapon::Reload()
{
	// UAnimationAsset* ReloadAnimation = GetAnimation(ES1GameplayTags::Equipment_Weap_Reload);
	// if (ReloadAnimation)
	// {
	// 	Mesh->PlayAnimation(ReloadAnimation, 0.f);
	// }
}
