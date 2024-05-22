// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Zhun_TestGameMode.generated.h"

UCLASS(minimalapi)
class AZhun_TestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZhun_TestGameMode();

    virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void OnNewEnemySpawned();

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void OnEnemyKilled();

    void RespawnCharacter(APlayerController* Controller);

protected:

    // Function to trigger fleeing behavior for remaining Enemies
    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void TriggerFleeingBehaviorToAllEnemies(bool bIsFleeing);

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void HandleEnemyFleeing();
    
protected:
    // Total number of Enemies
    UPROPERTY(BlueprintReadOnly, Category = "Enemy")
    int32 NumOfEnemy;

    // Number of Enemies killed
    UPROPERTY(BlueprintReadOnly, Category = "Enemy")
    int32 NumOfEnemyKilled;

    class AZhun_TestPC* ZhunTestPC;
};



