// Fill out your copyright notice in the Description page of Project Settings.

#include "test11.h"
#include "MyBlueprintFunctionLibrary.h"




std::map<int32, FGraphData> gGraphs;
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::setGraphData(int32 handle, FGraphData graphData)
{
	createGraph(handle, graphData.height, graphData.width, graphData.cellsize, graphData.offsetX, graphData.offsetY, graphData.scaleX,
		graphData.scaleY, graphData.rangeOffsetX, graphData.rangeOffsetY, graphData.offsetLabelX, graphData.offsetLabelY,
		graphData.defaultMarkerSize);
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::getGraphData(int32 handle, FGraphData & graphData)
{
	graphData = gGraphs[handle];
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::createGraph(int32 id, float height, float width, int32 cellsize, float offsetX, float offsetY,
	float scaleX, float scaleY, float rangeOffsetX/*=0.0*/, float rangeOffsetY/*=0.0*/, float offsetLabelX, float offsetLabelY, float defaultMarkerSize )
{
	FGraphData fg;


	fg.height = height;
	fg.width = width;
	fg.cellsize = cellsize;
	fg.offsetX = offsetX;
	fg.offsetY = offsetY;
	fg.scaleX = scaleX;
	fg.scaleY = scaleY;
	fg.rangeOffsetX = rangeOffsetX;
	fg.rangeOffsetY = rangeOffsetY;
	fg.offsetLabelX = offsetLabelX;
	fg.offsetLabelY = offsetLabelY;
	fg.defaultMarkerSize = defaultMarkerSize;



	gGraphs[id] = fg;
}

UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::isfirstOrSecondFloat(bool isFirst, float first, float second, float &val)
{
	val = isFirst ? first : second;
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::addGraphPoint(int32 handle, FVector2D fv)
{
	FGraphData fg;
	FVector2D fvout;

	//fg & = gGraphs[handle];

	gGraphs[handle].RawPoints.Add(fv);

	translateGraphPoint(handle, gGraphs[handle].defaultMarkerSize, fv, fvout);
	gGraphs[handle].TranslatedPoints.Add(fvout);

	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::FromInt(fg.TranslatedPoints.Num()));
	//gGraphs[handle].TranslatedPointsQ.Enqueue(fvout);
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
bool UMyBlueprintFunctionLibrary::isPointInRect(FVector2D point, float height, float width, float offsetX, float offsetY)
{
	FVector2D UL, UR, LL, LR, v;

	UL = FVector2D(offsetX, offsetY);
	UR = FVector2D(width + offsetX, offsetY);
	LL = FVector2D(offsetX, height + offsetY);
	LR = FVector2D(width + offsetX, offsetY + height);


	bool bInRange = false;


	bInRange = point.X > UL.X && point.X<UR.X &&
		point.Y>UL.Y && point.Y < LL.Y;

	return bInRange;
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::clipTranslatedPoints(int32 handle)
{
	FGraphData *p = &gGraphs[handle];
	int cnt = p->TranslatedPoints.Num();
	FVector2D v;

	FVector2D UL, UR, LL, LR;

	UL = FVector2D(p->offsetX, p->offsetY);
	UR = FVector2D(p->width + p->offsetX, p->offsetY);
	LL = FVector2D(p->offsetX, p->height + p->offsetY);
	LR = FVector2D(p->width + p->offsetX, p->offsetY + p->height);


	bool bInRange = false;
	/*	std::ofstream of("c:\\temp\\gd.txt");



		of << "rectangle" << std::endl;
		of << "UL:" << UL.X << " ULY:" << UL.Y << " UR:" << UR.X << " URY:" << UR.Y << std::endl;
		of << "LL:" << LL.X << " LLY:" << LL.Y << " UL:" << LR.X << " LRY:" << LR.Y << std::endl;*/



	for (int i = 0; i < cnt; i++)
	{

		v = p->TranslatedPoints[i];

		//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, v.ToString());

			//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::fr(i));
//			of << "test vec:" << std::endl;

		bInRange = v.X > UL.X && v.X<UR.X &&
			v.Y>UL.Y && v.Y < LL.Y;

		//		of << "vX:"<< v.X << " vY:" << v.Y <<" unrange:"<<bInRange<< std::endl;


				//if not in region
		if (!bInRange)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, v.ToString());
			p->TranslatedPoints.RemoveAt(i, 1, true);
			cnt--;
			i--;


		}


	}
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::returnGraphTranslatedPoints(int32 handle, TArray<FVector2D> &translatedPoints, FVector2D &markerSize)
{
	

	translatedPoints = gGraphs[handle].TranslatedPoints;
	markerSize.X = gGraphs[handle].defaultMarkerSize;
	markerSize.Y = markerSize.X;
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::clearGraphPoints(int32 handle)
{
	gGraphs[handle].RawPoints.Empty();
	gGraphs[handle].TranslatedPoints.Empty();
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::createDemoPoints(int32 handle)
{
	//auto fg = gGraphs[handle];
	FVector2D fv;

	for (int i = -20; i <= 20; i++)
	{
		fv.X = i;
		fv.Y = i;

		addGraphPoint(handle, fv);
	}
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
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

UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
bool UMyBlueprintFunctionLibrary::generateAxis(int32 cellsize, int32 gridNum, float height, TArray<FVector4> &points)
{
	//	const int cellsize = 20;   // 20 pixels wide/high cells. 
	FVector4 fv;


	int i;
	for (i = 0; i <= gridNum; i++)
	{
		//	DrawLine(0, i * cellsize, gridSize * cellsize, i * cellsize);
		//DRAW HOIZONTAL LINES , X =0 Y increases
		fv.Z = 0;

		if (i*cellsize <= FMath::Abs(height))
			fv.W = i*cellsize;
		else
			goto skip;

		fv.X = gridNum * cellsize;
		fv.Y = i * cellsize;

		points.Add(fv);
	skip:
		//DrawLine(i * cellsize, 0, i * cellsize, gridSize * cellsize);
		//Draw VERTICAL , Y=0 X increases
		fv.X = i * cellsize;
		fv.Y = 0;
		fv.Z = i * cellsize;


		fv.W = gridNum * cellsize <= FMath::Abs(height) ? gridNum * cellsize : height;

		points.Add(fv);



	}
	return true;
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::generateGraphOffsets(float height, float width, int32 cellsize, float offsetX, float offsetY, float scaleX, float scaleY, FVector4 line, FVector2D &PointA, FVector2D &PointB, FString & label)
{
	PointA.X = offsetX + line.X;
	PointA.Y = offsetY + line.Y;
	PointB.X = offsetX + line.Z;
	PointB.Y = offsetY + line.W;




	if (line.Y == 0)
	{
		label = FString::FromInt((int32)((line.X / scaleX)));
	}
	else
	{
		label = FString::FromInt((int32)((height - line.Y) / scaleY));


	}
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
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

UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
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
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::switchStringOnBool(bool dofirst, FString first, FString second, FString & stringChosen)
{
	stringChosen = dofirst ? first : second;
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::switchColorOnBool(bool dofirst, FLinearColor first, FLinearColor second, FLinearColor & color)
{
	color = dofirst ? first : second;
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::switchFloatOnBool(bool dofirst, float first, float second, float & val)
{
	val = dofirst ? first : second;
}

#define ifdo(X) X= addValuetoExisting? X+=value:value
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::changeGraphParm(int32 handle, EGraphParam eParam, float value, bool addValuetoExisting/*=true*/)
{

	FGraphData *pFg = &gGraphs[handle];

	switch (eParam)
	{
	case EGraphParam::SCALE_X:
		ifdo(pFg->scaleX);
		break;
	case EGraphParam::SCALE_Y:
		ifdo(pFg->scaleY);
		break;
	case EGraphParam::SCALE_X_AND_Y:
		ifdo(pFg->scaleY);
		ifdo(pFg->scaleX);
		break;

	case EGraphParam::Width:
		ifdo(pFg->width);
		break;
	case EGraphParam::Height:
		ifdo(pFg->height);
		break;
	case EGraphParam::OFFSET_X:
		ifdo(pFg->offsetX);

		break;
	case EGraphParam::OFFSET_Y:
		ifdo(pFg->offsetY);
		break;
	case EGraphParam::OFFSET_X_AND_Y:
		ifdo(pFg->offsetY);
		ifdo(pFg->offsetX);
		break;

	case EGraphParam::OFFSETLABEL_X:
		ifdo(pFg->offsetLabelX);
		break;
	case EGraphParam::OFFSETLABEL_Y:
		ifdo(pFg->offsetLabelY);
		break;
	case EGraphParam::OFFSETLABEL_XY:
		ifdo(pFg->offsetY);
		ifdo(pFg->offsetX);
		break;

	case EGraphParam::RANGEOFFSET_X:
		ifdo(pFg->offsetX);
		break;
	case EGraphParam::RANGEOFFSET_Y:
		ifdo(pFg->offsetLabelY);
		break;
	case EGraphParam::RANGEOFFSET_X_AND_Y:
		ifdo(pFg->offsetY);
		ifdo(pFg->offsetX);
		break;


	case EGraphParam::DEFAULTMARKERSIZE:
		ifdo(pFg->defaultMarkerSize);
		break;
	default:
		break;
	}

	recalcGraphTranslatedPoints(handle);
	clipTranslatedPoints(handle);
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::recalcGraphTranslatedPoints(int32 handle)
{
	FGraphData *p = &gGraphs[handle];
	FVector2D fv;

	p->TranslatedPoints.Empty();
	for (auto a : gGraphs[handle].RawPoints)
	{

		translateGraphPoint(handle, p->defaultMarkerSize, a, fv);
		p->TranslatedPoints.Add(fv);
	}
}
