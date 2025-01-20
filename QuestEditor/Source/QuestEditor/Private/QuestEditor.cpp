#include "QuestEditor.h"
#include "ToolMenus.h"
#include "ToolMenu.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Text/STextBlock.h"
#include "QuestEditorWidget.h"

IMPLEMENT_MODULE(FQuestEditorModule, QuestEditor)

void FQuestEditorModule::StartupModule()
{
    UToolMenus::Get()->RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FQuestEditorModule::RegisterMenus));
}

void FQuestEditorModule::ShutdownModule()
{
    UToolMenus::Get()->UnregisterOwner(this);
}

void FQuestEditorModule::RegisterMenus()
{
    FToolMenuOwnerScoped MenuOwner("QuestEditor");
    FToolMenuSection& Section = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window")->AddSection("QuestEditor", NSLOCTEXT("QuestEditor", "QuestEditor", "Quest Editor"));

    Section.AddMenuEntry(
        "OpenQuestEditor",
        NSLOCTEXT("QuestEditor", "OpenQuestEditor", "Quest Editor"),
        NSLOCTEXT("QuestEditor", "OpenQuestEditorTooltip", "Open the Quest Editor"),
        FSlateIcon(),
        FUIAction(FExecuteAction::CreateRaw(this, &FQuestEditorModule::OpenQuestEditor))
    );
}

void FQuestEditorModule::OpenQuestEditor()
{
    const FName QuestEditorTabName("Quest Editor");

    FGlobalTabmanager::Get()->InsertNewDocumentTab(
        QuestEditorTabName,
        FTabManager::ESearchPreference::PreferLiveTab,
        SNew(SDockTab)
        .TabRole(ETabRole::MajorTab)
        [
            SNew(SQuestEditorWidget)
        ]
    );
}