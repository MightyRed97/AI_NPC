// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TimerManager.h"

#include "EnemyAIController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EnemyActionMode : uint8 {
	IDLE = 0 UMETA(DisplayName = "IDLE"),
	SWARM = 1 UMETA(DisplayName = "SWARM"),
	APPROACH = 2 UMETA(DisplayName = "APPROACH"),
	ATTACK = 3 UMETA(DisplayName = "ATTACK"),
	FLEE = 4 UMETA(DisplayName = "FLEE"),
};

UCLASS()
class ZHUN_TEST_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void OnCharacterDeath();

	UFUNCTION(BlueprintCallable, Category = "AI|Action Mode")
	void TriggerFleeingBehavior(bool bIsFleeing);

	// Set Enemy Action Mode and apply to Blackboard
	UFUNCTION(BlueprintCallable, Category = "AI|Action Mode")
	void SetActionMode(EnemyActionMode ActionMode);

	bool IsAttacking() const { return ActionMode == EnemyActionMode::ATTACK; }

	class AZhun_TestCharacter* GetTargetPlayer() const;

protected:
	virtual void Tick(float DeltaSeconds) override;

	// Called when the AI controller possesses a pawn
	virtual void OnPossess(APawn* InPawn) override;

	
	// *************************************************************
	//                      Player
	// *************************************************************

	// AI Perception
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// Function to be called by the timer
	void PlayerLostFromSight();

	// whether or not the enemy charater owned is fleeing
	UFUNCTION(BlueprintCallable, Category = "AI|Action Mode")
	bool IsEnemyFleeing() const;

	// To Save player character info into Blackboard
	void SetTargetPlayer(class AZhun_TestCharacter* playerCharacter);


	// *************************************************************
	//                      Swarm
	// *************************************************************

	// Called in every tick to handle the enemy's motion of swarming 
	void DecideActionMode();

	// For the enemies in a swarm to share information about the enemy
	class AZhun_TestCharacter* ReceiveTargetInfoFromNearby();

	// To find the location away from the neighbours in a swarm
	FVector FindLocationAwayFromNeighbour();

	// To find the location close to the neighbours nearby to be a swarm
	FVector FindLocationCloseToNeighbour();
	
	// To trace all enemies that can swarm
	TArray<class AEnemyCharacter*> TraceEnemiesCanSwarm();
	
	// To find all enemies in a radius from a location
	TArray<class AEnemyCharacter*> PerformSphereTrace(FVector Location, class AEnemyCharacter* EnemyToIgnore, float Radius);

protected:
	// *************************************************************
	//                      AI Components
	// *************************************************************

	// Behavior tree component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|AI")
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|AI")
	class UBehaviorTree* BehaviorTreeAsset;

	// AI Sight configuration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|AI")
	class UAISenseConfig_Sight* SightConfig;


	// *************************************************************
	//                      Enemy Action Mode
	// *************************************************************

	AEnemyCharacter* EnemyCharacter;

	// *************************************************************
	//                      Enemy Action Mode
	// *************************************************************

	// Enemy Action Mode
	EnemyActionMode ActionMode;

	void ApplyActionModeToBlackboard(FName& ActionModeKeyNameEnabled);


	// *************************************************************
	//                      Player and Enemy
	// *************************************************************

	// Timer to handle the event of player lost
	FTimerHandle PlayerLostTimerHandle;


	// *************************************************************
	//                      Blackboard Key
	// *************************************************************

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard Key")
	FName PlayerKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard Key")
	FName LastDistanceKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard Key")
	FName IsIdleKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard Key")
	FName IsSwarmingKeyName;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard Key")
	FName IsApproachingKeyName;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard Key")
	FName IsAttackingKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard Key")
	FName IsFleeingKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard Key")
	FName SwarmingLocationKeyName;


	// *************************************************************
	//                      Swarm
	// *************************************************************

	UPROPERTY(EditDefaultsOnly, Category = "AI|Distance")
	float MinDistance;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Distance")
	float MaxDistance;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Distance")
	float AttackingDistance;

	float TickTime;
};
