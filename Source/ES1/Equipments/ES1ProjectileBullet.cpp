#include "ES1ProjectileBullet.h"

#include "Characters/ES1Character.h"
#include "Kismet/GameplayStatics.h"
#include "Players/ES1PlayerController.h"


void AES1ProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{

	AES1Character* OwnerCharacter = Cast<AES1Character>(GetOwner());
	if (!IsValid(OwnerCharacter)) return;

	AController* OwnerController = OwnerCharacter->GetController();
	if (!IsValid(OwnerController)) return;

	UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
	
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
