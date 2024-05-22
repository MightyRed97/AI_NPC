// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_BB_CheckTargetDistance.generated.h"

/**
 * 
 */
UCLASS()
class ZHUN_TEST_API UBTS_BB_CheckTargetDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_BB_CheckTargetDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FName PlayerKeyName;

	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FName LastDistanceKeyName;

	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FName IsFleeingKeyName;

	UPROPERTY(EditAnywhere, Category = "AI|Movement")
	float CautiousMovementDistance;

	UPROPERTY(EditAnywhere, Category = "AI|Movement")
	float FastMovementDistance;
};
