#include "Postac/ZwInputSet.h"

#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "ZwSettings.h"

namespace
{
	template <typename T>
	T* AddModifier(UObject* Outer, FEnhancedActionKeyMapping& Mapping)
	{
		T* Mod = NewObject<T>(Outer);
		Mapping.Modifiers.Add(Mod);
		return Mod;
	}

	/** Klawisz kierunku na osi 2D: W/S idą na Y (swizzle), S i A są odwrócone. */
	void MapDirection(UObject* Outer, UInputMappingContext* Ctx, UInputAction* Action, FKey Key, bool bToY, bool bNegate)
	{
		FEnhancedActionKeyMapping& M = Ctx->MapKey(Action, Key);
		if (bToY)
		{
			AddModifier<UInputModifierSwizzleAxis>(Outer, M)->Order = EInputAxisSwizzle::YXZ;
		}
		if (bNegate)
		{
			AddModifier<UInputModifierNegate>(Outer, M);
		}
	}

	void MapStick(UObject* Outer, UInputMappingContext* Ctx, UInputAction* Action, FKey Key)
	{
		FEnhancedActionKeyMapping& M = Ctx->MapKey(Action, Key);
		UInputModifierDeadZone* Dz = AddModifier<UInputModifierDeadZone>(Outer, M);
		Dz->LowerThreshold = 0.2f;
		Dz->Type = EDeadZoneType::Radial;
	}
}

UInputAction* UZwInputSet::NewAction(FName Name, bool bAxis2D)
{
	UInputAction* A = NewObject<UInputAction>(this, Name);
	A->ValueType = bAxis2D ? EInputActionValueType::Axis2D : EInputActionValueType::Boolean;
	return A;
}

UZwInputSet* UZwInputSet::Create(UObject* Outer)
{
	UZwInputSet* S = NewObject<UZwInputSet>(Outer, TEXT("ZwInputSet"));
	const UZwSettings& Cfg = UZwSettings::Get();

	S->Move = S->NewAction(TEXT("IA_Ruch"), true);
	S->Look = S->NewAction(TEXT("IA_Rozgladanie"), true);
	S->Jump = S->NewAction(TEXT("IA_Skok"), false);
	S->Sprint = S->NewAction(TEXT("IA_Sprint"), false);
	S->Crouch = S->NewAction(TEXT("IA_Kucanie"), false);

	UInputMappingContext* Ctx = NewObject<UInputMappingContext>(S, TEXT("IMC_Zwiadowca"));
	S->Context = Ctx;

	// ruch
	MapDirection(S, Ctx, S->Move, EKeys::W, true, false);
	MapDirection(S, Ctx, S->Move, EKeys::S, true, true);
	MapDirection(S, Ctx, S->Move, EKeys::D, false, false);
	MapDirection(S, Ctx, S->Move, EKeys::A, false, true);
	MapStick(S, Ctx, S->Move, EKeys::Gamepad_Left2D);

	// kamera: mysz w stopniach na jednostkę, pad w stopniach na sekundę
	{
		FEnhancedActionKeyMapping& M = Ctx->MapKey(S->Look, EKeys::Mouse2D);
		AddModifier<UInputModifierScalar>(S, M)->Scalar = FVector(Cfg.MouseSensitivity, Cfg.MouseSensitivity, 1.0);
	}
	{
		FEnhancedActionKeyMapping& M = Ctx->MapKey(S->Look, EKeys::Gamepad_Right2D);
		UInputModifierDeadZone* Dz = AddModifier<UInputModifierDeadZone>(S, M);
		Dz->LowerThreshold = 0.15f;
		Dz->Type = EDeadZoneType::Radial;
		AddModifier<UInputModifierResponseCurveExponential>(S, M)->CurveExponent = FVector(2.0, 2.0, 1.0);
		AddModifier<UInputModifierScaleByDeltaTime>(S, M);
		AddModifier<UInputModifierScalar>(S, M)->Scalar = FVector(Cfg.GamepadLookSpeed, Cfg.GamepadLookSpeed * 0.7, 1.0);
	}

	// akcje
	Ctx->MapKey(S->Jump, EKeys::SpaceBar);
	Ctx->MapKey(S->Jump, EKeys::Gamepad_FaceButton_Bottom);
	Ctx->MapKey(S->Sprint, EKeys::LeftShift);
	Ctx->MapKey(S->Sprint, EKeys::Gamepad_LeftThumbstick);
	Ctx->MapKey(S->Crouch, EKeys::LeftControl);
	Ctx->MapKey(S->Crouch, EKeys::C);
	Ctx->MapKey(S->Crouch, EKeys::Gamepad_FaceButton_Right);

	return S;
}

TArray<UInputAction*> UZwInputSet::AllActions() const
{
	return { Move, Look, Jump, Sprint, Crouch };
}
