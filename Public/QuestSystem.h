#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QuestData.h"
#include "QuestSystem.generated.h"

// Event Declaration
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestEvent, int32, QuestId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGoalEvent, int32, QuestId, int32, GoalId);

UCLASS(Blueprintable)
class TWEEDUUSTER_UE_API UQuestSystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // Automatic subsystem initialization
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // Loading all quests
    UFUNCTION(BlueprintCallable, Category = "Quest System")
    void LoadAllQuests();

    // Setting an active quest
    UFUNCTION(BlueprintCallable, Category = "Quest System")
    void SetActiveQuest(int32 QuestId);

    // Updating the target status
    UFUNCTION(BlueprintCallable, Category = "Quest System")
    void UpdateGoalStatus(int32 QuestId, int32 GoalId, EQuestGoalStatus NewStatus);

    UFUNCTION(BlueprintCallable, Category = "Quest System")
    FString GetActiveQuestTitle()
    {
        return ActiveQuest ? ActiveQuest->QuestTitle : TEXT("");
    }

    UFUNCTION(BlueprintCallable, Category = "Quest System")
    FString GetActiveGoalTitle()
    {
        return !ActiveQuestGoal.GoalTitle.IsEmpty() ? ActiveQuestGoal.GoalTitle : TEXT("");
    }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Quest System")
    FQuestEvent OnQuestActivated;

    UPROPERTY(BlueprintAssignable, Category = "Quest System")
    FQuestEvent OnQuestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Quest System")
    FGoalEvent OnGoalActivated;

    UPROPERTY(BlueprintAssignable, Category = "Quest System")
    FGoalEvent OnGoalCompleted;

private:
    // Quest Vault
    UPROPERTY()
    TMap<int32, UQuestData*> QuestMap;

    // Active quest
    UPROPERTY()
    UQuestData* ActiveQuest;

    UPROPERTY()
    FQuestGoalData ActiveQuestGoal;
};