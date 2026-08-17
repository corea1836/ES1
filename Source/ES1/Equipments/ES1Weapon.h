#pragma once

#include "CoreMinimal.h"
#include "ES1Define.h"
#include "Data/ES1AnimationData.h"
#include "GameFramework/Actor.h"
#include "ES1Weapon.generated.h"

UCLASS()
class ES1_API AES1Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AES1Weapon();
	
	virtual void OnRep_Instigator() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	USkeletalMeshComponent* GetMesh() const;
	
	void AttachToOwningPawn() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|WeaponType")
	FGameplayTag WeaponType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ES1|Camera|Aming")
	float AimFieldOfView;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Data")
	float UseInterval;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Animation")
	TObjectPtr<UES1AnimationData> AnimationData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanUse = true;
	
	FTimerHandle UseTimerHandle;
	
protected:
	virtual void BeginPlay() override;
	


private:
	void SetMeshVisibilities(APawn* OwningPawn) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1 | Mesh",  meta=(AllowPrivateAccess=true))
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
public:
	FORCEINLINE float GetUseInterval() const { return UseInterval; }

	TObjectPtr<UAnimMontage> GetMontage(const FGameplayTag& GroupTag) const;
	TObjectPtr<UAnimationAsset> GetAnimation(const FGameplayTag& GroupTag) const;
	
public:
	virtual void EquipItem();
	virtual void UnequipItem();
	virtual void AttachToOwner(FName SocketName);
	virtual void Use();
	void ToggleUse();
};