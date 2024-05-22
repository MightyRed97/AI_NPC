// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "Zhun_TestCharacter.h"
#include "Zhun_TestGameMode.h"
#include "EnemyCharacter.h"
#include "ZhunUtils.h"

AEnemyAIController::AEnemyAIController()
{
	// Initialize behavior tree component
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

    // Find default behavior tree asset
    BehaviorTreeAsset = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/Zhun_Test/AI/BT_Enemy.BT_Enemy"));

    // Initialize AI Perception component
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    SetPerceptionComponent(*PerceptionComponent);

    // Create AI Sight configuration
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 10000.0f;
    SightConfig->LoseSightRadius = 10500.0f;
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 100.f;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    // Add AI Sight configuration to Perception component
    PerceptionComponent->ConfigureSense(*SightConfig);

    // Bind perception updated event
    PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);

    ActionMode = EnemyActionMode::IDLE;

    PlayerKeyName = TEXT("TargetActor");
    LastDistanceKeyName = TEXT("LastDistanceToTarget");
    IsIdleKeyName = TEXT("IsIdle");
    IsSwarmingKeyName = TEXT("IsSwarming");
    IsApproachingKeyName = TEXT("IsApproaching");
    IsAttackingKeyName = TEXT("IsAttacking");
    IsFleeingKeyName = TEXT("IsFleeing");
    SwarmingLocationKeyName = TEXT("SwarmingLocation");

    MinDistance = 75.f;
    MaxDistance = 200.f;
    AttackingDistance = 120.f;

    EnemyCharacter = nullptr;

    TickTime = 0;
}

// OnPossess implementation
void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    EnemyCharacter = Cast<AEnemyCharacter>(InPawn);

    // Check if the Behavior Tree is valid
    if (BehaviorTreeAsset)
    {
        // Start the Behavior Tree execution
        RunBehaviorTree(BehaviorTreeAsset);
    }

    // let the game mode know that a new enemy is spawned
    AZhun_TestGameMode* GameMode = Cast<AZhun_TestGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode) GameMode->OnNewEnemySpawned();
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    TickTime += DeltaSeconds;
    if (TickTime < 0.5f) return;

    TickTime = 0.f;

    // This function is called every 0.5 seconds
    DecideActionMode();
}

void AEnemyAIController::OnCharacterDeath()
{
    if (AZhun_TestGameMode* GameMode = Cast<AZhun_TestGameMode>(UGameplayStatics::GetGameMode(this))) 
        GameMode->OnEnemyKilled();

    GetCharacter()->DetachFromControllerPendingDestroy();
}

// Event implementation
void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // Implement logic for when the target perception is updated
    AZhun_TestCharacter* PlayerCharacter = Cast<AZhun_TestCharacter>(Actor);
    if (!PlayerCharacter) return;

    if (PlayerCharacter->ActorHasTag(TEXT("Player")) && Stimulus.WasSuccessfullySensed())
    {
        // when the player is percepted
        GetWorldTimerManager().ClearTimer(PlayerLostTimerHandle);

        SetTargetPlayer(PlayerCharacter);
    }
    else if (!IsEnemyFleeing())
    {
        // when the player is lost
        GetWorldTimerManager().SetTimer(PlayerLostTimerHandle, this, &AEnemyAIController::PlayerLostFromSight, 5.0f, false);
    }
}

// Player Lost Timer function implementation
void AEnemyAIController::PlayerLostFromSight()
{
    // This function will be called after 5 seconds
    SetTargetPlayer(nullptr);

    if (EnemyCharacter) EnemyCharacter->ApplyNormalMovementSpeed();
}

void AEnemyAIController::TriggerFleeingBehavior(bool bIsFleeing)
{
    if (bIsFleeing)
    {
        SetActionMode(EnemyActionMode::FLEE);
        if (EnemyCharacter) EnemyCharacter->ApplyFastMovementSpeed();
    }
    else if (ActionMode == EnemyActionMode::FLEE)
    {
        // if the enemy isn't fleeing, he continues to move
        SetActionMode(EnemyActionMode::IDLE);
        if (EnemyCharacter) EnemyCharacter->ApplyNormalMovementSpeed();
    }
}

void AEnemyAIController::SetActionMode(EnemyActionMode Mode)
{
    ActionMode = Mode;

    switch (ActionMode)
    {
        case EnemyActionMode::IDLE:
        default:
            ApplyActionModeToBlackboard(IsIdleKeyName);
            break;
        case EnemyActionMode::SWARM:
            ApplyActionModeToBlackboard(IsSwarmingKeyName);
            break;
        case EnemyActionMode::APPROACH:
            ApplyActionModeToBlackboard(IsApproachingKeyName);
            break;
        case EnemyActionMode::ATTACK:
            ApplyActionModeToBlackboard(IsAttackingKeyName);
            break;
        case EnemyActionMode::FLEE:
            ApplyActionModeToBlackboard(IsFleeingKeyName);
            break;
    }
}

void AEnemyAIController::ApplyActionModeToBlackboard(FName& ActionModeKeyNameEnabled)
{
    if (!Blackboard) return;

    Blackboard->SetValueAsBool(IsIdleKeyName, ActionModeKeyNameEnabled == IsIdleKeyName);
    Blackboard->SetValueAsBool(IsSwarmingKeyName, ActionModeKeyNameEnabled == IsSwarmingKeyName);
    Blackboard->SetValueAsBool(IsApproachingKeyName, ActionModeKeyNameEnabled == IsApproachingKeyName);
    Blackboard->SetValueAsBool(IsAttackingKeyName, ActionModeKeyNameEnabled == IsAttackingKeyName);
    Blackboard->SetValueAsBool(IsFleeingKeyName, ActionModeKeyNameEnabled == IsFleeingKeyName);

    if (ActionModeKeyNameEnabled == IsIdleKeyName)
    {
        EnemyCharacter->SetActionModeHeadText("Idle");
    }
    else if (ActionModeKeyNameEnabled == IsSwarmingKeyName)
    {
        EnemyCharacter->SetActionModeHeadText("Swarming");
    }
    else if (ActionModeKeyNameEnabled == IsApproachingKeyName)
    {
        EnemyCharacter->SetActionModeHeadText("Approaching");
    }
    else if (ActionModeKeyNameEnabled == IsAttackingKeyName)
    {
        EnemyCharacter->SetActionModeHeadText("Attacking");
    }
    else if (ActionModeKeyNameEnabled == IsFleeingKeyName)
    {
        EnemyCharacter->SetActionModeHeadText("Fleeing");

        Blackboard->SetValueAsFloat(LastDistanceKeyName, -1.f);
    }
}

bool AEnemyAIController::IsEnemyFleeing() const
{
    if (Blackboard) return Blackboard->GetValueAsBool(IsFleeingKeyName);

    return false;
}

void AEnemyAIController::SetTargetPlayer(class AZhun_TestCharacter* PlayerCharacter)
{
    if (Blackboard)
    {
        Blackboard->SetValueAsObject(PlayerKeyName, PlayerCharacter);
        Blackboard->SetValueAsFloat(LastDistanceKeyName, -1.0f);
    }
}

AZhun_TestCharacter* AEnemyAIController::GetTargetPlayer() const
{
    if (!Blackboard) return nullptr;

    return Cast<AZhun_TestCharacter>(Blackboard->GetValueAsObject(PlayerKeyName));
}

void AEnemyAIController::DecideActionMode()
{
    if (!EnemyCharacter) return;

    if (IsEnemyFleeing()) return;

    AZhun_TestCharacter* PlayerCharacter = GetTargetPlayer();

    if (!PlayerCharacter)
    {
        // receive target info from nearby enemies
        PlayerCharacter = ReceiveTargetInfoFromNearby();
        if (!PlayerCharacter)
        {
            SetActionMode(EnemyActionMode::IDLE);
            return;
        }
    }

    // check if the enemy is enough close to player to attack
    float DistanceToPlayer = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
    if (DistanceToPlayer <= AttackingDistance)
    {
        SetActionMode(EnemyActionMode::ATTACK);
        return;
    }

    // check the min-max rule in a swarm
    int32 NumOfEnemyInMin = PerformSphereTrace(EnemyCharacter->GetActorLocation(), EnemyCharacter, MinDistance).Num();
    int32 NumOfEnemyInMax = PerformSphereTrace(EnemyCharacter->GetActorLocation(), EnemyCharacter, MaxDistance).Num();

    if ((NumOfEnemyInMin == 0 && NumOfEnemyInMax > 0) ||
        (NumOfEnemyInMax == 0 && TraceEnemiesCanSwarm().Num() == 0))
    {
        SetActionMode(EnemyActionMode::APPROACH);
        return;
    }

    SetActionMode(EnemyActionMode::SWARM);

    // move to be a swarm by rule
    FVector SwarmingLocation = EnemyCharacter->GetActorLocation();
    if (NumOfEnemyInMin > 0)
    {
        // there is an enemy in MIN_DISTANCE
        SwarmingLocation = FindLocationAwayFromNeighbour();
    }
    else
    {
        // there isn't any enemy is MAX_DISTANCE
        SwarmingLocation = FindLocationCloseToNeighbour();
    }

    DrawDebugPoint(
        GetWorld(),
        SwarmingLocation,
        10.f,
        FColor::Red,
        false, // Persistent lines (false for a one-time dot)
        0.2f // Duration (use 0.0f for a one-time dot)
    );

    if (Blackboard) Blackboard->SetValueAsVector(SwarmingLocationKeyName, SwarmingLocation);
}

AZhun_TestCharacter* AEnemyAIController::ReceiveTargetInfoFromNearby()
{
    AZhun_TestCharacter* PlayerCharacter = nullptr;

    // Try to receive player target info from nearby enemies within a max distance
    TArray<AEnemyCharacter*> Enemies = PerformSphereTrace(EnemyCharacter->GetActorLocation(), EnemyCharacter, MaxDistance);
    for (AEnemyCharacter* Enemy : Enemies)
    {
        if (AZhun_TestCharacter* TargetCharacter = Enemy->GetTargetPlayer())
        {
            PlayerCharacter = TargetCharacter;
            break;
        }
    }

    if (PlayerCharacter) SetTargetPlayer(PlayerCharacter);

    return PlayerCharacter;
}

FVector AEnemyAIController::FindLocationAwayFromNeighbour()
{
    //if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Cyan, FString::Printf(TEXT("%s away from neighboour"), *EnemyCharacter->GetFName().ToString()));

    AZhun_TestCharacter* PlayerCharacter = GetTargetPlayer();
    if (!PlayerCharacter) return EnemyCharacter->GetActorLocation();

    // calculate the direction to be far away from the neighbours
    TArray<AEnemyCharacter*> CloseNeighbours = PerformSphereTrace(EnemyCharacter->GetActorLocation(), EnemyCharacter, MinDistance);
    FVector DirectAway = FVector::ZeroVector;

    for (AEnemyCharacter* CloseNeighbour : CloseNeighbours)
    {
        DirectAway += (EnemyCharacter->GetActorLocation() - CloseNeighbour->GetActorLocation());
    }
    DirectAway.Normalize();
    
    // get the random location fit to the swarming condition
    FVector Delta = FVector::ZeroVector;
    FVector SwarmingLocation = EnemyCharacter->GetActorLocation();
    while(Delta.Size() <= MaxDistance)
    {
        FVector Location = EnemyCharacter->GetActorLocation() + Delta;

        int32 NumOfEnemyInMin = PerformSphereTrace(Location, EnemyCharacter, MinDistance).Num();
        int32 NumOfEnemyInMax = PerformSphereTrace(Location, EnemyCharacter, MaxDistance).Num();

        if (NumOfEnemyInMin == 0 && NumOfEnemyInMax > 0) SwarmingLocation = Location;

        Delta += DirectAway;
    }

    FVector VectorToPlayer = PlayerCharacter->GetActorLocation() - EnemyCharacter->GetActorLocation();
    FVector VectorToSwarmingLocation = SwarmingLocation - EnemyCharacter->GetActorLocation();

    // check if the direction is away from the player
    if (ZhunUtils::IsAngleLessThan90Degrees(VectorToPlayer, VectorToSwarmingLocation))
        return SwarmingLocation;

    //if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Cyan, FString::Printf(TEXT("%s stopped"), *EnemyCharacter->GetFName().ToString()));

    return EnemyCharacter->GetActorLocation();
}

FVector AEnemyAIController::FindLocationCloseToNeighbour()
{
    //if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Cyan, FString::Printf(TEXT("%s close neighboour"), *EnemyCharacter->GetFName().ToString()));

    AZhun_TestCharacter* PlayerCharacter = GetTargetPlayer();
    if (!PlayerCharacter) return EnemyCharacter->GetActorLocation();

    TArray<AEnemyCharacter*> Neighbours = TraceEnemiesCanSwarm();
    if (Neighbours.Num() == 0) return EnemyCharacter->GetActorLocation();

    // Find the nearest enemy
    float MinNeighbourDistance = FLT_MAX;
    FVector SwarmingLocation = EnemyCharacter->GetActorLocation();
    for (AEnemyCharacter* Neighbour : Neighbours)
    {
        float NeighbourDistance = FVector::Dist(EnemyCharacter->GetActorLocation(), Neighbour->GetActorLocation());
        if (NeighbourDistance < MinNeighbourDistance)
        {
            SwarmingLocation = Neighbour->GetActorLocation();
            MinNeighbourDistance = NeighbourDistance;
        }
    }
    
    FVector VectorToPlayer = PlayerCharacter->GetActorLocation() - EnemyCharacter->GetActorLocation();
    FVector VectorToSwarmingLocation = SwarmingLocation - EnemyCharacter->GetActorLocation();
    
    // check if the direction is away from the player
    if (ZhunUtils::IsAngleLessThan90Degrees(VectorToPlayer, VectorToSwarmingLocation))
        return SwarmingLocation;

    return EnemyCharacter->GetActorLocation();
}

TArray<AEnemyCharacter*> AEnemyAIController::TraceEnemiesCanSwarm()
{
    AZhun_TestCharacter* PlayerCharacter = GetTargetPlayer();
    if (!PlayerCharacter) return TArray<AEnemyCharacter*>();

    // Trace all enemies that can swarm
    float DistanceToPlayer = FVector::Dist(PlayerCharacter->GetActorLocation(), EnemyCharacter->GetActorLocation());
    return PerformSphereTrace(EnemyCharacter->GetActorLocation(), EnemyCharacter, DistanceToPlayer);
}

TArray<AEnemyCharacter*> AEnemyAIController::PerformSphereTrace(FVector Location, class AEnemyCharacter* EnemyToIgnore, float Radius)
{
    // Array to ignore self
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(EnemyToIgnore);

    TArray<FHitResult> OutHits;

    bool bHit = UKismetSystemLibrary::SphereTraceMulti(
        this,
        Location,
        Location,
        Radius,
        UEngineTypes::ConvertToTraceType(ECC_Visibility),
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        OutHits,
        true
    );

    TArray<AEnemyCharacter*> EnemyCharactersTraced;

    for (FHitResult Hit : OutHits)
    {
        AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Hit.Actor);

        if (Enemy && !Enemy->IsAttacking())
        {
            // only tracing approaching or swarming enemies
            EnemyCharactersTraced.Add(Enemy);
        }
    }

    return EnemyCharactersTraced;
}