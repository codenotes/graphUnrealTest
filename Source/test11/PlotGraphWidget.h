// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "PlotGraphWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEST11_API UPlotGraphWidget : public UUserWidget
{
	GENERATED_BODY()
	
		virtual void   OnPaint(FPaintContext& Context	)	;

		
	
};
