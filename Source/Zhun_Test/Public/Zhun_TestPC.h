// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Zhun_TestPC.generated.h"

/**
 * 
 */
UCLASS()
class ZHUN_TEST_API AZhun_TestPC : public APlayerController
{
	GENERATED_BODY()

public:
	AZhun_TestPC();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void UpdateEnemyInfo(int NumSpawned, int NumKilled);

	void UpdateEnemyFleeingInfo(bool bIsFleeing);

	void OnCharacterHealthUpdate(float CurrentHealth, float MaxHealth);

	void OnCharacterDeath();

protected:
	// Reference to the UMG asset in the editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UPlayerWidget> UserWidgetClass;

private:
	UPROPERTY()
	class UPlayerWidget* UserWidgetInstance;
};
