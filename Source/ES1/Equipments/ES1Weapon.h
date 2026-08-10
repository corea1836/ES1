#pragma once

#include "CoreMinimal.h"
#include "Equipments/ES1Equipment.h"
#include "ES1Weapon.generated.h"

UCLASS()
class ES1_API AES1Weapon : public AES1Equipment
{
	GENERATED_BODY()
	

public:
	AES1Weapon();
	
public:
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	virtual void EquipItem() override;
	virtual void UnequipItem() override;
	
	void Reload();
};
