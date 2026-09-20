#pragma once

#include "CoreMinimal.h"
#include "ES1Projectile.h"
#include "ES1ProjectileBullet.generated.h"

UCLASS()
class ES1_API AES1ProjectileBullet : public AES1Projectile
{
	GENERATED_BODY()
	
protected:
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
