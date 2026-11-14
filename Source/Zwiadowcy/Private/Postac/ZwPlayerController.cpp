#include "Postac/ZwPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Postac/ZwInputSet.h"

UZwInputSet* AZwPlayerController::GetInputSet()
{
	if (!InputSet)
	{
		InputSet = UZwInputSet::Create(this);
		AddMappingContext();
	}
	return InputSet;
}

void AZwPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GetInputSet();
	AddMappingContext();
}

void AZwPlayerController::AddMappingContext()
{
	if (!InputSet || !IsLocalPlayerController())
	{
		return;
	}
	if (UEnhancedInputLocalPlayerSubsystem* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (!Sub->HasMappingContext(InputSet->Context))
		{
			Sub->AddMappingContext(InputSet->Context, 0);
		}
	}
}
