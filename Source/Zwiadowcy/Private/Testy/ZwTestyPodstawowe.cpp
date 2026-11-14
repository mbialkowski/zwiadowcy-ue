// Testy automatyczne bez świata: dane i sterowanie. Uruchamianie: Scripts/zw.ps1 test
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Dane/ZwData.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Postac/ZwInputSet.h"
#include "ZwSettings.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FZwTestLokacje, "Zwiadowcy.Dane.Lokacje",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FZwTestLokacje::RunTest(const FString& Parameters)
{
	TArray<FZwLocation> Locations;
	FString Error;
	if (!TestTrue(TEXT("lokacje.json wczytany: ") + Error, ZwData::LoadLocations(Locations, &Error)))
	{
		return false;
	}
	TestTrue(TEXT("co najmniej 8 miejsc"), Locations.Num() >= 8);

	TSet<FName> Ids;
	for (const FZwLocation& L : Locations)
	{
		TestFalse(FString::Printf(TEXT("powtórzone id %s"), *L.Id.ToString()), Ids.Contains(L.Id));
		Ids.Add(L.Id);
		TestFalse(FString::Printf(TEXT("%s ma nazwę"), *L.Id.ToString()), L.Name.IsEmpty());
		// świat ma 2x2 km z (0,0) w środku
		TestTrue(FString::Printf(TEXT("%s w granicach świata"), *L.Id.ToString()),
			FMath::Abs(L.PositionM.X) <= 1000.0 && FMath::Abs(L.PositionM.Y) <= 1000.0);
	}
	for (const TCHAR* Needed : { TEXT("redmont"), TEXT("chata_halta"), TEXT("ruiny_gorlanu") })
	{
		TestTrue(FString::Printf(TEXT("jest miejsce %s"), Needed), Ids.Contains(FName(Needed)));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FZwTestSterowanie, "Zwiadowcy.Postac.Sterowanie",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FZwTestSterowanie::RunTest(const FString& Parameters)
{
	UZwInputSet* Set = UZwInputSet::Create(GetTransientPackage());
	TestNotNull(TEXT("kontekst mapowania"), Set->Context.Get());

	TMap<const UInputAction*, TSet<FKey>> KeysPerAction;
	for (const FEnhancedActionKeyMapping& M : Set->Context->GetMappings())
	{
		KeysPerAction.FindOrAdd(M.Action).Add(M.Key);
	}
	for (UInputAction* A : Set->AllActions())
	{
		if (!TestNotNull(TEXT("akcja istnieje"), A))
		{
			continue;
		}
		const TSet<FKey>* Keys = KeysPerAction.Find(A);
		bool bGamepad = false;
		bool bKeyboardMouse = false;
		if (Keys)
		{
			for (const FKey& K : *Keys)
			{
				(K.IsGamepadKey() ? bGamepad : bKeyboardMouse) = true;
			}
		}
		// pad jest pierwszy, ale mysz i klawiatura też muszą działać
		TestTrue(FString::Printf(TEXT("%s ma klawisz na padzie"), *A->GetName()), bGamepad);
		TestTrue(FString::Printf(TEXT("%s ma klawisz na klawiaturze/myszy"), *A->GetName()), bKeyboardMouse);
	}
	TestEqual(TEXT("ruch to oś 2D"), Set->Move->ValueType, EInputActionValueType::Axis2D);
	TestEqual(TEXT("kamera to oś 2D"), Set->Look->ValueType, EInputActionValueType::Axis2D);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FZwTestUstawienia, "Zwiadowcy.Postac.Ustawienia",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EngineFilter)

bool FZwTestUstawienia::RunTest(const FString& Parameters)
{
	const UZwSettings& Cfg = UZwSettings::Get();
	TestFalse(TEXT("model postaci ustawiony w DefaultGame.ini"), Cfg.PlayerMesh.IsNull());
	TestFalse(TEXT("animacje postaci ustawione w DefaultGame.ini"), Cfg.PlayerAnimClass.IsNull());
	TestNotNull(TEXT("model postaci wczytuje się (Scripts/zw.ps1 setup)"), Cfg.PlayerMesh.LoadSynchronous());
	TestNotNull(TEXT("animacje postaci wczytują się"), Cfg.PlayerAnimClass.LoadSynchronous());
	return true;
}

#endif
