// Fill out your copyright notice in the Description page of Project Settings.

/**
 *  Deprecated BT Service
 */

#include "BTS_BB_FindFleeingLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnemyCharacter.h"
#include "Zhun_TestCharacter.h"
#include "ZhunUtils.h"

UBTS_BB_FindFleeingLocation::UBTS_BB_FindFleeingLocation()
{
	PlayerKeyName = TEXT("TargetActor");
	FleeingLocationKeyName = TEXT("FleeingLocation");

    MinDistanceFromPlayer = 3000.0f;
}

void UBTS_BB_FindFleeingLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!EnemyCharacter) return;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return;

    AZhun_TestCharacter* PlayerCharacter = Cast<AZhun_TestCharacter>(BlackboardComp->GetValueAsObject(PlayerKeyName));
    if (!PlayerCharacter) return;

    float Distance = FVector::Dist(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
    if (Distance >= MinDistanceFromPlayer && MinDistanceFromPlayer > 0.f) return;

    // Calculate direction away from player
    FVector DirectionAwayFromPlayer = (EnemyCharacter->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal();

    FVector FleeingLocation;
    FVector NextLocation = EnemyCharacter->GetActorLocation();
    NextLocation += DirectionAwayFromPlayer * EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed * DeltaSeconds;

    if (ZhunUtils::IsLocationReachable(EnemyCharacter->GetActorLocation(), NextLocation, GetWorld()))
    {
        FleeingLocation = NextLocation;

        //if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Orange, "Back");
    }
    else
    {
        NextLocation = GetRandomFleeingLocation(PlayerCharacter->GetActorLocation(), EnemyCharacter->GetActorLocation());

        //if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Orange, "Random");
    }

    BlackboardComp->SetValueAsVector(FleeingLocationKeyName, NextLocation);
}

FVector UBTS_BB_FindFleeingLocation::GetRandomFleeingLocation(const FVector& PlayerLocation, const FVector& EnemyLocation)
{
    float DistanceOrg = FVector::Dist(PlayerLocation, EnemyLocation);

    float Radius = 1000.0f;  // Example radius
    const int32 MaxAttempts = 100;

    FVector Location = FVector::ZeroVector;
    for (int32 i = 0; i < MaxAttempts; i++)
    {
        Location = ZhunUtils::GetRandomReachablePointInRadius(EnemyLocation, Radius, GetWorld());
        if (Location != FVector::ZeroVector && FVector::Dist(Location, PlayerLocation) > DistanceOrg) break;
    }
    
    if (Location != FVector::ZeroVector && FVector::Dist(Location, PlayerLocation) > DistanceOrg)
    {
        // RandomReachablePoint contains a valid reachable point within the radius
        return Location;
    }
    
    return EnemyLocation;
}
