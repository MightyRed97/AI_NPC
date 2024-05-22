// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_BB_CheckTargetDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyCharacter.h"
#include "Zhun_TestCharacter.h"

UBTS_BB_CheckTargetDistance::UBTS_BB_CheckTargetDistance()
{
	CautiousMovementDistance = 3000.0f;
	FastMovementDistance = 1500.0f;

    PlayerKeyName = TEXT("TargetActor");
    LastDistanceKeyName = TEXT("LastDistanceToTarget");
    IsFleeingKeyName = TEXT("IsFleeing");
}

void UBTS_BB_CheckTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!EnemyCharacter) return;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return;

    // make disable if the enemy is fleeing
    bool bIsFleeing = BlackboardComp->GetValueAsBool(IsFleeingKeyName);
    if (bIsFleeing) return;

    AZhun_TestCharacter* PlayerCharacter = Cast<AZhun_TestCharacter>(BlackboardComp->GetValueAsObject(PlayerKeyName));
    if (!PlayerCharacter) return;

    float LastDistanceToTarget = BlackboardComp->GetValueAsFloat(LastDistanceKeyName);

    float DistanceToTarget = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
    if (DistanceToTarget <= FastMovementDistance)
    {
        // Set the enemy movement mode to Fast mode
        if (LastDistanceToTarget > FastMovementDistance || LastDistanceToTarget < 0.f)
            EnemyCharacter->ApplyFastMovementSpeed();
    }
    else if (DistanceToTarget <= CautiousMovementDistance)
    {
        // Set the enemy movement mode to Cautious mode
        if (LastDistanceToTarget > CautiousMovementDistance || LastDistanceToTarget < 0.f)
            EnemyCharacter->ApplyCautiousMovementSpeed();
    }
    else
    {
        // Set the enemy movement mode to Normal mode
        if (LastDistanceToTarget <= CautiousMovementDistance)
            EnemyCharacter->ApplyNormalMovementSpeed();
    }

    BlackboardComp->SetValueAsFloat(LastDistanceKeyName, DistanceToTarget);
}