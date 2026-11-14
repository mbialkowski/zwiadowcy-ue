#include "Narzedzia/ZwPhotoTour.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "HAL/PlatformMisc.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/CommandLine.h"
#include "Misc/DateTime.h"
#include "Misc/FileHelper.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "RHI.h"
#include "UnrealClient.h"
#include "Zwiadowcy.h"

namespace
{
	FString ShotDir() { return FPaths::ProjectSavedDir() / TEXT("Zrzuty"); }
}

bool UZwPhotoTour::ShouldCreateSubsystem(UObject* Outer) const
{
	FString Name;
	return FParse::Param(FCommandLine::Get(), TEXT("zwzrzut")) || FParse::Value(FCommandLine::Get(), TEXT("zwzrzut="), Name);
}

void UZwPhotoTour::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FParse::Value(FCommandLine::Get(), TEXT("zwzrzut="), ShotName);
	FParse::Value(FCommandLine::Get(), TEXT("zwrozgrzewka="), WarmupSeconds);
	FParse::Value(FCommandLine::Get(), TEXT("zwpomiar="), MeasureSeconds);
	UE_LOG(LogZwiadowcy, Display, TEXT("Tryb zrzutów: %s (rozgrzewka %.0f s, pomiar %.0f s)"), *ShotName, WarmupSeconds, MeasureSeconds);
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UZwPhotoTour::Tick));
}

void UZwPhotoTour::Deinitialize()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
	Super::Deinitialize();
}

bool UZwPhotoTour::Tick(float DeltaTime)
{
	Elapsed += DeltaTime;
	switch (Phase)
	{
	case 0: // rozgrzewka: shadery, strumieniowanie tekstur, Lumen
		if (Elapsed >= WarmupSeconds)
		{
			Phase = 1;
			Elapsed = 0.0;
		}
		break;
	case 1: // pomiar czasu klatki
		FrameTimesMs.Add(DeltaTime * 1000.f);
		if (Elapsed >= MeasureSeconds)
		{
			IFileManager::Get().MakeDirectory(*ShotDir(), true);
			FScreenshotRequest::RequestScreenshot(ShotDir() / (ShotName + TEXT(".png")), false, false);
			WriteReport();
			Phase = 2;
			Elapsed = 0.0;
		}
		break;
	case 2: // chwila na zapis zrzutu i wyjście
		if (Elapsed >= 2.0)
		{
			Phase = 3;
			UE_LOG(LogZwiadowcy, Display, TEXT("Tryb zrzutów: koniec"));
			FPlatformMisc::RequestExit(false, TEXT("ZwPhotoTour"));
		}
		break;
	default:
		break;
	}
	return true;
}

void UZwPhotoTour::WriteReport() const
{
	if (FrameTimesMs.Num() == 0)
	{
		return;
	}
	TArray<float> Sorted = FrameTimesMs;
	Sorted.Sort();
	double Sum = 0.0;
	for (float Ms : FrameTimesMs)
	{
		Sum += Ms;
	}
	const double AvgMs = Sum / FrameTimesMs.Num();
	// 1% najgorszych klatek
	const int32 Worst = FMath::Max(1, FrameTimesMs.Num() / 100);
	double WorstSum = 0.0;
	for (int32 i = Sorted.Num() - Worst; i < Sorted.Num(); ++i)
	{
		WorstSum += Sorted[i];
	}
	const double LowMs = WorstSum / Worst;

	FIntPoint Res(0, 0);
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->Viewport)
	{
		Res = GEngine->GameViewport->Viewport->GetSizeXY();
	}
	const FString Line = FString::Printf(TEXT("%s\t%s\tGPU: %s\t%dx%d\tśrednio %.1f kl./s (%.2f ms)\t1%% najgorszych %.1f kl./s\tklatek %d\n"),
		*FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M")), *ShotName, *GRHIAdapterName, Res.X, Res.Y,
		1000.0 / AvgMs, AvgMs, 1000.0 / LowMs, FrameTimesMs.Num());
	UE_LOG(LogZwiadowcy, Display, TEXT("ZW-POMIAR %s"), *Line.TrimEnd());
	FFileHelper::SaveStringToFile(Line, *(ShotDir() / TEXT("raport.txt")), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM,
		&IFileManager::Get(), FILEWRITE_Append);
}
