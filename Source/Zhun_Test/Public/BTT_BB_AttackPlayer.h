// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_BB_AttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ZHUN_TEST_API UBTT_BB_AttackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
    UBTT_BB_AttackPlayer();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
