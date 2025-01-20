#include "QuestEditorWidget.h"

void SQuestEditorWidget::Construct(const FArguments& InArgs)
{
	// Initialize ID and text fields
	InitializeAssetsFolder();
	InitializeQuestId();
	InitializeQuestList();

	// Initialize the panel to display the targets
	GoalListPanel = SNew(SVerticalBox);

	ChildSlot
		[
			SNew(SSplitter)
				+ SSplitter::Slot()
				.Value(0.4f)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Create and Edit Quests"))
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
								.Justification(ETextJustify::Center)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						.HAlign(HAlign_Left)
						[
							SNew(SBox)
								.WidthOverride(115.0f)
								[
									SNew(SButton)
										.Text(FText::FromString("New Quest"))
										.OnClicked(this, &SQuestEditorWidget::OnNewQuestPrepared)
										.HAlign(HAlign_Center)
								]
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Quest ID"))
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SAssignNew(IdTextBox, SEditableTextBox)
								.Text(FText::FromString(FString::FromInt(LastId))) // Set the current ID
								.IsReadOnly(true)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Quest Title"))
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SAssignNew(TitleTextBox, SEditableTextBox)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Quest Description"))
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SAssignNew(DescriptionTextBox, SMultiLineEditableTextBox)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Quest Goals"))
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
								.Justification(ETextJustify::Center)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SNew(SBox)
								//.HeightOverride(150)
								.MinDesiredHeight(0.f)
								.MaxDesiredHeight(175)
								[
									SNew(SScrollBox)
										+ SScrollBox::Slot()
										[
											GoalListPanel.ToSharedRef()
										]
								]
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SNew(SButton)
								.Text(FText::FromString("Add Goal"))
								.HAlign(HAlign_Center)
								.OnClicked(this, &SQuestEditorWidget::OnAddGoalClicked)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						.HAlign(HAlign_Left)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(5)
								[
									SNew(SButton)
										.Text(FText::FromString("Save Quest"))
										.OnClicked(this, &SQuestEditorWidget::OnSaveQuestClicked)
										.ButtonStyle(FCoreStyle::Get(), "FlatButton.Success")
								]
								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(5)
								[
									SNew(SButton)
										.Text(FText::FromString("Delete Quest"))
										.OnClicked(this, &SQuestEditorWidget::OnDeleteQuestClicked)
										.ButtonStyle(FCoreStyle::Get(), "FlatButton.Danger")
								]
						]
				]
				+ SSplitter::Slot()
				.Value(0.6f)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Quests List"))
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
								.Justification(ETextJustify::Center)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(5)
						[
							SNew(SScrollBox)
								+ SScrollBox::Slot()
								[
									SAssignNew(QuestListView, SListView<TSharedPtr<FQuestListItem>>)
										.ListItemsSource(&QuestList)
										.OnGenerateRow(this, &SQuestEditorWidget::GenerateQuestRow)
										.OnSelectionChanged(this, &SQuestEditorWidget::OnQuestSelected)
								]
						]
				]
		];

	SortQuestList();
}

void SQuestEditorWidget::InitializeAssetsFolder()
{
	// Check if the directory exists, if not, create it
	FString DirectoryPath = FPaths::ProjectContentDir() / TEXT("QuestsData");
	if (!FPaths::DirectoryExists(DirectoryPath))
	{
		IFileManager::Get().MakeDirectory(*DirectoryPath);
	}
}

void SQuestEditorWidget::InitializeQuestId()
{
	// Path to file c ID
	IdStorageFilePath = FPaths::ProjectSavedDir() / TEXT("QuestsData") / TEXT("LastQuestId.txt");

	// Load the current ID from the file, if it exists
	if (FPaths::FileExists(IdStorageFilePath))
	{
		FString FileContents;
		if (FFileHelper::LoadFileToString(FileContents, *IdStorageFilePath))
		{
			LastId = FCString::Atoi(*FileContents);

			//// Increment ID
			LastId++;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to read LastQuestId.txt file. Defaulting to 1."));
			LastId = 1;
		}
	}
	else
	{
		// If the file does not exist, create it with initial value ID = 1
		LastId = 1;
		if (!FFileHelper::SaveStringToFile(FString::FromInt(LastId), *IdStorageFilePath))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create LastQuestId.txt file."));
		}
	}
}

void SQuestEditorWidget::InitializeQuestList()
{
	// Clearing the current list
	QuestList.Empty();

	// Load quests from the folder “/Game/QuestsData/”
	TArray<FAssetData> AssetDataArray;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistryModule.Get().GetAssetsByPath(FName("/Game/QuestsData"), AssetDataArray);

	for (const FAssetData& AssetData : AssetDataArray)
	{
		if (UQuestData* QuestData = Cast<UQuestData>(AssetData.GetAsset()))
		{
			QuestList.Add(MakeShareable(new FQuestListItem(QuestData->QuestId, QuestData->QuestTitle, QuestData->QuestDescription.ToString())));
		}
	}

	// Updating the list view
	if (QuestListView.IsValid())
	{
		QuestListView->RequestListRefresh();
	}
}

void SQuestEditorWidget::SortQuestList()
{
	if (QuestList.Num() > 0)
	{
		QuestList.Sort([](const TSharedPtr<FQuestListItem>& A, const TSharedPtr<FQuestListItem>& B) {
			return A->Id < B->Id; // Sorting in ascending order Id
			});
	}

	// Updating the list display
	if (QuestListView.IsValid())
	{
		QuestListView->RequestListRefresh();
	}
}

void SQuestEditorWidget::ClearInputFields()
{
	// Updating text fields
	IdTextBox->SetText(FText::FromString(FString::FromInt(LastId))); // Set the current ID
	TitleTextBox->SetText(FText::FromString(TEXT("")));
	DescriptionTextBox->SetText(FText::FromString(TEXT("")));

	// Updating the list view
	if (QuestListView.IsValid())
	{
		QuestListView->RequestListRefresh();
	}
}

TSharedRef<ITableRow> SQuestEditorWidget::GenerateQuestRow(TSharedPtr<FQuestListItem> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FQuestListItem>>, OwnerTable)
		[
			SNew(STextBlock).Text(FText::FromString(FString::Printf(TEXT("%d - %s (%s)"), Item->Id, *Item->Title, *Item->Description)))
		];
}

void SQuestEditorWidget::OnQuestSelected(TSharedPtr<FQuestListItem> SelectedItem, ESelectInfo::Type SelectInfo)
{
	if (SelectedItem.IsValid())
	{
		// Load data of the selected quest
		FString PackageName = FString::Printf(TEXT("/Game/QuestsData/%d"), SelectedItem->Id);
		if (UQuestData* QuestData = Cast<UQuestData>(StaticLoadObject(UQuestData::StaticClass(), nullptr, *PackageName)))
		{
			// Set values in text fields
			IdTextBox->SetText(FText::FromString(FString::FromInt(QuestData->QuestId)));
			TitleTextBox->SetText(FText::FromString(QuestData->QuestTitle));
			DescriptionTextBox->SetText(QuestData->QuestDescription);

			LastId = FCString::Atoi(*(IdTextBox->GetText().ToString()));

			ClearGoalList();

			for (const FQuestGoalData& Goal : QuestData->QuestGoals)
			{
				TSharedPtr<SEditableTextBox> GoalIdBox = SNew(SEditableTextBox)
					.Text(FText::AsNumber(Goal.GoalId))
					.IsReadOnly(true);

				TSharedPtr<SEditableTextBox> GoalTitleBox = SNew(SEditableTextBox)
					.Text(FText::FromString(Goal.GoalTitle));

				TSharedPtr<SMultiLineEditableTextBox> GoalDescriptionBox = SNew(SMultiLineEditableTextBox)
					.Text(FText::FromString(Goal.GoalDescription));

				GoalIdTextBoxes.Add(GoalIdBox);
				GoalTitleTextBoxes.Add(GoalTitleBox);
				GoalDescriptionTextBoxes.Add(GoalDescriptionBox);

				if (GoalListPanel.IsValid())
				{
					GoalListPanel->AddSlot()
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									SNew(STextBlock)
										.Text(FText::FromString("Goal ID"))
										.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									GoalIdBox.ToSharedRef()
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									SNew(STextBlock)
										.Text(FText::FromString("Goal Title"))
										.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									GoalTitleBox.ToSharedRef()
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									SNew(STextBlock)
										.Text(FText::FromString("Goal Description"))
										.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									GoalDescriptionBox.ToSharedRef()
								]
						];
				}
			}
		}
	}
}

FReply SQuestEditorWidget::OnNewQuestPrepared()
{
	InitializeQuestId();
	ClearInputFields();
	ClearGoalList();

	if (QuestListView.IsValid())
	{
		QuestListView->SetSelection(nullptr);
	}

	return FReply::Handled();
}

FReply SQuestEditorWidget::OnAddGoalClicked()
{
	TSharedPtr<SEditableTextBox> NewGoalIdBox = SNew(SEditableTextBox).Text(FText::AsNumber(GoalIdTextBoxes.Num())).IsReadOnly(true);
	TSharedPtr<SEditableTextBox> NewGoalNameBox = SNew(SEditableTextBox);
	TSharedPtr<SMultiLineEditableTextBox> NewGoalDescriptionBox = SNew(SMultiLineEditableTextBox);

	// Add new text fields for target data entry
	GoalIdTextBoxes.Add(NewGoalIdBox);
	GoalTitleTextBoxes.Add(NewGoalNameBox);
	GoalDescriptionTextBoxes.Add(NewGoalDescriptionBox);

	// Add new text fields to the vertical scroll list
	int32 GoalNumber = GoalIdTextBoxes.Num() - 1;
	GoalListPanel->AddSlot()
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(STextBlock)
						.Text(FText::FromString("Goal ID"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					GoalIdTextBoxes[GoalNumber].ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(STextBlock)
						.Text(FText::FromString("Goal Title"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					GoalTitleTextBoxes[GoalNumber].ToSharedRef()
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(STextBlock)
						.Text(FText::FromString("Goal Description"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					GoalDescriptionTextBoxes[GoalNumber].ToSharedRef()
				]
		];

	return FReply::Handled();
}

void SQuestEditorWidget::ClearGoalList()
{
	// Remove all items from GoalListPanel
	if (GoalListPanel.IsValid())
	{
		GoalListPanel->ClearChildren(); // Deletes all slots from the panel
	}

	// Clearing linked lists
	GoalIdTextBoxes.Empty();
	GoalTitleTextBoxes.Empty();
	GoalDescriptionTextBoxes.Empty();
}

FReply SQuestEditorWidget::OnSaveQuestClicked()
{
	if (QuestListView.IsValid())
	{
		QuestListView->SetSelection(nullptr);
	}

	FString QuestId = FString::FromInt(LastId);
	FString QuestTitle = TitleTextBox.IsValid() ? TitleTextBox->GetText().ToString() : TEXT("");
	FString QuestDescription = DescriptionTextBox.IsValid() ? DescriptionTextBox->GetText().ToString() : TEXT("");

	// Path for saving an asset
	FString AssetPath = FPaths::ProjectContentDir() / TEXT("QuestsData") / QuestId + TEXT(".") + QuestId;

	// Check if an asset with this name already exists
	UQuestData* ExistingQuest = Cast<UQuestData>(StaticLoadObject(UQuestData::StaticClass(), nullptr, *FString::Printf(TEXT("/Game/QuestsData/%s.%s"), *QuestId, *QuestId)));
	if (ExistingQuest)
	{
		// Updating the data of an existing assembly
		ExistingQuest->QuestTitle = QuestTitle;
		ExistingQuest->QuestDescription = FText::FromString(QuestDescription);

		// Clearing old targets
		ExistingQuest->QuestGoals.Empty();

		// Keeping the new goals
		for (int32 i = 0; i < GoalIdTextBoxes.Num(); ++i)
		{
			int32 GoalId = GoalIdTextBoxes[i].IsValid()
				? FCString::Atoi(*GoalIdTextBoxes[i]->GetText().ToString())
				: 0;
			FString GoalTitle = GoalTitleTextBoxes.IsValidIndex(i) && GoalTitleTextBoxes[i].IsValid() ? GoalTitleTextBoxes[i]->GetText().ToString() : TEXT("");
			FString GoalDescription = GoalDescriptionTextBoxes.IsValidIndex(i) && GoalDescriptionTextBoxes[i].IsValid() ? GoalDescriptionTextBoxes[i]->GetText().ToString() : TEXT("");

			if (GoalTitle.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("Goal %d is missing a title. Skipping."), i);
				continue;
			}

			FQuestGoalData NewGoal;
			NewGoal.GoalId = GoalId;
			NewGoal.GoalTitle = GoalTitle;
			NewGoal.GoalDescription = GoalDescription;
			NewGoal.GoalStatus = EQuestGoalStatus::Ready; // Set default status

			ExistingQuest->QuestGoals.Add(NewGoal);
		}

		// Save the updated package
		UPackage* ExistingPackage = ExistingQuest->GetOutermost();
		if (!UPackage::SavePackage(ExistingPackage, ExistingQuest, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FPackageName::LongPackageNameToFilename(FString::Printf(TEXT("/Game/QuestsData/%s"), *QuestId), FPackageName::GetAssetPackageExtension())))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save updated QuestData asset to disk."));
			return FReply::Handled();
		}

		UE_LOG(LogTemp, Log, TEXT("Updated QuestData asset saved successfully: %s"), *QuestId);

		InitializeQuestId();
		ClearInputFields();
		ClearGoalList();
		InitializeQuestList();
		SortQuestList();

		return FReply::Handled();
	}

	// Create a new object UQuestData
	UQuestData* NewQuestData = NewObject<UQuestData>(GetTransientPackage(), UQuestData::StaticClass(), FName(*QuestId), RF_Public | RF_Standalone);
	if (!NewQuestData)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create UQuestData object."));
		return FReply::Handled();
	}

	// Fill in the fields of the quest
	NewQuestData->QuestId = FCString::Atoi(*QuestId);
	NewQuestData->QuestTitle = QuestTitle;
	NewQuestData->QuestDescription = FText::FromString(QuestDescription);

	// Creating quest objectives
	for (int32 i = 0; i < GoalIdTextBoxes.Num(); ++i)
	{
		int32 GoalId = GoalIdTextBoxes[i].IsValid()
			? FCString::Atoi(*GoalIdTextBoxes[i]->GetText().ToString())
			: 0;

		FString GoalTitle = GoalTitleTextBoxes.IsValidIndex(i) && GoalTitleTextBoxes[i].IsValid()
			? GoalTitleTextBoxes[i]->GetText().ToString()
			: TEXT("");

		FString GoalDescription = GoalDescriptionTextBoxes.IsValidIndex(i) && GoalDescriptionTextBoxes[i].IsValid()
			? GoalDescriptionTextBoxes[i]->GetText().ToString()
			: TEXT("");

		if (GoalTitle.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Goal %d is missing a title. Skipping."), i);
			continue;
		}

		FQuestGoalData NewGoal;
		NewGoal.GoalId = GoalId;
		NewGoal.GoalTitle = GoalTitle;
		NewGoal.GoalDescription = GoalDescription;
		NewGoal.GoalStatus = EQuestGoalStatus::Ready;

		NewQuestData->QuestGoals.Add(NewGoal);
	}

	// Save the object as an asset
	FString PackageName = FString::Printf(TEXT("/Game/QuestsData/%s"), *QuestId);
	UPackage* Package = CreatePackage(*PackageName);
	if (!Package)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create package for QuestData asset."));
		return FReply::Handled();
	}

	NewQuestData->Rename(*QuestId, Package);
	FAssetRegistryModule::AssetCreated(NewQuestData);

	// Save the package to disk
	if (!UPackage::SavePackage(Package, NewQuestData, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension())))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save QuestData asset to disk."));
		return FReply::Handled();
	}

	UE_LOG(LogTemp, Log, TEXT("QuestData asset saved successfully: %s"), *PackageName);

	// Save the new ID back to the file
	FFileHelper::SaveStringToFile(FString::FromInt(LastId), *IdStorageFilePath);

	InitializeQuestId();

	ClearInputFields();
	ClearGoalList();
	InitializeQuestList();
	SortQuestList();

	return FReply::Handled();
}

FReply SQuestEditorWidget::OnDeleteQuestClicked()
{
	// Check if the quest is selected
	if (QuestListView->GetSelectedItems().Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No quest selected to delete!"));
		return FReply::Handled();
	}

	// Receive the chosen quest
	TSharedPtr<FQuestListItem> SelectedQuest = QuestListView->GetSelectedItems()[0];
	FString AssetPath = FString::Printf(TEXT("/Game/QuestsData/%d"), SelectedQuest->Id);

	// Checking the existence of an asset
	if (!UEditorAssetLibrary::DoesAssetExist(AssetPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset does not exist: %s"), *AssetPath);
		return FReply::Handled();
	}

	// Deleting the asset
	if (!UEditorAssetLibrary::DeleteAsset(AssetPath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to delete asset: %s"), *AssetPath);
		return FReply::Handled();
	}

	// Remove the quest from the list
	QuestList.Remove(SelectedQuest);

	InitializeQuestId();
	ClearInputFields();
	ClearGoalList();
	SortQuestList();

	UE_LOG(LogTemp, Log, TEXT("Quest deleted: %s"), *AssetPath);

	return FReply::Handled();
}