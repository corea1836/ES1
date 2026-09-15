#include "ES1BulletShell.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AES1BulletShell::AES1BulletShell()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 50.f;
}

void AES1BulletShell::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	Mesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
	
}

void AES1BulletShell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}

	Destroy();
}

