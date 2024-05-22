// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_BB_FindFleeingLocation.generated.h"

/**
 *  Deprecated BT Service
 */

UCLASS()
class ZHUN_TEST_API UBTS_BB_FindFleeingLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTS_BB_FindFleeingLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Calculate random location for the enemy to flee from the player
	FVector GetRandomFleeingLocation(const FVector& PlayerLocation, const FVector& EnemyLocation);

	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FName PlayerKeyName;

	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FName FleeingLocationKeyName;

	UPROPERTY(EditAnywhere, Category = "Fleeing")
	float MinDistanceFromPlayer;
};
