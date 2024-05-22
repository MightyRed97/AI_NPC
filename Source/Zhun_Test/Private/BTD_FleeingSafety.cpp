// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_FleeingSafety.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyCharacter.h"
#include "AIController.h"
#include "Zhun_TestCharacter.h"

UBTD_FleeingSafety::UBTD_FleeingSafety()
{
	PlayerKeyName = TEXT("TargetActor");
	FleeingDistance = 4000.0f;
}

bool UBTD_FleeingSafety::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AZhun_TestCharacter* TargetCharacter = Cast<AZhun_TestCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(PlayerKeyName));
	if (!TargetCharacter) return false;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!EnemyCharacter) return false;

	// Check if the player is out-sight of the enemy
	float Distance = FVector::Dist(TargetCharacter->GetActorLocation(), EnemyCharacter->GetActorLocation());

	return Distance <= FleeingDistance;
}
