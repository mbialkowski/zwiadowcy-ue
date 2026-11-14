#include "Postac/ZwPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Postac/ZwInputSet.h"
#include "Postac/ZwPlayerController.h"
#include "Zwiadowcy.h"
#include "ZwSettings.h"

AZwPlayerCharacter::AZwPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(38.f, 92.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->bOrientRotationToMovement = true;
	MoveComp->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	MoveComp->JumpZVelocity = 420.f;
	MoveComp->AirControl = 0.3f;
	MoveComp->MaxWalkSpeed = WalkSpeed;
	MoveComp->MaxWalkSpeedCrouched = CrouchSpeed;
	MoveComp->MinAnalogWalkSpeed = 20.f;
	MoveComp->BrakingDecelerationWalking = 2000.f;
	MoveComp->BrakingDecelerationFalling = 1500.0f;
	MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;

	// kamera za plecami, lekko nad prawym ramieniem (jak w soulslike'ach)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 380.0f;
	CameraBoom->SocketOffset = FVector(0.f, 45.f, 55.f);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 12.f;
	CameraBoom->ProbeSize = 14.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetFieldOfView(80.f);

	// manekin stoi stopami na dole kapsuły i patrzy wzdłuż +X
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -92.f), FRotator(0.f, -90.f, 0.f));
}

void AZwPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	ApplyMeshFromSettings();
	UpdateSpeed();
}

void AZwPlayerCharacter::ApplyMeshFromSettings()
{
	const UZwSettings& Cfg = UZwSettings::Get();
	if (USkeletalMesh* MeshAsset = Cfg.PlayerMesh.LoadSynchronous())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset);
	}
	else
	{
		UE_LOG(LogZwiadowcy, Warning, TEXT("Brak modelu postaci %s - uruchom Scripts/zw.ps1 setup"), *Cfg.PlayerMesh.ToString());
	}
	if (UClass* Anim = Cfg.PlayerAnimClass.LoadSynchronous())
	{
		GetMesh()->SetAnimInstanceClass(Anim);
	}
}

void AZwPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AZwPlayerController* PC = Cast<AZwPlayerController>(GetController());
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!PC || !Input)
	{
		UE_LOG(LogZwiadowcy, Error, TEXT("Postać wymaga AZwPlayerController i Enhanced Input"));
		return;
	}
	const UZwInputSet* Set = PC->GetInputSet();
	Input->BindAction(Set->Move, ETriggerEvent::Triggered, this, &AZwPlayerCharacter::Move);
	Input->BindAction(Set->Look, ETriggerEvent::Triggered, this, &AZwPlayerCharacter::Look);
	Input->BindAction(Set->Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
	Input->BindAction(Set->Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	Input->BindAction(Set->Sprint, ETriggerEvent::Started, this, &AZwPlayerCharacter::SprintStart);
	Input->BindAction(Set->Sprint, ETriggerEvent::Completed, this, &AZwPlayerCharacter::SprintStop);
	Input->BindAction(Set->Crouch, ETriggerEvent::Started, this, &AZwPlayerCharacter::ToggleCrouch);
}

void AZwPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D V = Value.Get<FVector2D>();
	if (!Controller)
	{
		return;
	}
	const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), V.Y);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), V.X);
}

void AZwPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D V = Value.Get<FVector2D>();
	AddControllerYawInput(V.X);
	// bez starych skal wejścia dodatni pitch = patrzenie w górę
	AddControllerPitchInput(V.Y);
}

void AZwPlayerCharacter::SprintStart(const FInputActionValue&)
{
	bSprinting = true;
	if (bIsCrouched)
	{
		UnCrouch();
	}
	UpdateSpeed();
}

void AZwPlayerCharacter::SprintStop(const FInputActionValue&)
{
	bSprinting = false;
	UpdateSpeed();
}

void AZwPlayerCharacter::ToggleCrouch(const FInputActionValue&)
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		bSprinting = false;
		Crouch();
	}
	UpdateSpeed();
}

void AZwPlayerCharacter::UpdateSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
}
