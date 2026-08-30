#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ES1WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FES1WeaponAnim
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> PlayerFireMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> PlayerReloadMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> PlayerEquipMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimationAsset> WeaponFireAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimationAsset> WeaponReloadAnim = nullptr;
};

USTRUCT(BlueprintType)
struct FES1PlayerAnimInstance
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimInstance;
};

UCLASS()
class ES1_API UES1WeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="ES1|WeaponData|Weapon")
	TMap<FGameplayTag, FName> EquippedSocketData;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|WeaponData|Weapon")
	TMap<FGameplayTag, FES1WeaponAnim> WeaponAnims;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|WeaponData|Weapon")
	TMap<FGameplayTag, FName> UnequippedSocketData;
	
	UPROPERTY(EditDefaultsOnly, Category="ES1|WeaponData|Animation")
	TMap<FGameplayTag, FES1PlayerAnimInstance> PlayerAnims;
};
