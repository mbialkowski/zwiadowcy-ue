#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ZwSettings.generated.h"

class USkeletalMesh;
class UAnimInstance;

/**
 * Ustawienia gry w Config/DefaultGame.ini ([/Script/Zwiadowcy.ZwSettings]).
 * Odwołania do assetów trzymamy tutaj, a nie w Blueprintach - podmiana modelu nie wymaga edytora.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Zwiadowcy"))
class ZWIADOWCY_API UZwSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Model Willa (na razie manekin z szablonu UE). */
	UPROPERTY(Config, EditAnywhere, Category = "Postać")
	TSoftObjectPtr<USkeletalMesh> PlayerMesh;

	/** Klasa animacji Willa. */
	UPROPERTY(Config, EditAnywhere, Category = "Postać")
	TSoftClassPtr<UAnimInstance> PlayerAnimClass;

	/** Czułość myszy: mnożnik ruchu myszy (po czułości osi Mouse2D z DefaultInput.ini). 2.5 = jak w szablonie UE. */
	UPROPERTY(Config, EditAnywhere, Category = "Sterowanie")
	float MouseSensitivity = 2.5f;

	/** Prędkość obrotu kamery na padzie (stopnie na sekundę przy pełnym wychyleniu). */
	UPROPERTY(Config, EditAnywhere, Category = "Sterowanie")
	float GamepadLookSpeed = 160.f;

	static const UZwSettings& Get() { return *GetDefault<UZwSettings>(); }
};
