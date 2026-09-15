#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ES1PlayerInterface.generated.h"

UINTERFACE()
class UES1PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class ES1_API IES1PlayerInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FName GetWeaponEquippedSocket(const struct FGameplayTag& WeaponType) const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetPlayerMesh() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void WeaponReplicated();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AES1BaseWeapon* GetCurrentWeapon();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetReserveAmmo() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Notify_CycleWeapon();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Notify_ReloadWeapon();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddAmmo(const FGameplayTag& WeaponType, int32 AmmoAmount);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool DoDamage(float DamageAmount, AActor* DamageInstigator);
};
