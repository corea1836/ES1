#pragma once

#include "CoreMinimal.h"
#include "Components/ES1CombatComponent.h"
#include "Equipments/ES1Equipment.h"
#include "GameFramework/Character.h"
#include "ES1Character.generated.h"

enum class EES1MovementGate : uint8;
enum class EES1EquipmentType : uint8;
struct FInputActionValue;

UCLASS()
class ES1_API AES1Character : public ACharacter
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	bool bIsInterpCameraBoom = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	float DefaultArmLength = 350.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	float AimArmLength = 200.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	float ArmInterpSpeed = 5.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> SwitchWeaponAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> AimAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> UseAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> ReloadAction;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<UES1CombatComponent> CombatComponent;
	
	float TargetArmLengthGoal = 500.f;
	
public:
	AES1Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	EES1MovementGate MovementGate;
	EES1MovementGate CachedMovementGate;
	
public:
	EES1MovementGate GetMovementGate() const { return MovementGate; }
	EES1EquipmentType GetSelectedEquipmentType() const;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UES1AttributeComponent> AttributeComponent;
	
protected:
	void Move(const FInputActionValue& Values);
	void Look(const FInputActionValue& Values);
	void SwitchWeapon(const FInputActionValue& Values);
	void AimStart(const FInputActionValue& Values);
	void AimComplete(const FInputActionValue& Values);
	void Crouch(const FInputActionValue& Values);
	void Jump(const FInputActionValue& Values);
	void StopJumping(const FInputActionValue& Values);
	void UseEquipment(const FInputActionValue& Values);
	void Reload(const FInputActionValue& Values);
	void UpdateCameraBoom(float DeltaTime);
	
	void OnUseMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void SetMovementGate();
};
