// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LifeCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ZHUN_TEST_API AEnemyCharacter : public ALifeCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnHealthUpdate_Implementation() override;

	virtual void OnDeath_Implementation() override;

	void FindTargetAndAttack();

public:
	void SetSpeedHeadText(const FString& HeadText);

	void SetActionModeHeadText(const FString& HeadText);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UpdateWalkSpeed(float Speed);
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplyNormalMovementSpeed();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplyCautiousMovementSpeed();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ApplyFastMovementSpeed();

	bool IsAttacking() const;

	class AZhun_TestCharacter* GetTargetPlayer() const;

	void PlayAttackAnimation();

	void OnAttackStarted();

	void OnAttackStopped();

protected:
	// Text Render Component to show the action mode of enemy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTextRenderComponent* ActionModeTextRenderComp;

	// Text Render Component to show the speed status of enemy
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTextRenderComponent* SpeedTextRenderComp;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float NormalMovementSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float CautiousMovementSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float FastMovementSpeed;

	bool bIsAttacking;
};
