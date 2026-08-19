#pragma once

#include "CoreMinimal.h"
#include "ES1Define.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Equipments/ES1Weapon.h"
#include "ES1CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ES1_API UES1CombatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Functions
	UES1CombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void Initiate_SwitchWeapon();
	void Initiate_FireWeapon_Pressed();
	void Initiate_FireWeapon_Released();
	void Initiate_ReloadWeapon();
	void Initiate_Aim_Pressed();
	void Initiate_Aim_Released();
	
	void Equip(AES1Weapon* Weapon);
	
	void SpawnInventory();
	void DestroyInventory();
	
	TSubclassOf<UAnimInstance> GetCurrentWeaponAnimLayer() const;
	
	FORCEINLINE AES1Weapon* GetCurrentWeapon() const { return CurrentWeapon; }
	FORCEINLINE bool GetIsAiming() const { return bIsAiming; }
	
	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ES1|Weapon")
	TObjectPtr<class UES1WeaponData> WeaponData;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|Weapon")
	float TraceLength;
	
private:	
	// Functions
	UFUNCTION()
	void OnRep_CurrentWeapon(AES1Weapon* LastWeapon);
	
	UFUNCTION(Server, Reliable)
	void Server_Aim(bool bPressed);
	
	UFUNCTION(Server, Reliable)
	void Server_FireWeapon(const FHitResult& Hit);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FireWeapon(const FHitResult& Hit);
	
	AES1Weapon* SpawnWeapon(TSubclassOf<AES1Weapon> WeaponClass);
	
	void FireTimerFinished();
	
	void Local_Aim(bool bPressed);
	void Local_FireWeapon();
	
	// Variables
	UPROPERTY(Transient, BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentWeapon, meta=(AllowPrivateAccess=true))
	TObjectPtr<AES1Weapon> CurrentWeapon;
	
	UPROPERTY(Transient, Replicated)
	TArray<AES1Weapon*> WeaponInventory;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|Weapon")
	TArray<TSubclassOf<AES1Weapon>> DefaultWeaponClasses;
	
	UPROPERTY(BlueprintReadOnly, Replicated, meta=(AllowPrivateAccess=true))
	bool bIsAiming;
	
	bool bIsTriggerPressed;
	FTimerHandle FireTimer;
	
	UPROPERTY(BlueprintReadOnly, Replicated, meta=(AllowPrivateAccess=true))
	bool bIsFiring;
	
	
	
	
		
	UAnimMontage* GetSelectedEquipmentMontage(const FGameplayTag& GroupTag) const;
	UAnimationAsset* GetSelectedEquipmentAnimation(const FGameplayTag& GroupTag) const;
	
};
