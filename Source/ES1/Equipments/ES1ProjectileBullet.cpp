#include "ES1ProjectileBullet.h"

#include "Characters/ES1Character.h"
#include "Kismet/GameplayStatics.h"
#include "Players/ES1PlayerController.h"


void AES1ProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{

	if (!bCosmetic)
	{
		if (AES1Character* OwnerCharacter = Cast<AES1Character>(GetOwner()))
		{
			if (AController* OwnerController = OwnerCharacter->GetController())
			{
				UE_LOG(LogTemp, Warning, TEXT("Server Hit"))
				UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
			}
		}
	}
	
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
