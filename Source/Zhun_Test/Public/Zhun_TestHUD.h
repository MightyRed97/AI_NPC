// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Zhun_TestHUD.generated.h"

/**
 * 
 */
UCLASS()
class ZHUN_TEST_API AZhun_TestHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AZhun_TestHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
};
