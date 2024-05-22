// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BB_AttackPlayer.h"

#include "AIController.h"
#include "TimerManager.h"
#include "EnemyCharacter.h"

UBTT_BB_AttackPlayer::UBTT_BB_AttackPlayer()
{
	NodeName = "Attack Player";
}

EBTNodeResult::Type UBTT_BB_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AEnemyCharacter* Character = Cast<AEnemyCharacter>(AIController->GetPawn());
    if (!Character)
    {
        return EBTNodeResult::Failed;
    }

    Character->PlayAttackAnimation();

    return EBTNodeResult::Succeeded;
}
