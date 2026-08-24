#pragma once

#include "CoreMinimal.h"
#include "ES1Define.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Equipments/ES1Weapon.h"
#include "ES1CombatComponent.generated.h"

class UES1WeaponData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReticleChanged, UMaterialInstanceDynamic*, ReticleDynMatInst, const FReticleParams&, ReticleParams);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAmmoCounterChanged, UMaterialInstanceDynamic*, AmmoCounterDynMatInst, int32, RoundCurrent, int32, RoundsMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRoundFired, int32, RoundsCurrent, int32, RoundsMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimingStatusChanged, bool, bIsAiming);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ES1_API UES1CombatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Functions
	UES1CombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintPure, Category="ES1|Component")
	static UES1CombatComponent* FindCombatComponent(const AActor* Actor) { return (IsValid(Actor) ? Actor->FindComponentByClass<UES1CombatComponent>() : nullptr); }
	
	TSubclassOf<UAnimInstance> GetCurrentWeaponAnimLayer() const;
	FORCEINLINE AES1Weapon* GetCurrentWeapon() const { return CurrentWeapon; }
	FORCEINLINE bool GetIsAiming() const { return bAiming; }
	
	void Initiate_SwitchWeapon();
	void Initiate_FireWeapon_Pressed();
	void Initiate_FireWeapon_Released();
	void Initiate_ReloadWeapon();
	void Initiate_Aim_Pressed();
	void Initiate_Aim_Released();
	
	void Equip(AES1Weapon* Weapon);
	
	void SpawnInventory();
	void DestroyInventory();
	
	void InitializeWeaponWidgets() const;
	
	UPROPERTY(BlueprintAssignable)
	FReticleChanged OnReticleChanged;
	
	UPROPERTY(BlueprintAssignable)
	FAmmoCounterChanged OnAmmoCounterChanged;
	
	UPROPERTY(BlueprintAssignable)
	FRoundFired OnRoundFired;
	
	UPROPERTY(BlueprintAssignable)
	FAimingStatusChanged OnAimingStatusChanged;
	
	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ES1|Weapon")
	TObjectPtr<UES1WeaponData> WeaponData;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|Weapon")
	float TraceLength;
	
private:	
	// Functions
	UFUNCTION()
	void OnRep_CurrentWeapon(AES1Weapon* LastWeapon);
	
	UFUNCTION(Server, Reliable)
	void Server_Aim(bool bIsPressed);
	
	UFUNCTION(Server, Reliable)
	void Server_FireWeapon(const FHitResult& Hit);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FireWeapon(const FHitResult& Hit, int32 AuthAmmo);
	
	AES1Weapon* SpawnWeapon(TSubclassOf<AES1Weapon> WeaponClass);
	
	void FireTimerFinished();
	
	void Local_Aim(bool bIsPressed);
	void Local_FireWeapon();
	
	// Variables
	UPROPERTY(Transient, BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentWeapon, meta=(AllowPrivateAccess=true))
	TObjectPtr<AES1Weapon> CurrentWeapon;
	
	UPROPERTY(Transient, Replicated)
	TArray<AES1Weapon*> WeaponInventory;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|Weapon")
	TArray<TSubclassOf<AES1Weapon>> DefaultWeaponClasses;
	
	UPROPERTY(BlueprintReadOnly, Replicated, meta=(AllowPrivateAccess=true))
	bool bAiming;
	
	bool bTriggerPressed;
	FTimerHandle FireTimer;
	
	UPROPERTY(BlueprintReadOnly, Replicated, meta=(AllowPrivateAccess=true))
	bool bFiring;	
};
