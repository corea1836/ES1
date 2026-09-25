#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ES1BulletShell.generated.h"

UCLASS()
class ES1_API AES1BulletShell : public AActor
{
	GENERATED_BODY()

public:
	AES1BulletShell();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> ShellSound;
};
