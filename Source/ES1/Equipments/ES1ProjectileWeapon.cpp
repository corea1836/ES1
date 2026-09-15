#include "ES1ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipments/ES1Projectile.h"
#include "Interfaces/ES1PlayerInterface.h"

void AES1ProjectileWeapon::PlayFire(const FVector& HitTarget)
{
	Super::PlayFire(HitTarget);
	
	if (!HasAuthority()) return;

	APawn* Instigator = GetInstigator();

	USkeletalMeshComponent* OwnerMesh = IES1PlayerInterface::Execute_GetPlayerMesh(Instigator);

	if (IsValid(OwnerMesh))
	{
		OwnerMesh->RefreshBoneTransforms();
	}
	
	if (const USkeletalMeshSocket* MuzzleFlashSocket = Mesh->GetSocketByName(FName("MuzzleFlash")))
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(Mesh);
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();

		if (IsValid(ProjectileClass) && IsValid(Instigator))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			SpawnParams.Instigator = Instigator;
			UWorld* World = GetWorld();
			if (IsValid(World))
			{
				World->SpawnActor<AES1Projectile>(
				
					ProjectileClass,
					SocketTransform.GetLocation(),
					TargetRotation,
					SpawnParams
				);
			}
		}
	}
	
	
}
