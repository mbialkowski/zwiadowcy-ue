// Testy w działającej grze (kontekst klienta, czyli UnrealEditor -game). Uruchamianie: Scripts/zw.ps1 test
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Postac/ZwPlayerCharacter.h"
#include "Tests/AutomationCommon.h"

namespace
{
	const TCHAR* PoligonMap = TEXT("/Game/Zwiadowcy/Mapy/L_Poligon");

	/** Czeka, aż Will stanie na ziemi, a potem przez chwilę idzie do przodu i sprawdza, czy się przesunął. */
	class FZwWalkCheck : public IAutomationLatentCommand
	{
	public:
		explicit FZwWalkCheck(FAutomationTestBase* InTest) : Test(InTest) {}

		virtual bool Update() override
		{
			const double Elapsed = FPlatformTime::Seconds() - StartTime;
			UWorld* World = AutomationCommon::GetAnyGameWorld();
			AZwPlayerCharacter* Will = World ? Cast<AZwPlayerCharacter>(UGameplayStatics::GetPlayerPawn(World, 0)) : nullptr;
			if (!Will)
			{
				if (Elapsed > 10.0)
				{
					Test->AddError(TEXT("po 10 s nie ma postaci gracza AZwPlayerCharacter"));
					return true;
				}
				return false;
			}
			UCharacterMovementComponent* Move = Will->GetCharacterMovement();
			if (!bWalking)
			{
				if (Move->IsMovingOnGround())
				{
					bWalking = true;
					WalkStart = FPlatformTime::Seconds();
					StartPos = Will->GetActorLocation();
					Test->TestNotNull(TEXT("postać ma model"), Will->GetMesh()->GetSkeletalMeshAsset());
					Test->TestNotNull(TEXT("postać ma kamerę"), Will->GetFollowCamera());
				}
				else if (Elapsed > 10.0)
				{
					Test->AddError(TEXT("postać nie stanęła na ziemi w 10 s (spada przez podłoże?)"));
					return true;
				}
				return false;
			}
			if (FPlatformTime::Seconds() - WalkStart < 1.5)
			{
				Will->AddMovementInput(FVector::ForwardVector, 1.f);
				return false;
			}
			const double Dist = FVector::Dist2D(StartPos, Will->GetActorLocation());
			Test->TestTrue(FString::Printf(TEXT("postać przeszła %.0f cm w 1,5 s (oczekiwane > 150)"), Dist), Dist > 150.0);
			Test->TestTrue(TEXT("postać nadal na ziemi"), Move->IsMovingOnGround());
			return true;
		}

	private:
		FAutomationTestBase* Test;
		bool bWalking = false;
		double WalkStart = 0.0;
		FVector StartPos = FVector::ZeroVector;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FZwTestPoligon, "Zwiadowcy.Swiat.Poligon",
	EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FZwTestPoligon::RunTest(const FString& Parameters)
{
	AutomationOpenMap(PoligonMap);
	ADD_LATENT_AUTOMATION_COMMAND(FZwWalkCheck(this));
	return true;
}

#endif
