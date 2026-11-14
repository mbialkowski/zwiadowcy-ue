#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ZwPhotoTour.generated.h"

/**
 * Tryb zrzutów jak PhotoTour w modzie: gra uruchomiona z -zwzrzut czeka, aż świat się wczyta, mierzy czas klatki,
 * robi zrzut ekranu i zamyka się. Wyniki: Saved/Zrzuty/<nazwa>.png i Saved/Zrzuty/raport.txt.
 *   -zwzrzut=<nazwa>     nazwa zrzutu (domyślnie "poligon")
 *   -zwrozgrzewka=<s>    ile sekund czekać przed pomiarem (domyślnie 12)
 *   -zwpomiar=<s>        ile sekund mierzyć klatki (domyślnie 8)
 */
UCLASS()
class ZWIADOWCY_API UZwPhotoTour : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	bool Tick(float DeltaTime);
	void WriteReport() const;

	FTSTicker::FDelegateHandle TickHandle;
	FString ShotName = TEXT("poligon");
	double WarmupSeconds = 12.0;
	double MeasureSeconds = 8.0;
	double Elapsed = 0.0;
	int32 Phase = 0;
	TArray<float> FrameTimesMs;
};
