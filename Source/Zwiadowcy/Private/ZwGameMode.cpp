#include "ZwGameMode.h"

#include "Postac/ZwPlayerCharacter.h"
#include "Postac/ZwPlayerController.h"

AZwGameMode::AZwGameMode()
{
	DefaultPawnClass = AZwPlayerCharacter::StaticClass();
	PlayerControllerClass = AZwPlayerController::StaticClass();
}
