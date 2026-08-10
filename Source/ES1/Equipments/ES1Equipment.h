#pragma once

#include "CoreMinimal.h"
#include "ES1Define.h"
#include "Data/ES1AnimationData.h"
#include "GameFramework/Actor.h"
#include "ES1Equipment.generated.h"

UCLASS()
class ES1_API AES1Equipment : public AActor
{
	GENERATED_BODY()
	
public:	
	AES1Equipment();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Socket")
	FName EquipSocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Socket")
	FName UnequipSocketName;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Mesh")
	TObjectPtr<USkeletalMesh> MeshAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Mesh")
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Type")
	EES1EquipmentType EquipmentType = EES1EquipmentType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Data")
	float UseInterval;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment | Animation")
	TObjectPtr<UES1AnimationData> AnimationData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanUse = true;
	
	FTimerHandle UseTimerHandle;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	FORCEINLINE EES1EquipmentType GetEquipmentType() const { return EquipmentType; }
	FORCEINLINE float GetUseInterval() const { return UseInterval; }
	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }
	
	TObjectPtr<UAnimMontage> GetMontage(const FGameplayTag& GroupTag) const;
	TObjectPtr<UAnimationAsset> GetAnimation(const FGameplayTag& GroupTag) const;
	
public:
	virtual void EquipItem();
	virtual void UnequipItem();
	virtual void AttachToOwner(FName SocketName);
	virtual void Use();
	void ToggleUse();
};
