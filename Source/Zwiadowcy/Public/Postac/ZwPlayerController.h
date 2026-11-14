#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZwPlayerController.generated.h"

class UZwInputSet;

UCLASS()
class ZWIADOWCY_API AZwPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Zestaw akcji, tworzony przy pierwszym użyciu (postać może o niego poprosić przed BeginPlay kontrolera). */
	UZwInputSet* GetInputSet();

protected:
	virtual void BeginPlay() override;

private:
	void AddMappingContext();

	UPROPERTY() TObjectPtr<UZwInputSet> InputSet;
};
