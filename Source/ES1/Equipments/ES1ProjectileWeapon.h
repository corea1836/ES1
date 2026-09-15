#pragma once

#include "CoreMinimal.h"
#include "ES1Weapon.h"
#include "ES1ProjectileWeapon.generated.h"

class AES1Projectile;

UCLASS()
class ES1_API AES1ProjectileWeapon : public AES1BaseWeapon
{
	GENERATED_BODY()

public:
	// Functions
	virtual void PlayFire(const FVector& HitTarget);

	// Variables
protected:

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AES1Projectile> ProjectileClass;
};
