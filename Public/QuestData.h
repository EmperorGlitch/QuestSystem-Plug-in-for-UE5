#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestGoalData.h"
#include "QuestData.generated.h"

UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
	Ready UMETA(DisplayName = "Ready"),
	Active UMETA(DisplayName = "Active"),
	Completed UMETA(DisplayName = "Completed"),
	Failed UMETA(DisplayName = "Failed")
};

UCLASS(BlueprintType)
class TWEEDUUSTER_UE_API UQuestData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	int32 QuestId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FString QuestTitle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FText QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestGoalData> QuestGoals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestStatus QuestStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRepeatable;

	UQuestData()
		: QuestId(0),
		QuestTitle(TEXT("")),
		QuestDescription(FText::FromString("")),
		QuestStatus(EQuestStatus::Ready),
		bIsRepeatable(false)
	{
	}
};