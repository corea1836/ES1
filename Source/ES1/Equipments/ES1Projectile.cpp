#include "ES1Projectile.h"

#include "Characters/ES1Character.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Types/ES1CoreTypes.h"

AES1Projectile::AES1Projectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	CollisionBox->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionBox->SetAllUseCCD(true);
	
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ES1TraceChannel::ECC_SkeletalMesh, ECR_Block);

	ProjectileMovementComponent= CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void AES1Projectile::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Spawn at %s"), *GetActorLocation().ToString());
	if (IsValid(Tracer))
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition);
	}
	
	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AES1Projectile::OnHit);
	}
	
}

void AES1Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AES1Projectile::Destroyed()
{
	Super::Destroyed();
	if (IsValid(ImpactParticles))
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	if (IsValid(ImpactSound))
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

}

void AES1Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit)
{
	SetActorLocation(Hit.ImpactPoint);   
	SetActorEnableCollision(false);
	if (IsValid(ProjectileMovementComponent))
		ProjectileMovementComponent->StopMovementImmediately();
	SetLifeSpan(0.02f);  
}

