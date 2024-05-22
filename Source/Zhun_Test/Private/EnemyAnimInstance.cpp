// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "EnemyCharacter.h"

void UEnemyAnimInstance::AnimNotify_OnAttackStarted()
{
    if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner()))
    {
        EnemyCharacter->OnAttackStarted();
    }
}

void UEnemyAnimInstance::AnimNotify_OnAttackStopped()
{
    if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner()))
    {
        EnemyCharacter->OnAttackStopped();
    }
}

void UEnemyAnimInstance::OnDamage()
{
    if (DamageAnimMontage) Montage_Play(DamageAnimMontage);
}
