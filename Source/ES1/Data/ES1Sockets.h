
#pragma once

#include "CoreMinimal.h"
#include "ES1Sockets.generated.h"

USTRUCT(BlueprintType)
struct FES1Sockets
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Socket")
	FName PistolUnequippedSocket;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Socket")
	FName RifleUnequippedSocket;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Socket")
	FName WeaponEquippedSocket;
};