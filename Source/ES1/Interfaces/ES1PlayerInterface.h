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
	AES1Weapon* GetCurrentWeapon();
};
