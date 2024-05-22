// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TextRenderComponent.h"
#include "Blueprint/UserWidget.h"

#include "EnemyAIController.h"
#include "Zhun_TestCharacter.h"
#include "EnemyAnimInstance.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    ActionModeTextRenderComp = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ActionModeTextRenderComponent"));
    ActionModeTextRenderComp->SetupAttachment(GetMesh()); // Attach to the root component
    ActionModeTextRenderComp->SetText(TEXT("Idle")); // Set initial text
    ActionModeTextRenderComp->SetHorizontalAlignment(EHTA_Center); // Set text alignment
    ActionModeTextRenderComp->SetVerticalAlignment(EVRTA_TextCenter); // Set vertical alignment

    SpeedTextRenderComp = CreateDefaultSubobject<UTextRenderComponent>(TEXT("SpeedTextRenderComponent"));
    SpeedTextRenderComp->SetupAttachment(GetMesh()); // Attach to the root component
    SpeedTextRenderComp->SetText(TEXT("Normal")); // Set initial text
    SpeedTextRenderComp->SetHorizontalAlignment(EHTA_Center); // Set text alignment
    SpeedTextRenderComp->SetVerticalAlignment(EVRTA_TextCenter); // Set vertical alignment

    AIControllerClass = AEnemyAIController::StaticClass();

    AttackRange = 20.f;
    AttackDamage = 5.f;

    NormalMovementSpeed = 150.f;
    CautiousMovementSpeed = 150.f;
    FastMovementSpeed = 300.f;

    ApplyNormalMovementSpeed();
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyCharacter::OnHealthUpdate_Implementation()
{
    //Super::OnHealthUpdate_Implementation();

    if (UEnemyAnimInstance* EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance()))
    {
        EnemyAnimInstance->OnDamage();
    }
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FindTargetAndAttack();
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::SetSpeedHeadText(const FString& HeadText)
{
    SpeedTextRenderComp->SetText(HeadText);
}

void AEnemyCharacter::SetActionModeHeadText(const FString& HeadText)
{
    ActionModeTextRenderComp->SetText(HeadText);
}

void AEnemyCharacter::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

    // Destroy text render components to show the status of the enemy
    ActionModeTextRenderComp->DestroyComponent();
    SpeedTextRenderComp->DestroyComponent();

    if (AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController()))
    {
        // let the AI controller know the owned enemy character is dead
        EnemyAIController->OnCharacterDeath();
    }
}

void AEnemyCharacter::FindTargetAndAttack()
{
    if (bIsAttacking)
    {
        // Trace the player target to attack
        FVector StartLocation = GetMesh()->GetSocketLocation("hand_r");

        TArray<FHitResult> OutHits;
        FCollisionShape Sphere = FCollisionShape::MakeSphere(AttackRange);

        bool bHit = GetWorld()->SweepMultiByChannel(OutHits, StartLocation, StartLocation, FQuat::Identity, ECC_Pawn, Sphere);

        if (bHit)
        {
            for (auto& Hit : OutHits)
            {
                if (AZhun_TestCharacter* Character = Cast<AZhun_TestCharacter>(Hit.GetActor()))
                {
                    UGameplayStatics::ApplyDamage(Character, AttackDamage, GetController(), this, UDamageType::StaticClass());
                    bIsAttacking = false;
                    break;
                }
            }
        }
    }
}

void AEnemyCharacter::UpdateWalkSpeed(float WalkSpeed)
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AEnemyCharacter::ApplyNormalMovementSpeed()
{
    UpdateWalkSpeed(NormalMovementSpeed);

    SetSpeedHeadText(TEXT("Normal"));
}

void AEnemyCharacter::ApplyCautiousMovementSpeed()
{
    UpdateWalkSpeed(CautiousMovementSpeed);

    SetSpeedHeadText(TEXT("Cautious"));
}

void AEnemyCharacter::ApplyFastMovementSpeed()
{
    UpdateWalkSpeed(FastMovementSpeed);

    SetSpeedHeadText(TEXT("Fast"));
}

bool AEnemyCharacter::IsAttacking() const
{
    AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());
    if (!EnemyAIController) return false;

    return EnemyAIController->IsAttacking();
}

AZhun_TestCharacter* AEnemyCharacter::GetTargetPlayer() const
{
    AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

    if (!EnemyAIController) return nullptr;
    
    return EnemyAIController->GetTargetPlayer();
}

void AEnemyCharacter::PlayAttackAnimation()
{
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        if (!AnimInstance->IsAnyMontagePlaying() && AnimMontage)
        {
            AnimInstance->Montage_Play(AnimMontage);
        }
    }
}

void AEnemyCharacter::OnAttackStarted()
{
    bIsAttacking = true;
}

void AEnemyCharacter::OnAttackStopped()
{
    bIsAttacking = false;
}
