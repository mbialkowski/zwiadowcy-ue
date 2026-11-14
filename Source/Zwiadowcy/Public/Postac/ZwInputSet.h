#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ZwInputSet.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * Akcje i mapowanie klawiszy budowane w kodzie (bez assetów w edytorze).
 * Pad jest pierwszy, ale każda akcja ma też klawisz na klawiaturze i myszy.
 */
UCLASS()
class ZWIADOWCY_API UZwInputSet : public UObject
{
	GENERATED_BODY()

public:
	static UZwInputSet* Create(UObject* Outer);

	UPROPERTY() TObjectPtr<UInputMappingContext> Context;

	UPROPERTY() TObjectPtr<UInputAction> Move;    // Axis2D: X w prawo, Y do przodu
	UPROPERTY() TObjectPtr<UInputAction> Look;    // Axis2D: X obrót, Y w górę
	UPROPERTY() TObjectPtr<UInputAction> Jump;
	UPROPERTY() TObjectPtr<UInputAction> Sprint;
	UPROPERTY() TObjectPtr<UInputAction> Crouch;

	/** Wszystkie akcje - do testów i podpowiedzi w interfejsie. */
	TArray<UInputAction*> AllActions() const;

private:
	UInputAction* NewAction(FName Name, bool bAxis2D);
};
