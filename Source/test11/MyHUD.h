// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class TEST11_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
		virtual void     DrawHUD() override;
	
	
};
