// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_FleeingSafety.generated.h"

/**
 * 
 */
UCLASS()
class ZHUN_TEST_API UBTD_FleeingSafety : public UBTDecorator_Blackboard
{
	GENERATED_BODY()

public:
	UBTD_FleeingSafety();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	FName PlayerKeyName;

	UPROPERTY(EditAnywhere, Category = "Blackboard Key")
	float FleeingDistance;
};
