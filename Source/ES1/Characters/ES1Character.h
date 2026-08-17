#pragma once

#include "CoreMinimal.h"
#include "Components/ES1CombatComponent.h"
#include "Equipments/ES1Weapon.h"
#include "GameFramework/Character.h"
#include "Interfaces/ES1PlayerInterface.h"
#include "ES1Character.generated.h"

enum class EES1MovementGate : uint8;
enum class EES1EquipmentType : uint8;
struct FInputActionValue;

UCLASS()
class ES1_API AES1Character : public ACharacter, public IES1PlayerInterface
{
	GENERATED_BODY()
	
public:
	AES1Character();
	
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	
	// PlayerInterface 
	virtual FName GetWeaponEquippedSocket_Implementation(const FGameplayTag& WeaponType) const override;
	virtual USkeletalMeshComponent* GetPlayerMesh_Implementation() const override;
	
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
	void LinkAnimLayer();
	void RefreshMovementGate();
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnAim(bool bIsAming);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Camera", meta=(AllowPrivateAccess=true))
	bool bIsInterpCameraBoom = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Camera", meta=(AllowPrivateAccess=true))
	float DefaultArmLength = 350.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Camera", meta=(AllowPrivateAccess=true))
	float AimArmLength = 200.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Camera", meta=(AllowPrivateAccess=true))
	float ArmInterpSpeed = 5.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ES1|Camera|Aming", meta=(AllowPrivateAccess=true))
	float DefaultFieldOfView;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Component", meta=(AllowPrivateAccess=true))
	TObjectPtr<UES1CombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Component", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UES1AttributeComponent> AttributeComponent;
	
	float TargetArmLengthGoal = 500.f;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputMappingContext> ES1IMC;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> SwitchWeaponAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> FireWeaponAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> AimWeaponAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ES1|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> ReloadWeaponAction;
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Crouch();
	void Input_Jump();
	void Input_SwitchWeapon();
	void Input_FireWeapon_Pressed();
	void Input_FireWeapon_Released();
	void Input_Aim_Pressed();
	void Input_Aim_Released();
	void Input_ReloadWeapon();

private:
	EES1MovementGate MovementGate;
	EES1MovementGate CachedMovementGate;
	
	bool bIsFiring = false;
	
public:
	EES1MovementGate GetMovementGate() const { return MovementGate; }
	
protected:
	void Move(const FInputActionValue& Values);
	void Look(const FInputActionValue& Values);
	void SwitchWeapon(const FInputActionValue& Values);
	void AimStart(const FInputActionValue& Values);
	void AimComplete(const FInputActionValue& Values);
	void Crouch(const FInputActionValue& Values);
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	void Jump(const FInputActionValue& Values);
	void StopJumping(const FInputActionValue& Values);
	void UseEquipment(const FInputActionValue& Values);
	void Reload(const FInputActionValue& Values);
	void UpdateCameraBoom(float DeltaTime);
	
	void OnUseMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void SetMovementGate();
};
