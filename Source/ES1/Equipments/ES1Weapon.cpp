#include "ES1Weapon.h"

#include "ES1BulletShell.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Interfaces/ES1PlayerInterface.h"

AES1Weapon::AES1Weapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	// Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	Mesh->bReceivesDecals = false;
	Mesh->CastShadow = true;
	SetRootComponent(Mesh);
	Mesh->SetHiddenInGame(false);

	AimFieldOfView = 200.f;
	FireTime = 0.1f;

	TraceLength = 20'000;

	Ammo = 5;
	StartingCarriedAmmo = 10;
	Sequence;

	WeaponStatus = ES1WeaponStatus::Idle;
}

void AES1Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AES1Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AES1Weapon::AttachToOwningPawn(APawn* Pawn) const
{
	if (!IsValid(Pawn) || !Pawn->Implements<UES1PlayerInterface>()) return;

	Mesh->SetVisibility(true);
	Mesh->SetHiddenInGame(false);

	const FName EquippedSocket = IES1PlayerInterface::Execute_GetWeaponEquippedSocket(Pawn, WeaponType);
	USkeletalMeshComponent* PawnMesh = IES1PlayerInterface::Execute_GetPlayerMesh(Pawn);

	Mesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, EquippedSocket);
}

void AES1Weapon::DetachFromOwningPawn()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->SetHiddenInGame(true);
}

void AES1Weapon::PlayFire(const FVector& HitTarget)
{
	if (!IsValid(FireAnimation)) return;
	Mesh->PlayAnimation(FireAnimation, false);

	if (BulletShellClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = Mesh->GetSocketByName(FName("AmmoEject"));
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(Mesh);

			if (UWorld* World = GetWorld(); IsValid(World))
			{
				World->SpawnActor<AES1BulletShell>(
					BulletShellClass,
					SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator()
				);
			}
		}
	}
}

void AES1Weapon::Local_Fire(const FVector& HitTarget)
{
	PlayFire(HitTarget);
	if (GetInstigator()->IsLocallyControlled())
	{
		Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
		++Sequence;
	}
}

void AES1Weapon::Auth_Fire(const FVector& HitTarget)
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
}

void AES1Weapon::Rep_Fire(int32 AuthAmmo)
{
	if (GetInstigator()->IsLocallyControlled())
	{
		Ammo = AuthAmmo;
		--Sequence;
		Ammo -= Sequence;
	}
}

void AES1Weapon::PlayReload()
{
	if (!IsValid(ReloadAnimation)) return;

	Mesh->PlayAnimation(ReloadAnimation, false);
}

UMaterialInstanceDynamic* AES1Weapon::GetReticleDynamicMaterialInstance()
{
	if (!IsValid(DynMatInst_Reticle))
	{
		DynMatInst_Reticle = UMaterialInstanceDynamic::Create(ReticleMaterial, this);
	}
	return DynMatInst_Reticle;
}

