#include "ES1ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipments/ES1Projectile.h"
#include "Interfaces/ES1PlayerInterface.h"

void AES1ProjectileWeapon::PlayFire(const FVector& HitTarget)
{
	Super::PlayFire(HitTarget);
}

TObjectPtr<AES1Projectile> AES1ProjectileWeapon::SpawnProjectile(const FVector& HitTarget, bool bCosmetic)
{
	APawn* WeaponInstigator = GetInstigator();
	if (!IsValid(ProjectileClass) || !IsValid(WeaponInstigator) || !IsValid(Mesh)) return nullptr;

	const USkeletalMeshSocket* MuzzleFlashSocket = Mesh->GetSocketByName(FName("MuzzleFlash"));
	if (!MuzzleFlashSocket) return nullptr;

	if (USkeletalMeshComponent* OwnerMesh = IES1PlayerInterface::Execute_GetPlayerMesh(WeaponInstigator))
		OwnerMesh->RefreshBoneTransforms();

	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(Mesh);
	const FVector MuzzleLocation = SocketTransform.GetLocation();
	const FRotator SpawnRotation = (HitTarget - MuzzleLocation).Rotation();
	const FTransform SpawnTM(SpawnRotation, MuzzleLocation);

	UWorld* World = GetWorld();
	if (!IsValid(World)) return nullptr;

	AES1Projectile* Projectile = World->SpawnActorDeferred<AES1Projectile>(
		ProjectileClass,
		SpawnTM,
		GetOwner(),
		WeaponInstigator,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (IsValid(Projectile))
	{
		Projectile->bCosmetic = bCosmetic;
		Projectile->FinishSpawning(SpawnTM);
	}

	return Projectile;
}

void AES1ProjectileWeapon::Local_Fire(const FVector& HitTarget)
{
	Super::Local_Fire(HitTarget);
	SpawnProjectile(HitTarget, true);
}

void AES1ProjectileWeapon::Auth_Fire(const FVector& HitTarget)
{
	Super::Auth_Fire(HitTarget);
	SpawnProjectile(HitTarget, false);
}
