// Fill out your copyright notice in the Description page of Project Settings.


#include "Zhun_TestPC.h"

#include "Kismet/GameplayStatics.h"

#include "PlayerWidget.h"
#include "Zhun_TestGameMode.h"

AZhun_TestPC::AZhun_TestPC()
{

}

void AZhun_TestPC::BeginPlay()
{
	Super::BeginPlay();

    // Check if the UserWidgetClass is set
    if (UserWidgetClass)
    {
        // Create the widget and store the instance
        UserWidgetInstance = CreateWidget<UPlayerWidget>(this, UserWidgetClass);

        // Add the widget to the viewport
        if (UserWidgetInstance)
        {
            UserWidgetInstance->AddToViewport();
        }
    }
}

void AZhun_TestPC::UpdateEnemyInfo(int NumSpawned, int NumKilled)
{
    if (UserWidgetInstance)
    {
        UserWidgetInstance->UpdateEnemyNum(NumSpawned);
        UserWidgetInstance->UpdateEnemyKilledNum(NumKilled);
    }
}

void AZhun_TestPC::UpdateEnemyFleeingInfo(bool bIsFleeing)
{
    if (UserWidgetInstance)
        UserWidgetInstance->UpdateEnemyFleeingStatus(bIsFleeing);
}

void AZhun_TestPC::OnCharacterHealthUpdate(float CurrentHealth, float MaxHealth)
{
    if (UserWidgetInstance)
        UserWidgetInstance->UpdateHealth(CurrentHealth, MaxHealth);
}

void AZhun_TestPC::OnCharacterDeath()
{
    AZhun_TestGameMode* GameMode = Cast<AZhun_TestGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        GameMode->RespawnCharacter(this);
    }
}

