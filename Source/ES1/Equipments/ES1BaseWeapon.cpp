#include "ES1BaseWeapon.h"

#include "ES1BulletShell.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Interfaces/ES1PlayerInterface.h"

AES1BaseWeapon::AES1BaseWeapon()
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
}

void AES1BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AES1BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AES1BaseWeapon::AttachToOwningPawn(APawn* Pawn) const
{
	if (!IsValid(Pawn) || !Pawn->Implements<UES1PlayerInterface>()) return;

	Mesh->SetVisibility(true);

	const FName EquippedSocket = IES1PlayerInterface::Execute_GetWeaponEquippedSocket(Pawn, WeaponType);
	USkeletalMeshComponent* PawnMesh = IES1PlayerInterface::Execute_GetPlayerMesh(Pawn);

	Mesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepRelativeTransform, EquippedSocket);
}

void AES1BaseWeapon::DetachFromOwningPawn()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->SetHiddenInGame(true);
}

void AES1BaseWeapon::PlayFire(const FVector& HitTarget)
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

void AES1BaseWeapon::Local_Fire(const FVector& HitTarget)
{
	PlayFire(HitTarget);
	if (GetInstigator()->IsLocallyControlled())
	{
		Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
		++Sequence;
	}
}

void AES1BaseWeapon::Auth_Fire(const FVector& HitTarget)
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
}

void AES1BaseWeapon::Rep_Fire(int32 AuthAmmo)
{
	if (GetInstigator()->IsLocallyControlled())
	{
		Ammo = AuthAmmo;
		--Sequence;
		Ammo -= Sequence;
	}
}

UMaterialInstanceDynamic* AES1BaseWeapon::GetReticleDynamicMaterialInstance()
{
	if (!IsValid(DynMatInst_Reticle))
	{
		DynMatInst_Reticle = UMaterialInstanceDynamic::Create(ReticleMaterial, this);
	}
	return DynMatInst_Reticle;
}

