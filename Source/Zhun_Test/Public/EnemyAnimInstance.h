// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZHUN_TEST_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AnimNotify_OnAttackStarted();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_OnAttackStopped();

	UFUNCTION(BlueprintCallable)
	void OnDamage();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DamageAnimMontage;
};
