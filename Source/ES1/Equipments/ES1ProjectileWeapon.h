#pragma once

#include "CoreMinimal.h"
#include "ES1LyraWeapon.h"
#include "ES1ProjectileWeapon.generated.h"

class AES1Projectile;

UCLASS()
class ES1_API AES1ProjectileWeapon : public AES1Weapon
{
	GENERATED_BODY()

public:
	// Functions
	virtual void PlayFire(const FVector& HitTarget);

	TObjectPtr<AES1Projectile> SpawnProjectile(const FVector& HitTarget, bool bCosmetic);

	virtual void Local_Fire(const FVector& HitTarget) override;
	virtual void Auth_Fire(const FVector& HitTarget) override;

	// Variables
protected:

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AES1Projectile> ProjectileClass;
};
