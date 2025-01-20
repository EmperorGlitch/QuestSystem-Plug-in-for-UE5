#pragma once

#include "CoreMinimal.h"

#include "QuestData.h"

#include "EditorAssetLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Views/SListView.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"

// Structure for displaying data in the list
struct FQuestListItem
{
    int32 Id;
    FString Title;
    FString Description;

    FQuestListItem(int32 InId, const FString& InTitle, const FString& InDescription)
        : Id(InId), Title(InTitle), Description(InDescription) {
    }
};

class SQuestEditorWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SQuestEditorWidget) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    int32 LastId;
    FString IdStorageFilePath;

    TSharedPtr<SEditableTextBox> IdTextBox;
    TSharedPtr<SEditableTextBox> TitleTextBox;
    TSharedPtr<SMultiLineEditableTextBox> DescriptionTextBox;

    TSharedPtr<SVerticalBox> GoalListPanel;
    TArray<TSharedPtr<SEditableTextBox>> GoalIdTextBoxes;
    TArray<TSharedPtr<SEditableTextBox>> GoalTitleTextBoxes;
    TArray<TSharedPtr<SMultiLineEditableTextBox>> GoalDescriptionTextBoxes;

    TArray<TSharedPtr<FQuestListItem>> QuestList; // Список квестов
    TSharedPtr<SListView<TSharedPtr<FQuestListItem>>> QuestListView; // Виджет списка

    TArray<TSharedPtr<FQuestGoalData>> Goals;

    void InitializeAssetsFolder();
    void InitializeQuestId();

    TSharedRef<ITableRow> GenerateQuestRow(TSharedPtr<FQuestListItem> Item, const TSharedRef<STableViewBase>& OwnerTable);
    void OnQuestSelected(TSharedPtr<FQuestListItem> SelectedItem, ESelectInfo::Type SelectInfo);
    void InitializeQuestList();
    void SortQuestList();

    void ClearInputFields();
    void ClearGoalList();

    FReply OnNewQuestPrepared();
    FReply OnSaveQuestClicked();
    FReply OnDeleteQuestClicked();

    FReply OnAddGoalClicked();
};