// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALifeCharacter::ALifeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize the life character's Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsDead = false;
}

// Called when the game starts or when spawned
void ALifeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALifeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALifeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALifeCharacter::OnHealthUpdate_Implementation()
{
	FString HealthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, HealthMessage);
}

void ALifeCharacter::OnDeath_Implementation()
{
	// Disable character movement
	GetCharacterMovement()->DisableMovement();

	if (UCapsuleComponent* CapComp = GetCapsuleComponent())
		CapComp->SetCollisionProfileName(TEXT("NoCollision"));

	if (USkeletalMeshComponent* SkelMesh = GetMesh())
	{
		// Enable physics simulation on the skeletal mesh
		SkelMesh->SetSimulatePhysics(true);

		// Set collision profile to ragdoll to handle physics interactions properly
		SkelMesh->SetCollisionProfileName(TEXT("Ragdoll"));

		// Optionally, apply an impulse to the character for immediate effect
		FVector Impulse = FVector(0, 0, 1000.0f); // Example impulse
		SkelMesh->AddImpulse(Impulse, NAME_None, true);
	}

	//FString DeathMessage = FString::Printf(TEXT("%s is dead."), *GetFName().ToString());
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DeathMessage);
}

void ALifeCharacter::SetCurrentHealth(float HealthValue)
{
	CurrentHealth = FMath::Clamp(HealthValue, 0.f, MaxHealth);
	
	if (CurrentHealth <= 0.f)
	{
		bIsDead = true;
		OnDeath();
	}

	OnHealthUpdate();
}

float ALifeCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return CurrentHealth;

	float DamageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(DamageApplied);
	return DamageApplied;
}

