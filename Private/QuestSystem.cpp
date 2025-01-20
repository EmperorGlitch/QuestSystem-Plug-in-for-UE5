#include "QuestSystem.h"
#include "Engine/AssetManager.h"

void UQuestSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Load quests at initialization
	if (FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
	{
		LoadAllQuests();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AssetRegistry module is not loaded."));
	}
	

	UE_LOG(LogTemp, Log, TEXT("QuestSystem initialized with %d quests."), QuestMap.Num());
}

void UQuestSystem::LoadAllQuests()
{
	QuestMap.Empty();

	// Path to the folder with quest assets
	const FString QuestPath = TEXT("/Game/QuestsData");

	// Obtaining the Asset Register
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataArray;

	// Get all the assemblies in the folder
	FARFilter Filter;
	Filter.PackagePaths.Add(*QuestPath);
	//Filter.ClassNames.Add(UQuestData::StaticClass()->GetFName());
	//TODO!
	Filter.ClassPaths.Add(UQuestData::StaticClass()->GetClassPathName());
	Filter.bRecursivePaths = true;

	AssetRegistryModule.Get().GetAssets(Filter, AssetDataArray);

	// We go through the assets we found and add them to the map
	for (const FAssetData& AssetData : AssetDataArray)
	{
		if (UQuestData* QuestData = Cast<UQuestData>(AssetData.GetAsset()))
		{
			UE_LOG(LogTemp, Log, TEXT("Loaded quest: %s"), *QuestData->GetName());
			QuestMap.Add(QuestData->QuestId, QuestData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load quest asset: %s"), *AssetData.PackageName.ToString());
		}
	}

	if (QuestMap.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No quests loaded from the specified path: %s"), *QuestPath);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully loaded %d quests."), QuestMap.Num());
	}

}

void UQuestSystem::SetActiveQuest(int32 QuestId)
{
	if (UQuestData** FoundQuest = QuestMap.Find(QuestId))
	{
		ActiveQuest = *FoundQuest;

		// Set the first target as active
		if (ActiveQuest && ActiveQuest->QuestGoals.Num() > 0)
		{
			ActiveQuest->QuestGoals[0].GoalStatus = EQuestGoalStatus::Active;
			ActiveQuestGoal = ActiveQuest->QuestGoals[0];  // Set an active target

			// Trigger for UI changes and other actions
			OnGoalActivated.Broadcast(ActiveQuest->QuestId, ActiveQuestGoal.GoalId);
		}

		// Trigger for UI changes and other actions
		OnQuestActivated.Broadcast(QuestId);
	}
}

void UQuestSystem::UpdateGoalStatus(int32 QuestId, int32 GoalId, EQuestGoalStatus NewStatus)
{
	if (UQuestData** FoundQuest = QuestMap.Find(QuestId))
	{
		UQuestData* Quest = *FoundQuest;

		for (FQuestGoalData& Goal : Quest->QuestGoals)
		{
			if (Goal.GoalId == GoalId)
			{
				Goal.GoalStatus = NewStatus;

				if (NewStatus == EQuestGoalStatus::Active)
				{
					ActiveQuestGoal = Goal;
					OnGoalActivated.Broadcast(QuestId, GoalId);
				}
				else if (NewStatus == EQuestGoalStatus::Completed)
				{
					OnGoalCompleted.Broadcast(QuestId, GoalId);
				}

				bool bAllGoalsCompleted = true;
				for (const FQuestGoalData& OtherGoal : Quest->QuestGoals)
				{
					if (OtherGoal.GoalStatus != EQuestGoalStatus::Completed)
					{
						bAllGoalsCompleted = false;
						break;
					}
				}

				if (bAllGoalsCompleted)
				{
					OnQuestCompleted.Broadcast(QuestId);
				}

				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest with ID %d not found!"), QuestId);
	}
}
