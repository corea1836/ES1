// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ES1PickupItem.generated.h"

class UES1InteractableComponent;

UCLASS()
class ES1_API AES1PickupItem : public AActor
{
	GENERATED_BODY()

public:
	AES1PickupItem();
	virtual void Tick(float DeltaTime) override;



protected:
	virtual void BeginPlay() override;

private:
	
	// Variables
	UPROPERTY(VisibleAnywhere, Category="ES1|Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<USkeletalMeshComponent> Mesh;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Interact", meta=(AllowPrivateAccess=true))
	TObjectPtr<UES1InteractableComponent> Interactable;
};
