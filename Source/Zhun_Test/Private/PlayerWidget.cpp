// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind functions, initialize variables, etc.
}

void UPlayerWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
    // Assuming you have a ProgressBar named "Prg_Health" in your Widget Blueprint
    if (UProgressBar* PrgbarHeath = Cast<UProgressBar>(GetWidgetFromName(TEXT("Prg_Health"))))
    {
        PrgbarHeath->SetPercent(CurrentHealth / MaxHealth);
    }
}

void UPlayerWidget::UpdateEnemyNum(int TotalNum)
{
    // Assuming you have a TextBlock named "Text_Enemy_Num" in your Widget Blueprint
    if (UTextBlock* TextEnemyNum = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Enemy_Num"))))
    {
        TextEnemyNum->SetText(FText::FromString(FString::FromInt(TotalNum)));
    }
}

void UPlayerWidget::UpdateEnemyKilledNum(int NumKilled)
{
    // Assuming you have a TextBlock named "Text_EnemyKilled_Num" in your Widget Blueprint
    if (UTextBlock* TextEnemyKilledNum = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_EnemyKilled_Num"))))
    {
        TextEnemyKilledNum->SetText(FText::FromString(FString::FromInt(NumKilled)));
    }
}

void UPlayerWidget::UpdateEnemyFleeingStatus(bool bIsFleeing)
{
    // Assuming you have a TextBlock named "Text_Enemy_Fleeing" in your Widget Blueprint
    if (UTextBlock* TextEnemyFleeing = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Enemy_Fleeing"))))
    {
        TextEnemyFleeing->SetVisibility(bIsFleeing ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}
