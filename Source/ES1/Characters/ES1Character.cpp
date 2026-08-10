#include "Characters/ES1Character.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ES1Define.h"
#include "ES1GameplayTags.h"
#include "Animations/ES1AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/ES1AttributeComponent.h"
#include "Components/ES1CombatComponent.h"
#include "Equipments/ES1Equipment.h"
#include "Equipments/ES1Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AES1Character::AES1Character()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 카메라 초기화
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(138.f, 94.f, 40.f);
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<UES1AttributeComponent>(TEXT("AttributeComponent"));
	CombatComponent = CreateDefaultSubobject<UES1CombatComponent>(TEXT("CombatComponent"));
}

void AES1Character::BeginPlay()
{
	Super::BeginPlay();		 
	
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	
	MovementGate = EES1MovementGate::Jogging;
	SetMovementGate();
	
	CombatComponent->SwitchEquipment(EES1SelectedWeaponSlot::Unarmed);
	if (UES1AnimInstance* AnimInstance = Cast<UES1AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		
		TSubclassOf<UAnimInstance> LayerAnimClass = AnimInstance->GetEquippedWeaponLocomotion(GetSelectedEquipmentType());
	
		GetMesh()->LinkAnimClassLayers(LayerAnimClass);
	}	

}

void AES1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	FVector StartPosition = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FVector EndPosition = GetActorLocation() - FVector(0.f, 0.f, 1000.f);
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	
	UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartPosition,
		EndPosition,
		5.0f,
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		);
	
	Cast<UES1AnimInstance>(GetMesh()->GetAnimInstance())->ReceiveGroundDistance(HitResult.Distance);

	UpdateCameraBoom(DeltaTime);
}

void AES1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AES1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		 EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		 EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		 EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &ThisClass::SwitchWeapon);
		 EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::AimStart);
		 EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::AimComplete);
		 EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::Crouch);
		 EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		 EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
		 EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &ThisClass::UseEquipment);
		 EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ThisClass::Reload);
	}
}

EES1EquipmentType AES1Character::GetSelectedEquipmentType() const
{
	AES1Equipment* Equipment = CombatComponent->GetSelectedEquipment();
	if (Equipment)
	{
		return Equipment->GetEquipmentType();
	}
	
	return EES1EquipmentType::UnArmed;
}

void AES1Character::Move(const FInputActionValue& Values)
{
	FVector2D MovementVector = Values.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotator(0, Rotation.Yaw, 0);
		
		const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void AES1Character::Look(const FInputActionValue& Values)
{
	FVector2D LookDirection = Values.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
	}
}

void AES1Character::SwitchWeapon(const FInputActionValue& Values)
{
	int8 InputNumber = static_cast<int8>(Values.Get<float>());
	
	EES1SelectedWeaponSlot SelectedWeaponSlot = static_cast<EES1SelectedWeaponSlot>(InputNumber);
	
	CombatComponent->SwitchEquipment(SelectedWeaponSlot);

	if (UES1AnimInstance* AnimInstance = Cast<UES1AnimInstance>(GetMesh()->GetAnimInstance()))
	{
		TSubclassOf<UAnimInstance> LayerAnimClass = AnimInstance->GetEquippedWeaponLocomotion(GetSelectedEquipmentType());
	
		GetMesh()->LinkAnimClassLayers(LayerAnimClass);
	}

}

void AES1Character::AimStart(const FInputActionValue& Values)
{
	switch (MovementGate)
	{
	case EES1MovementGate::Walking:
		MovementGate = EES1MovementGate::Walking;
		break;
	case EES1MovementGate::Jogging:
		MovementGate = EES1MovementGate::Walking;
		break;
	case EES1MovementGate::Crouching:
		MovementGate = EES1MovementGate::Crouching;
		break;
	}
	
	
	TargetArmLengthGoal = AimArmLength;
	bIsInterpCameraBoom = true;
	SetMovementGate();
}

void AES1Character::AimComplete(const FInputActionValue& Values)
{
	switch (MovementGate)
	{
	case EES1MovementGate::Walking:
		MovementGate = EES1MovementGate::Jogging;
		break;
	case EES1MovementGate::Jogging:
		MovementGate = EES1MovementGate::Jogging;
		break;
	case EES1MovementGate::Crouching:
		MovementGate = EES1MovementGate::Crouching;
		break;
	}
	
	TargetArmLengthGoal = DefaultArmLength;
	bIsInterpCameraBoom = true;
	SetMovementGate();
}

void AES1Character::Crouch(const FInputActionValue& Values)
{
	EES1MovementGate currentGate = MovementGate;
	if (currentGate != EES1MovementGate::Crouching)
	{
		MovementGate = EES1MovementGate::Crouching;
		Super::Crouch(false);
	}
	else
	{
		MovementGate = EES1MovementGate::Jogging;
		Super::UnCrouch(false);
	}
	SetMovementGate();
}

void AES1Character::Jump(const FInputActionValue& Values)
{
	Super::Jump();
}

void AES1Character::StopJumping(const FInputActionValue& Values)
{
	Super::StopJumping();
}

void AES1Character::UseEquipment(const FInputActionValue& Values)
{	
	CachedMovementGate = MovementGate;
	
	if (MovementGate == EES1MovementGate::Jogging)
	{
		MovementGate = EES1MovementGate::Walking;
		SetMovementGate();
	}
	
	if (CombatComponent)
	{
		if (CombatComponent->UseSelectedEquipment())
		{
			UAnimMontage* Montage = CombatComponent->GetSelectedEquipmentMontage(ES1GameplayTags::Character_Action_Fire);
			
			if (Montage)
			{
				float Duration = PlayAnimMontage(Montage);
				
				if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
				{
					FOnMontageEnded EndDelegate;
					EndDelegate.BindUObject(this, &ThisClass::OnUseMontageEnded);
					AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
				}
			}
		}		
	}	
}

void AES1Character::Reload(const FInputActionValue& Values)
{
	if (CombatComponent)
	{
		UAnimMontage* Montage = CombatComponent->GetSelectedEquipmentMontage(ES1GameplayTags::Character_Action_Reload);
		if (Montage)
		{
			PlayAnimMontage(Montage);
			AES1Weapon* Weapon= Cast<AES1Weapon>(CombatComponent->GetSelectedEquipment());
			if (Weapon)
			{
				Weapon->Reload();
			}
		}
	}
}

void AES1Character::UpdateCameraBoom(float DeltaTime)
{
	if (!bIsInterpCameraBoom || !CameraBoom) return;
	
	const float Current = CameraBoom->TargetArmLength;
	const float New = FMath::FInterpTo(Current, TargetArmLengthGoal, DeltaTime, ArmInterpSpeed);
	CameraBoom->TargetArmLength = New;
	
	if (FMath::IsNearlyEqual(New, TargetArmLengthGoal, 0.5f))
	{
		CameraBoom->TargetArmLength = TargetArmLengthGoal;
		bIsInterpCameraBoom = false;
	}
}

void AES1Character::OnUseMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	MovementGate = CachedMovementGate;
	SetMovementGate();
}

void AES1Character::SetMovementGate()
{
	if (UCharacterMovementComponent* movementComponent = GetCharacterMovement())
	{
		movementComponent->MaxWalkSpeed = AttributeComponent->GetMaxWalkSpeed(MovementGate);
		movementComponent->MaxAcceleration = AttributeComponent->GetMaxAcceleration(MovementGate);
		movementComponent->BrakingDecelerationWalking = AttributeComponent->GetBrakingDeceleration(MovementGate);
		movementComponent->BrakingFrictionFactor = AttributeComponent->GetBrakingFrictionFactor(MovementGate);
		movementComponent->BrakingFriction = AttributeComponent->GetBrakingFriction(MovementGate);
		movementComponent->bUseSeparateBrakingFriction = AttributeComponent->GetbUseSeperateBrakingFriction(MovementGate);
	}
}
