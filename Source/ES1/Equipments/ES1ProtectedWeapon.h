#pragma once

#include "CoreMinimal.h"
#include "Equipments/ES1Weapon.h"
#include "ES1ProtectedWeapon.generated.h"

UCLASS()
class ES1_API AES1ProtectedWeapon : public AES1Weapon
{
	GENERATED_BODY()
	

public:
	AES1ProtectedWeapon();
	
public:
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	virtual void EquipItem() override;
	virtual void UnequipItem() override;
	
	void Reload();
};
