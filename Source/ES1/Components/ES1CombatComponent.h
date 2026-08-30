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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRoundFired, int32, RoundsCurrent, int32, RoundsMax, int32, RoundsInReserve);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAimingStatusChanged, bool, bIsAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCurrentReserveAmmoChanged, int32, RoundsInReserve, int32, RoundsInWeapon, UMaterialInterface*, WeaponIconMaterial);

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
	
	void Notify_CycleWeapon();
	void Notify_ReloadWeapon();
	
	void Equip(AES1Weapon* Weapon);
	void EquipWeapon(AES1Weapon* Weapon);
	
	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon(AES1Weapon* Weapon);
	
	void SpawnInventory();
	void DestroyInventory();
	
	void InitializeWeaponWidgets() const;
	
	void AddAmmo(const FGameplayTag& WeaponType, int32 AmmoAmount);
	
	UPROPERTY(BlueprintAssignable)
	FReticleChanged OnReticleChanged;
	
	UPROPERTY(BlueprintAssignable)
	FAmmoCounterChanged OnAmmoCounterChanged;
	
	UPROPERTY(BlueprintAssignable)
	FRoundFired OnRoundFired;
	
	UPROPERTY(BlueprintAssignable)
	FAimingStatusChanged OnAimingStatusChanged;
	
	UPROPERTY(BlueprintAssignable)
	FCurrentReserveAmmoChanged OnCurrentReserveeAmmoChanged;
	
	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ES1|Weapon")
	TObjectPtr<UES1WeaponData> WeaponData;
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentReserveAmmo)
	int32 CurrentReserveAmmo;
	
protected:
	// Functions
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void BlendOut_CycleWeapon(UAnimMontage* Montage, bool bInterrupted);
	
	// Variables
	UPROPERTY(EditDefaultsOnly, Category="ES1|Weapon")
	float TraceLength;
	
private:	
	// Functions
	UFUNCTION()
	void OnRep_CurrentWeapon(AES1Weapon* LastWeapon);
	
	UFUNCTION()
	void OnRep_CurrentReserveAmmo();
	
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
	
	int32 AdvancedWeaponIndex();
	
	void Local_CycleWeapon(int32 WeaponIndex);
	
	UFUNCTION(Server, Reliable)
	void Server_CycleWeapon(int32 WeaponIndex);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CycleWeapon(int32 WeaponIndex);
	
	void SetCurrentWeapon(AES1Weapon* NewWeapon, AES1Weapon* LastWeapon);
	
	void Local_ReloadWeapon();
	
	UFUNCTION(Server, Reliable)
	void Server_ReloadWeapon();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReloadWeapon();
	
	UFUNCTION(Client, Reliable)
	void Client_ReloadWeapon(int32 NewWeaponAmmo, int32 NewCarriedAmmo);
	
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
	
	TMap<FGameplayTag, int32> ReserveAmmo;
	
	int32 Local_WeaponIndex;
};
