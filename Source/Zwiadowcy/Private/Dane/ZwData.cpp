#include "Dane/ZwData.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Zwiadowcy.h"

namespace ZwData
{
	FString DataDir()
	{
		return FPaths::ProjectContentDir() / TEXT("Data");
	}

	TSharedPtr<FJsonObject> LoadJson(const FString& RelativePath, FString* Error)
	{
		const FString Path = DataDir() / RelativePath;
		FString Text;
		auto Fail = [&](const FString& Msg) -> TSharedPtr<FJsonObject>
		{
			UE_LOG(LogZwiadowcy, Error, TEXT("%s: %s"), *Path, *Msg);
			if (Error)
			{
				*Error = Msg;
			}
			return nullptr;
		};
		if (!FFileHelper::LoadFileToString(Text, *Path))
		{
			return Fail(TEXT("nie można odczytać pliku"));
		}
		TSharedPtr<FJsonObject> Root;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Text);
		if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid())
		{
			return Fail(FString::Printf(TEXT("błędny JSON: %s"), *Reader->GetErrorMessage()));
		}
		return Root;
	}

	bool LoadLocations(TArray<FZwLocation>& Out, FString* Error)
	{
		Out.Reset();
		const TSharedPtr<FJsonObject> Root = LoadJson(TEXT("lokacje.json"), Error);
		const TArray<TSharedPtr<FJsonValue>>* List = nullptr;
		if (!Root || !Root->TryGetArrayField(TEXT("lokacje"), List))
		{
			if (Error && Error->IsEmpty())
			{
				*Error = TEXT("brak tablicy \"lokacje\"");
			}
			return false;
		}
		for (const TSharedPtr<FJsonValue>& V : *List)
		{
			const TSharedPtr<FJsonObject> O = V->AsObject();
			if (!O)
			{
				continue;
			}
			FZwLocation L;
			L.Id = FName(O->GetStringField(TEXT("id")));
			L.Name = O->GetStringField(TEXT("nazwa"));
			const TArray<TSharedPtr<FJsonValue>>* Pos = nullptr;
			if (O->TryGetArrayField(TEXT("xy"), Pos) && Pos->Num() == 2)
			{
				L.PositionM = FVector2D((*Pos)[0]->AsNumber(), (*Pos)[1]->AsNumber());
			}
			double R = 0.0;
			O->TryGetNumberField(TEXT("promien"), R);
			L.RadiusM = static_cast<float>(R);
			Out.Add(L);
		}
		return true;
	}
}
