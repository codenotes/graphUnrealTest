// Fill out your copyright notice in the Description page of Project Settings.

#include "test11.h"
#include "MyHUD.h"




//AMyHUD::AMyHUD()
//{
//	//static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/crosshair.crosshair'"));
//	//static ConstructorHelpers::FObjectFinder<UFont> HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
//
//	//_crosshairTex = CrosshairTexObj.Object;
//	//_HUDFont = HUDFontOb.Object;
//}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
	UE_LOG(LogTemp, Warning, TEXT("DrawHUD called"));
	FVector2D start, stop;

	start.X = 0;
	start.Y = 0;

	stop.X = 400;
	stop.Y = 400;

	Canvas->K2_DrawLine(start, stop);

}