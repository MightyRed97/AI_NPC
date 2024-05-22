// Copyright Epic Games, Inc. All Rights Reserved.

#include "Zhun_TestGameMode.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

#include "Zhun_TestPC.h"
#include "Zhun_TestCharacter.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Zhun_TestHUD.h"

AZhun_TestGameMode::AZhun_TestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Zhun_Test/Blueprints/BP_TPCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    
    HUDClass = AZhun_TestHUD::StaticClass();

    // Initialize total number of enemies
    NumOfEnemy = 0;
    

    // Initialize number of enemies killed
	NumOfEnemyKilled = 0;
}

void AZhun_TestGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ZhunTestPC = Cast<AZhun_TestPC>(NewPlayer);

    // Initialize the enemy info of Player HUD
    FTimerHandle Timer;
    GetWorldTimerManager().SetTimer(Timer, this, &AZhun_TestGameMode::HandleEnemyFleeing, 0.2f);
}

void AZhun_TestGameMode::BeginPlay()
{
    // Get the number of enemies in the map
    Super::BeginPlay();
}

void AZhun_TestGameMode::HandleEnemyFleeing()
{
    // Check if half of the enemies are killed
    if ((float) NumOfEnemyKilled >= NumOfEnemy / 2.0f)
    {
        // Trigger fleeing behavior for remaining enemies
        TriggerFleeingBehaviorToAllEnemies(true);
    }
    else
    {
        TriggerFleeingBehaviorToAllEnemies(false);
    }

    if (ZhunTestPC) ZhunTestPC->UpdateEnemyInfo(NumOfEnemy, NumOfEnemyKilled);
}

void AZhun_TestGameMode::OnNewEnemySpawned()
{
    NumOfEnemy++;

    HandleEnemyFleeing();
}

// Call this function whenever an enemy is killed
void AZhun_TestGameMode::OnEnemyKilled()
{
    NumOfEnemyKilled++;

    HandleEnemyFleeing();
}

void AZhun_TestGameMode::RespawnCharacter(APlayerController* Controller)
{
    if (Controller)
    {
        // Destroy the old character
        if (ACharacter* OldCharacter = Controller->GetCharacter())
        {
            OldCharacter->Destroy();
        }

        APlayerStart* PlayerStart = nullptr;
        for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
        {
            PlayerStart = *It;
            break; // Only need one player start
        }

        if (!PlayerStart) return;

        // Spawn a new character
        FVector SpawnLocation = PlayerStart->GetActorLocation(); // Set your spawn location here
        FRotator SpawnRotation = PlayerStart->GetActorRotation(); // Set your spawn rotation here

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Controller;
        ACharacter* NewCharacter = GetWorld()->SpawnActor<ACharacter>(DefaultPawnClass, SpawnLocation, SpawnRotation, SpawnParams);

        // Possess the new character
        if (NewCharacter)
        {
            Controller->Possess(NewCharacter);
        }
    }
}

void AZhun_TestGameMode::TriggerFleeingBehaviorToAllEnemies(bool bIsFleeing)
{
    if (!GetWorld()) return;

    // Iterate through remaining enemies and make them flee from the player
    for (TActorIterator<AEnemyCharacter> It(GetWorld()); It; ++It)
    {
        AEnemyCharacter* EnemyCharacter = *It;
        if (!EnemyCharacter->IsDead())
        {
            // Implement fleeing behavior here
            AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(EnemyCharacter->GetController());
            if (EnemyAIController) EnemyAIController->TriggerFleeingBehavior(bIsFleeing);
        }
    }

    if (ZhunTestPC) ZhunTestPC->UpdateEnemyFleeingInfo(bIsFleeing);
}
