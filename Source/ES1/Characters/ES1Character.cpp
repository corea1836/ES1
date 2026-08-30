#include "Characters/ES1Character.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ES1Define.h"
#include "ES1GameplayTags.h"
#include "Animations/ES1AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/ES1AttributeComponent.h"
#include "Components/ES1CombatComponent.h"
#include "Components/ES1HealthComponent.h"
#include "Data/ES1WeaponData.h"
#include "Equipments/ES1Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tags/ES1WeaponTags.h"
#include "Equipments/ES1Weapon.h"
#include "Players/ES1PlayerController.h"
#include "UI/ES1Overlay.h"

AES1Character::AES1Character()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	// 카메라 초기화
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->SocketOffset = FVector(138.f, 94.f, 40.f);
	SpringArm->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<UES1AttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent->SetIsReplicated(true);
	
	CombatComponent = CreateDefaultSubobject<UES1CombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);
	
	HealthComponent = CreateDefaultSubobject<UES1HealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);
	
	DefaultFieldOfView = 65.f;
	
	bWeaponFirstReplicated = false;
}


void AES1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	FVector StartPosition = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FVector EndPosition = GetActorLocation() - FVector(0.f, 0.f, 1000.f);
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	//
	// UKismetSystemLibrary::SphereTraceSingle(
	// 	GetWorld(),
	// 	StartPosition,
	// 	EndPosition,
	// 	5.0f,
	// 	TraceChannel,
	// 	false,
	// 	ActorsToIgnore,
	// 	EDrawDebugTrace::ForDuration,
	// 	HitResult,
	// 	true
	// 	);
	//
	// Cast<UES1AnimInstance>(GetPlayerMesh()->GetAnimInstance())->ReceiveGroundDistance(HitResult.Distance);

}

void AES1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(ES1IMC, 0);
		}
	}
}

void AES1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::Input_Crouch);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Input_Jump);
	EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_SwitchWeapon);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
	EnhancedInputComponent->BindAction(FireWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_FireWeapon_Pressed);
	EnhancedInputComponent->BindAction(FireWeaponAction, ETriggerEvent::Completed, this, &ThisClass::Input_FireWeapon_Released);
	EnhancedInputComponent->BindAction(AimWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_Aim_Pressed);
	EnhancedInputComponent->BindAction(AimWeaponAction, ETriggerEvent::Completed, this, &ThisClass::Input_Aim_Released);
	EnhancedInputComponent->BindAction(ReloadWeaponAction, ETriggerEvent::Started, this, &ThisClass::Input_ReloadWeapon);
}

FName AES1Character::GetWeaponEquippedSocket_Implementation(const FGameplayTag& WeaponType) const
{
	checkf(CombatComponent->WeaponData, TEXT("No Weapon Data Asset - Please fill out BP_ES1Character"));
	return CombatComponent->WeaponData->EquippedSocketData.FindChecked(WeaponType);
}

USkeletalMeshComponent* AES1Character::GetPlayerMesh_Implementation() const
{
	return ACharacter::GetMesh();
}

void AES1Character::WeaponReplicated_Implementation()
{
	if (!bWeaponFirstReplicated)
	{
		bWeaponFirstReplicated = true;
		OnWeaponFirstReplicated.Broadcast(CombatComponent->GetCurrentWeapon());
	}
}

AES1Weapon* AES1Character::GetCurrentWeapon_Implementation()
{
	return CombatComponent->GetCurrentWeapon();
}

int32 AES1Character::GetReserveAmmo_Implementation() const
{
	return CombatComponent->CurrentReserveAmmo;
}

void AES1Character::Notify_CycleWeapon_Implementation()
{
	CombatComponent->Notify_CycleWeapon();
}

void AES1Character::Notify_ReloadWeapon_Implementation()
{
	CombatComponent->Notify_ReloadWeapon();
}

void AES1Character::AddAmmo_Implementation(const FGameplayTag& WeaponType, int32 AmmoAmount)
{
	if (HasAuthority() && IsValid(CombatComponent))
	{
		CombatComponent->AddAmmo(WeaponType, AmmoAmount);
	}
}

bool AES1Character::DoDamage_Implementation(float DamageAmount, AActor* DamageInstigator)
{
	if (!IsValid(HealthComponent)) return false;
	
	HealthComponent->ChangeHealthByAmount(-DamageAmount, DamageInstigator);
	
	
	
	const int32 MontageSelection = FMath::RandRange(0, HitReacts.Num() - 1);
	Multicast_HitReact(MontageSelection);
		
	return false;
}

void AES1Character::BeginPlay()
{
	Super::BeginPlay();		 
	
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	FollowCamera->SetFieldOfView(DefaultFieldOfView);
	AttributeComponent->SwitchGate(EES1MovementGate::Jogging);
	
	if (PlayerOverlayWidgetClass)
	{
		PlayerOverlayWidget = CreateWidget<UES1Overlay>(GetWorld(), PlayerOverlayWidgetClass);
		if (PlayerOverlayWidget)
		{
			PlayerOverlayWidget->AddToViewport();
		}
	}
	
	bPawnAlive = true;

}

void AES1Character::BeginDestroy()
{
	Super::BeginDestroy();
	
	if  (IsValid(CombatComponent))
	{
		CombatComponent->DestroyInventory();
	}
}

void AES1Character::LinkAnimLayer()
{
	TSubclassOf<UAnimInstance> AnimLayerClass = CombatComponent->GetCurrentWeaponAnimLayer();
	if (!IsValid(AnimLayerClass)) return;
	
	Execute_GetPlayerMesh(this)->LinkAnimClassLayers(AnimLayerClass);
}

void AES1Character::RefreshMovementGate()
{
	EES1MovementGate Gate;
	if (GetCharacterMovement()->IsCrouching()) Gate = EES1MovementGate::Crouching;
	else if (CombatComponent->GetIsAiming()) Gate = EES1MovementGate::Walking;
	else if (CombatComponent->GetIsAiming()) Gate = EES1MovementGate::Walking;
	else Gate = EES1MovementGate::Jogging;
	
	AttributeComponent->SwitchGate(Gate);
}

void AES1Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (IsValid(CombatComponent))
	{
		CombatComponent->SpawnInventory();
		LinkAnimLayer();
	}
}

void AES1Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// if (IsValid(CombatComponent))																																																											`b
	// {
	// 	CombatComponent->InitializeWeaponWidgets();
	// }
}

void AES1Character::Multicast_HitReact_Implementation(int32 MontageIndex)
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		if (HitReacts.IsValidIndex(MontageIndex))
		{
			GetMesh()->GetAnimInstance()->Montage_Play(HitReacts[MontageIndex]);
		}
	}
}

void AES1Character::OnDeathStarted()
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		DeathEffects();
		if (AES1PlayerController* PC = Cast<AES1PlayerController>(GetController()); IsValid(PC))
		{
			DisableInput(PC);
			if (PC->IsLocalController()) 
				bPawnAlive = false;
		}
	}
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ES1TraceChannel::ECC_Weapon, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ES1TraceChannel::ECC_Weapon, ECollisionResponse::ECR_Ignore);
}

void AES1Character::Input_Move(const FInputActionValue& InputActionValue)
{
	if (!bPawnAlive) return;
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, InputAxisVector.Y);
	AddMovementInput(RightDirection, InputAxisVector.X);
}

void AES1Character::Input_Look(const FInputActionValue& InputActionValue)
{
	if (!bPawnAlive) return;
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	
	if (IsValid(Controller))
	{
		AddControllerYawInput(InputAxisVector.X);
		AddControllerPitchInput(InputAxisVector.Y);
	}
}

void AES1Character::Input_Crouch()
{
	if (!bPawnAlive) return;
	if (UCharacterMovementComponent* CMC = GetCharacterMovement(); IsValid(CMC))
	{
		CMC->bWantsToCrouch = !CMC->bWantsToCrouch;
	}
}

void AES1Character::Input_Jump()
{
	if (!bPawnAlive) return;
	UCharacterMovementComponent* CMC = GetCharacterMovement();
	
	if (!IsValid(CMC)) return;
	
	if (CMC->bWantsToCrouch)
	{
		CMC->bWantsToCrouch = false;
	}
	else
	{
		Super::Jump();
	}
}

void AES1Character::Input_SwitchWeapon()
{
	if (!bPawnAlive) return;
	CombatComponent->Initiate_SwitchWeapon();
}

void AES1Character::Input_FireWeapon_Pressed()
{
	if (!bPawnAlive) return;
	CombatComponent->Initiate_FireWeapon_Pressed();
}

void AES1Character::Input_FireWeapon_Released()
{
	if (!bPawnAlive) return;
	CombatComponent->Initiate_FireWeapon_Released();
}

void AES1Character::Input_Aim_Pressed()
{
	if (!bPawnAlive) return;
	CombatComponent->Initiate_Aim_Pressed();
	OnAim(true);
}

void AES1Character::Input_Aim_Released()
{
	if (!bPawnAlive) return;
	CombatComponent->Initiate_Aim_Released();
	OnAim(false);
}

void AES1Character::Input_ReloadWeapon()
{
	if (!bPawnAlive) return;
	CombatComponent->Initiate_ReloadWeapon();
}

void AES1Character::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	RefreshMovementGate();
}

void AES1Character::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	RefreshMovementGate();
}

