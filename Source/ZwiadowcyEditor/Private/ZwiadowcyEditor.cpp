// Moduł tylko dla edytora: narzędzia do budowy świata i testów wywoływane ze skryptów (Scripts/).
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogZwiadowcyEditor, Log, All);

class FZwiadowcyEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override
	{
		UE_LOG(LogZwiadowcyEditor, Log, TEXT("ZwiadowcyEditor gotowy"));
	}
};

IMPLEMENT_MODULE(FZwiadowcyEditorModule, ZwiadowcyEditor);
