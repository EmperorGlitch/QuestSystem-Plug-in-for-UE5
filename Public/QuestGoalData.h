#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestGoalData.generated.h"

UENUM(BlueprintType)
enum class EQuestGoalStatus : uint8
{
	Ready UMETA(DisplayName = "Ready"),
	Active UMETA(DisplayName = "Active"),
	Completed UMETA(DisplayName = "Completed"),
	Failed UMETA(DisplayName = "Failed")
};

USTRUCT(BlueprintType)
struct FQuestGoalData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoalId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GoalTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GoalDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestGoalStatus GoalStatus;

	FQuestGoalData()
		: GoalId(0),
		GoalTitle(TEXT("")),
		GoalDescription(TEXT("")),
		GoalStatus(EQuestGoalStatus::Ready)
	{
	}
};