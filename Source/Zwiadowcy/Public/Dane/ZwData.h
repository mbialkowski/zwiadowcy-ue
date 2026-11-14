#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "ZwData.generated.h"

/** Miejsce w świecie z Content/Data/lokacje.json. Współrzędne w metrach, (0,0) w środku; +x wschód, +y południe. */
USTRUCT(BlueprintType)
struct ZWIADOWCY_API FZwLocation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FName Id;
	UPROPERTY(BlueprintReadOnly) FString Name;
	UPROPERTY(BlueprintReadOnly) FVector2D PositionM = FVector2D::ZeroVector;
	UPROPERTY(BlueprintReadOnly) float RadiusM = 0.f;

	/** Pozycja w jednostkach UE (cm), oś Y jak w planie (+y = południe). */
	FVector2D PositionCm() const { return PositionM * 100.0; }
};

/**
 * Pliki danych gry (misje, dialogi, lokacje) leżą w Content/Data jako JSON - jak w modzie.
 * W paczce gry katalog jest kopiowany jako zwykłe pliki (DirectoriesToAlwaysStageAsNonUFS).
 */
namespace ZwData
{
	ZWIADOWCY_API FString DataDir();

	/** Wczytuje JSON z Content/Data/<RelativePath>. Błędy idą do Error (gdy podano) i do logu. */
	ZWIADOWCY_API TSharedPtr<FJsonObject> LoadJson(const FString& RelativePath, FString* Error = nullptr);

	ZWIADOWCY_API bool LoadLocations(TArray<FZwLocation>& Out, FString* Error = nullptr);
}
