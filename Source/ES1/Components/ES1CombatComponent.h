#pragma once

#include "CoreMinimal.h"
#include "ES1Define.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Equipments/ES1Equipment.h"
#include "ES1CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ES1_API UES1CombatComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AES1Weapon> PrimaryWeaponClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AES1Weapon> SecondaryWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AES1Weapon> SideWeaponClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AES1Weapon> PrimaryWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AES1Weapon> SecondaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AES1Weapon> SideWeapon;

public:	
	UES1CombatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly)
	EES1SelectedWeaponSlot SelectedSlot = EES1SelectedWeaponSlot::Unarmed;
	
	UPROPERTY(EditDefaultsOnly)
	AES1Equipment* SelectedEquipment;
	
public:
	FORCEINLINE EES1SelectedWeaponSlot GetSelectedSlot() const { return SelectedSlot; }
	AES1Equipment* GetSelectedEquipment() const;
	
	void SwitchEquipment(EES1SelectedWeaponSlot IncomingSlot);
	UAnimMontage* GetSelectedEquipmentMontage(const FGameplayTag& GroupTag) const;
	UAnimationAsset* GetSelectedEquipmentAnimation(const FGameplayTag& GroupTag) const;
	
	AES1Weapon* Spawn(TSubclassOf<AES1Weapon> WeaponClass);
	
	bool UseSelectedEquipment();
};
