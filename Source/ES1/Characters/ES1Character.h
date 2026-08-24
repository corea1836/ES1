#pragma once

#include "CoreMinimal.h"
#include "Components/ES1CombatComponent.h"
#include "Equipments/ES1Weapon.h"
#include "GameFramework/Character.h"
#include "Interfaces/ES1PlayerInterface.h"
#include "ES1Character.generated.h"

class UES1Overlay;
enum class EES1MovementGate : uint8;
enum class EES1EquipmentType : uint8;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponFirstReplicated, AES1Weapon*, Weapon);

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
	virtual void OnRep_PlayerState() override;
	
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
	void LinkAnimLayer();
	void RefreshMovementGate();
	
	bool HasWeaponFirstReplicated() const { return bWeaponFirstReplicated; }
	
	// PlayerInterface 
	virtual FName GetWeaponEquippedSocket_Implementation(const FGameplayTag& WeaponType) const override;
	virtual USkeletalMeshComponent* GetPlayerMesh_Implementation() const override;
	virtual void WeaponReplicated_Implementation() override;
	virtual AES1Weapon* GetCurrentWeapon_Implementation() override;
	virtual int32 GetReserveAmmo_Implementation() const override;
	
	// Variables
	UPROPERTY(BlueprintAssignable)
	FWeaponFirstReplicated OnWeaponFirstReplicated;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnAim(bool bIsAiming);
	
private:	
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
	
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	
	// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ES1|Camera|Aming", meta=(AllowPrivateAccess=true))
	float DefaultFieldOfView;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Component", meta=(AllowPrivateAccess=true))
	TObjectPtr<UES1CombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ES1|Component", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UES1AttributeComponent> AttributeComponent;
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|UI", meta=(AllowPrivateAccess=true))
	TSubclassOf<UUserWidget> PlayerOverlayWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ES1|UI", meta=(AllowPrivateAccess=true))
	TObjectPtr<UES1Overlay> PlayerOverlayWidget;
	
	bool bWeaponFirstReplicated;
};
