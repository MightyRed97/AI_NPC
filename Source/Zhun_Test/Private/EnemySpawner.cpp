// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnemyCharacter.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

	//Definition for the Mesh that will serve as our visual representation.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	SpawnPointComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("SpawnPointComp"));
	SpawnPointComp->SetupAttachment(RootComponent);
	SpawnPointComp->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	SpawnPointComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// Set default values
	SpawnInterval = 10.0f;

	EnemyClass = AEnemyCharacter::StaticClass();

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// Set up the timer to spawn enemies
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, SpawnInterval, true);
}

void AEnemySpawner::SpawnEnemy()
{
	if (EnemyClass != nullptr)
	{
		// Get the spawn location and rotation
		FVector SpawnLocation = SpawnPointComp->GetComponentLocation() + SpawnPointComp->GetForwardVector() * 100.f;
		FRotator SpawnRotation = GetActorRotation();

		// Spawn the enemy
		AEnemyCharacter* EnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, SpawnLocation, SpawnRotation);
	}
}

