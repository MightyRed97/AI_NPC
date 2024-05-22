// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZHUN_TEST_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Player")
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void UpdateEnemyNum(int Num);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void UpdateEnemyKilledNum(int Num);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void UpdateEnemyFleeingStatus(bool bIsFleeing);

protected:
	virtual void NativeConstruct() override;
};
