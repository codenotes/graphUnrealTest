// Fill out your copyright notice in the Description page of Project Settings.

#include "test11.h"
#include "MyBlueprintFunctionLibrary.h"




std::map<int32, FGraphData> gGraphs;
UFUNCTION(BlueprintCallable, Category = "ROS")
void UMyBlueprintFunctionLibrary::createGraph(int32 id, float height, float width, int32 cellsize, float offsetX, float offsetY,
	float scaleX, float scaleY, float rangeOffsetX/*=0.0*/, float rangeOffsetY/*=0.0*/, float offsetLabelX, float offsetLabelY)
{
	FGraphData fg;


	fg.cellsize = cellsize;
	fg.height = height;
	fg.offsetX = offsetX;
	fg.offsetY = offsetY;
	fg.scaleX = scaleX;
	fg.scaleY = scaleY;
	fg.width = width;
	fg.rangeOffsetX = rangeOffsetX;
	fg.rangeOffsetY = rangeOffsetY;
	fg.offsetLabelX = offsetLabelX;
	fg.offsetLabelY = offsetLabelY;


	gGraphs[id] = fg;
}
UFUNCTION(BlueprintCallable, Category = "ROS")
bool UMyBlueprintFunctionLibrary::generateAxisFromID(int32 id, TArray<FVector4> &lines)
{
	//	const int cellsize = 20;   // 20 pixels wide/high cells. 
	FVector4 fv;
	FGraphData fg;

	fg = gGraphs[id]; //TODO test for valid key
	int gridNum = (int)(fg.width / fg.cellsize);
	int numHlines = fg.height / fg.cellsize;

	int i;
	for (i = 0; i <= gridNum; i++)
	{
		//	DrawLine(0, i * cellsize, gridSize * cellsize, i * cellsize);
		//DRAW HOIZONTAL LINES , X =0 Y increases
		fv.Z = 0;

		//if (i*fg.cellsize <= FMath::Abs(fg.height))
		if (i <= numHlines)
			fv.W = i*fg.cellsize;
		else
			goto skip;

		fv.X = gridNum * fg.cellsize;
		fv.Y = i * fg.cellsize;

		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,  FString::FromInt(fv.Y));


		lines.Add(fv);
	skip:
		//DrawLine(i * cellsize, 0, i * cellsize, gridSize * cellsize);
		//Draw VERTICAL , Y=0 X increases
		fv.X = i * fg.cellsize;
		fv.Y = 0;
		fv.Z = i * fg.cellsize;


		fv.W = gridNum * fg.cellsize <= FMath::Abs(fg.height) ? gridNum * fg.cellsize : fg.height;

		lines.Add(fv);



	}

	//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::FromInt(lines.Num()));
	return true;
}

UFUNCTION(BlueprintCallable, Category = "ROS")
void UMyBlueprintFunctionLibrary::generateGraphOffsetsFromID(int32 id, FVector4 line, FVector2D &PointA, FVector2D &PointB, FString & labelX, 
	FString &labelY, bool & isLabelX, FVector2D & labelPosition, bool & isOriginLine)
{
	FGraphData fg;

	fg = gGraphs[id];

	PointA.X = fg.offsetX + line.X;
	PointA.Y = fg.offsetY + line.Y;
	PointB.X = fg.offsetX + line.Z;
	PointB.Y = fg.offsetY + line.W;

	isOriginLine = false;

	//below just indicates the boundary of graph
	//if ((line.X == 0 && line.W == 0) || (line.Y == 0 && line.Z == 0))
	//	isOriginLine = true;




	if (line.Y == 0 && line.Z!=0) //X label
	{
		labelX = FString::FromInt((int32)((line.X) / fg.scaleX) + fg.rangeOffsetX);
		isLabelX = true;
	/*	labelPosition.X = PointB.X;
		labelPosition.Y = PointB.Y + fg.offsetLabelX*fg.cellsize;*/
		if (labelX == "0")
			isOriginLine = true;
		

		//		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::FromInt(line.X));
	}
	else //Y Label
	{
		float numHlines = fg.height / fg.cellsize;
		labelY = FString::FromInt((int32)((fg.height - line.Y ) / fg.scaleY) +fg.rangeOffsetY );
	//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::FromInt(line.Y));
	//	labelY = FString::FromInt((int32)((line.Y)));
		isLabelX = false;
		if (labelY == "0")
			isOriginLine = true;
		
	
	}


	labelPosition.Y = PointB.Y + fg.offsetLabelY*fg.cellsize;
	labelPosition.X = PointB.X + fg.offsetLabelX*fg.cellsize;


	//generateGraphOffsets(fg.height, fg.width, fg.cellsize, fg.offsetX, fg.offsetY, fg.scaleX, fg.scaleY, line, PointA, PointB, label);
}

UFUNCTION(BlueprintCallable, Category = "ROS")
void UMyBlueprintFunctionLibrary::translateGraphPoint(int32 id, float markerSize, FVector2D PointIn, FVector2D &PointOut)
{

	FGraphData fg = gGraphs[id];


//	PointOut.X = (PointIn.X) * fg.scaleX + fg.offsetX - markerSize / 2+FMath::Abs(fg.rangeOffsetX*fg.scaleX); //need the abs, not sure why
	PointOut.X = (PointIn.X) * fg.scaleX + fg.offsetX - markerSize / 2 + fg.rangeOffsetX*fg.scaleX*-1; //need the -, not sure why
	PointOut.Y = (fg.height - PointIn.Y* fg.scaleY) + fg.offsetY - markerSize / 2 + fg.rangeOffsetY*fg.scaleY;
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::FromInt(PointIn.Y));
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::FromInt(PointIn.X));
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::FromInt(fg.height));
}
