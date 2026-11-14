#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZwPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

/**
 * Will - postać gracza w trzeciej osobie. E0: ruch, kamera, bieg, kucanie, skok.
 * Łuk, skradanie i walka dochodzą w E1 jako osobne komponenty.
 */
UCLASS()
class ZWIADOWCY_API AZwPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZwPlayerCharacter();

	UFUNCTION(BlueprintPure, Category = "Zwiadowcy")
	bool IsSprinting() const { return bSprinting; }

	/** Prędkości ruchu (cm/s). */
	UPROPERTY(EditDefaultsOnly, Category = "Ruch") float WalkSpeed = 450.f;
	UPROPERTY(EditDefaultsOnly, Category = "Ruch") float SprintSpeed = 700.f;
	UPROPERTY(EditDefaultsOnly, Category = "Ruch") float CrouchSpeed = 220.f;

	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SprintStart(const FInputActionValue& Value);
	void SprintStop(const FInputActionValue& Value);
	void ToggleCrouch(const FInputActionValue& Value);
	void ApplyMeshFromSettings();
	void UpdateSpeed();

	UPROPERTY(VisibleAnywhere, Category = "Kamera") TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = "Kamera") TObjectPtr<UCameraComponent> FollowCamera;

	bool bSprinting = false;
};
