// Fill out your copyright notice in the Description page of Project Settings.


#include "Zhun_TestHUD.h"

#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"

AZhun_TestHUD::AZhun_TestHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/Zhun_Test/Textures/FirstPersonCrosshair.FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

/** This method draws a very simple crosshair */
void AZhun_TestHUD::DrawHUD()
{
	Super::DrawHUD();

	// Find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// Offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
		(Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)));

	// Draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}