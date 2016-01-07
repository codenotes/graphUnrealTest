// Fill out your copyright notice in the Description page of Project Settings.

#include "test11.h"
#include "MyBlueprintFunctionLibrary.h"



std::map<int32, FGraphData> gGraphs;
std::map<int32, TArray<FVector2D>> gPointSets;

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
void UMyBlueprintFunctionLibrary::createGraph(int32 id, float height, float width, int32 cellsize, float offsetX, float offsetY, float scaleX,
	float scaleY, float rangeOffsetX , float rangeOffsetY , float offsetLabelX , float offsetLabelY ,
	float defaultMarkerSize,
	float minRangeX ,
	float maxRangeX ,
	float minRangeY ,
	float maxRangeY 
	)
{
	FGraphData fg;



#define EXP
#ifdef EXP 
	fg.cellsize = cellsize;
	fg.offsetX = offsetX;
	fg.offsetY = offsetY;
	
	
	fg.height = height;
	fg.width = width;


	//greg1
	//float rangeX = FMath::Abs(maxRangeX) + FMath::Abs(minRangeX);
	//float rangeY = FMath::Abs(maxRangeY) + FMath::Abs(minRangeY);
	//fg.scaleX = width / rangeX;// scaleX;// height;// scaleX;// fg.height / FMath::Abs(maxRangeX) / 2;
	//fg.scaleY = height / rangeY;//  scaleY;// width;// scaleY; // / FMath::Abs(maxRangeY) / 2;
	//fg.rangeOffsetX =  minRangeX;
	//fg.rangeOffsetY =  minRangeY;

	float rangeX = FMath::Abs(maxRangeX) + FMath::Abs(minRangeX);
	float rangeY = FMath::Abs(maxRangeY) + FMath::Abs(minRangeY);
	
//	fg.scaleX = width / rangeX;// scaleX;// height;// scaleX;// fg.height / FMath::Abs(maxRangeX) / 2;
//	fg.scaleY = height / rangeY;//  scaleY;// width;// scaleY; // / FMath::Abs(maxRangeY) / 2;
		
	
	fg.scaleX = scaleX;// scaleX;// height;// scaleX;// fg.height / FMath::Abs(maxRangeX) / 2;
	fg.scaleY = scaleY;// / rangeY;//  scaleY;// width;// scaleY; // / FMath::Abs(maxRangeY) / 2;


	
	fg.rangeOffsetX =  minRangeX;
	fg.rangeOffsetY =  minRangeY;


	
	fg.offsetLabelX = offsetLabelX;
	fg.offsetLabelY = offsetLabelY;
	fg.defaultMarkerSize = defaultMarkerSize;
	
	
//	recalcRanges(id);
	
	
	fg.minRangeX = minRangeX;
	fg.maxRangeX = maxRangeX;
	fg.minRangeY = minRangeY;
	fg.maxRangeY = maxRangeY;

	//recalcRanges(id);
#else


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




#endif



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

		bInRange = v.X >= UL.X && v.X<=UR.X &&
			v.Y>=UL.Y && v.Y <= LL.Y;

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

	for (int i = -50; i <= 50; i++)
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
	int gridNum = (int)((fg.width ) / fg.cellsize);
	int numHlines = fg.height / fg.cellsize;
	
	//gridum = 1000/100 = 100
	//numH = 500/100 =  5





	int i;
	for (i = 0; i <= gridNum; i++)
	{
		//	DrawLine(0, i * cellsize, gridSize * cellsize, i * cellsize);
		//DRAW HOIZONTAL LINES , X =0 Y increases
		fv.Z = 0; //z= endpoint Y, if endpoint Y=0 then this is along the X AXIS

		//if (i*fg.cellsize <= FMath::Abs(fg.height))
		if (i <= numHlines)
			fv.W = i*fg.cellsize; //really endpoint.X W=X
		else
			goto skip;

		fv.X = gridNum * fg.cellsize; // just width.
		fv.Y = i * fg.cellsize;

		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,  FString::FromInt(fv.Y));


		lines.Add(fv);
	skip:
		//DrawLine(i * cellsize, 0, i * cellsize, gridSize * cellsize);
		//Draw VERTICAL , Y=0 X increases
		fv.X = i * fg.cellsize;
		fv.Y = 0;
		fv.Z = i * fg.cellsize;

		UE_LOG(LogTemp, Warning, TEXT("X:%f Y:%f z:%f W:%f"), fv.X, fv.Y,fv.Z,fv.W);


		fv.W = gridNum * fg.cellsize <= FMath::Abs(fg.height) ? gridNum * fg.cellsize : fg.height;
		

		lines.Add(fv);



	}
#define LOGIT
#ifdef LOGIT
	UE_LOG(LogTemp, Warning, TEXT("---"));
#endif

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

	//UE_LOG(LogTemp, Warning, TEXT("START OFFSETS"));
//#define ORIGINAL
#ifdef ORIGINAL
	float ylevel = ((fg.height - line.Y) / fg.scaleY) + fg.rangeOffsetY;
	float xlevel = ((line.X) / fg.scaleX) + fg.rangeOffsetX;
	
#else


	


	///(FMath::Abs(fg.maxRangeY) + FMath::Abs(fg.minRangeY)) - fg.maxRangeY )
	//float ylevel = ((fg.height - line.Y)/fg.scaleY  - fg.maxRangeY );//TODO:divide by scale
//	float ylevel = ((fg.height - line.Y) / (float)fg.cellHeightY - fg.maxRangeY);// +FMath::Abs(fg.maxRangeY - fg.minRangeY) / fg.scaleY;//TODO:divide by scale
	
	/*
	def test(x, minrange) :
		val = x*scale + minrange
		return val*/
	
	float ylevel = (line.Y / fg.cellHeightY)*fg.scaleY + fg.minRangeY;
	
	float xlevel = line.X/fg.cellLengthX + fg.minRangeX ;


	FString s;
#endif

	ylevel= roundf(ylevel * 100) / 100;
	xlevel = roundf(xlevel * 100) / 100;
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::Fromf());
	//if the position 

	//X Y Z W = X Y Z=X2 W=Y2
	if (line.Y == 0 && line.Z != 0 || line.X==0 && line.W==fg.height) //X label
	{
//		UE_LOG(LogTemp, Error, TEXT("linex:%f maxrangex:%f scalex:%f"), line.X, fg.maxRangeX, fg.scaleX);
		labelX = FString::SanitizeFloat(xlevel);
		isLabelX = true;
		/*	labelPosition.X = PointB.X;
			labelPosition.Y = PointB.Y + fg.offsetLabelX*fg.cellsize;*/
		if (labelX == "0")
			isOriginLine = true;


		//		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::FromInt(line.X));
	}
	else //Y Label
	{
		//UE_LOG(LogTemp, Warning, TEXT("y:%f h:%f mrangy:%f scale:%f range:%f calc:%f"), line.Y, fg.cellHeightY, fg.maxRangeY, fg.scaleY, FMath::Abs(fg.maxRangeY - fg.minRangeY), ylevel);
		
		
		//float numHlines = fg.height / fg.cellsize;
		labelY = FString::SanitizeFloat(ylevel);//FString::Printf(TEXT("%f"), ylevel); 
		//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::FromInt(line.Y));
		//	labelY = FString::FromInt((int32)((line.Y)));
		isLabelX = false;
		if (labelY == "0")
			isOriginLine = true;


	}

#ifdef LOGIT
	//if (ylevel == 40 )
	//{
//		FString f=FString::Printf(TEXT("TYPE:%s, xlev:%d ylev:%d posx:%f posy:%f"),  isLabelX?TEXT("XLABEL"):TEXT("YLABEL"),         xlevel, ylevel,  labelPosition.X, labelPosition.Y);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *f);
	//}
#endif

	//to prevent overlapp at the LL corner of text
	float cornerYOffset = 0;
	float cornerXOffset = 0;



	//is this the LL corner of Y
	if (ylevel == fg.minRangeY && !isLabelX) //upper is getting this also, TODO:fix
	{
		cornerYOffset = 0;// 10;
		cornerXOffset = 0;//  -25;
		//just get rig of the label
	//	labelY = "";
	}

	//if(line.X==line.Y==0 && isLabelX) 
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::FromInt(xlevel));
	////	cornerYOffset = 0;
	////	cornerXOffset = -20 -20;
	//	labelX.AppendChar(U'*');

	//}
	//	

#ifdef ORIGINAL
	labelPosition.Y = PointB.Y + fg.offsetLabelY*fg.cellsize +cornerYOffset;
	labelPosition.X = PointB.X + fg.offsetLabelX*fg.cellsize +cornerXOffset;
#else
	labelPosition.Y = PointB.Y + fg.offsetLabelY*fg.cellHeightY + cornerYOffset;
	labelPosition.X = PointB.X + fg.offsetLabelX*fg.cellLengthX + cornerXOffset;

#endif

	//generateGraphOffsets(fg.height, fg.width, fg.cellsize, fg.offsetX, fg.offsetY, fg.scaleX, fg.scaleY, line, PointA, PointB, label);
}

//UNUSED
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::generateGraphOffsetsFromID2(int32 id, FVector4 line, FVector2D &PointA, FVector2D &PointB, FString & labelX, FString &labelY, bool & isLabelX, FVector2D & labelPosition, bool & isOriginLine)
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

	//UE_LOG(LogTemp, Warning, TEXT("START OFFSETS"));


	float ylevel = ((fg.height - line.Y) / fg.scaleY) + fg.rangeOffsetY;
	float xlevel = ((line.X) / fg.scaleX) + fg.rangeOffsetX;

	ylevel = roundf(ylevel * 100) / 100;
	xlevel = roundf(xlevel * 100) / 100;
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::Fromf());
	//if the position 

	//X Y Z W = X Y Z=X2 W=Y2
	if (line.Y == 0 && line.Z != 0 || line.X == 0 && line.W == fg.height) //X label
	{
		labelX = FString::SanitizeFloat(xlevel);
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
		labelY = FString::SanitizeFloat(ylevel);
		//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::FromInt(line.Y));
		//	labelY = FString::FromInt((int32)((line.Y)));
		isLabelX = false;
		if (labelY == "0")
			isOriginLine = true;


	}

#ifdef LOGIT
	//if (ylevel == 40 )
	//{
	FString f = FString::Printf(TEXT("TYPE:%s, xlev:%d ylev:%d posx:%f posy:%f"), isLabelX ? TEXT("XLABEL") : TEXT("YLABEL"), xlevel, ylevel, labelPosition.X, labelPosition.Y);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *f);
	//}
#endif

	//to prevent overlapp at the LL corner of text
	float cornerYOffset = 0;
	float cornerXOffset = 0;



	//is this the LL corner of Y
	if (ylevel == fg.minRangeY && !isLabelX)
	{
		cornerYOffset = 0;
		cornerXOffset = -25;
		//just get rig of the label
		//	labelY = "";
	}

	//if(line.X==line.Y==0 && isLabelX) 
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::FromInt(xlevel));
	////	cornerYOffset = 0;
	////	cornerXOffset = -20 -20;
	//	labelX.AppendChar(U'*');

	//}
	//	


	labelPosition.Y = PointB.Y + fg.offsetLabelY*fg.cellsize + cornerYOffset;
	labelPosition.X = PointB.X + fg.offsetLabelX*fg.cellsize + cornerXOffset;


	//generateGraphOffsets(fg.height, fg.width, fg.cellsize, fg.offsetX, fg.offsetY, fg.scaleX, fg.scaleY, line, PointA, PointB, label);
}




UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
bool UMyBlueprintFunctionLibrary::translateGraphPoint(int32 id, float markerSize, FVector2D PointIn, FVector2D &PointOut)
{

	FGraphData * pFg = &gGraphs[id];

	//below, windowed offset
	//float woffsetX = FMath::Abs(pFg->maxRangeX - pFg->minRangeX)*pFg->cellLengthX;
	//float woffsetY = FMath::Abs(pFg->maxRangeY - pFg->minRangeY)* pFg->cellHeightY;

	//UE_LOG(LogTemp, Warning, TEXT("X:%f < minrangeX:%f, X:%f > maxrangeX:%f, Y:%f < minrangeY:%f, Y:%f > maxrangeY:%f"), PointIn.X, pFg->minRangeX, PointIn.X, pFg->maxRangeX, PointIn.Y, pFg->maxRangeY, PointIn.Y, pFg->minRangeY);


	//get rid of anything not viewable, ie outside the graph area


	//4.7
	//height doesn't need multiplier since it is already in screen units.
	PointOut.X = (PointIn.X)*pFg->cellLengthX / pFg->scaleX + pFg->offsetX*pFg->cellLengthX - pFg->cellLengthX*pFg->minRangeX/pFg->scaleX;
	PointOut.Y = (pFg->height + pFg->minRangeY*pFg->cellHeightY/pFg->scaleY) - PointIn.Y*pFg->cellHeightY/pFg->scaleY;
	
	
	
	
	// -pFg->cellHeightY*pFg->minRangeY;// +pFg->offsetY*pFg->cellHeightY;


	if (!(PointIn.X < pFg->minRangeX || (PointIn.X> pFg->maxRangeX) || (PointIn.Y > pFg->maxRangeY) || (PointIn.Y < pFg->minRangeY)))
	{
		//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString("Out of bounds"));




	//	UE_LOG(LogTemp, Warning, TEXT("INX:%f INY:%f OUTX:%f OUTY:%f"), PointIn.X, PointIn.Y, PointOut.X, PointOut.Y);
		return true;
	}
	else
	{
		return false;
	}

	//MOTHBALL all the below
	/* 
	FGraphData fg = gGraphs[id];


//	PointOut.X = (PointIn.X) * fg.scaleX + fg.offsetX - markerSize / 2+FMath::Abs(fg.rangeOffsetX*fg.scaleX); //need the abs, not sure why
	PointOut.X = (PointIn.X) * fg.scaleX + fg.offsetX - markerSize / 2 + fg.rangeOffsetX*fg.scaleX*-1; //need the -, not sure why
	PointOut.Y = (fg.height - PointIn.Y* fg.scaleY) + fg.offsetY - markerSize / 2 + fg.rangeOffsetY*fg.scaleY;
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::FromInt(PointIn.Y));
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::FromInt(PointIn.X));
	//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::FromInt(fg.height));
	*/
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


	case EGraphParam::MIN_RANGE_X:
		ifdo(pFg->minRangeX);
		break;
	case EGraphParam::MAX_RANGE_X:
		ifdo(pFg->maxRangeX);
		break;
	case EGraphParam::MIN_RANGE_Y:
		ifdo(pFg->minRangeY);
		break;
	case EGraphParam::MAX_RANGE_Y:
		ifdo(pFg->maxRangeY);
		break;

	default:
		break;
	}

	//run the routines from createGraph to recalc the axis in case a range has changed	
	//recalcRanges( handle);
	//end routines.  Likely unnecessary for most changes, but doesnt hurt to have these there. 

	//TODO...change these since we use pointsets now.
	//recalcGraphTranslatedPoints(handle);
	//clipTranslatedPoints(handle);
}
UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::recalcGraphTranslatedPoints(int32 handle)
{
	FGraphData *p = &gGraphs[handle];
	FVector2D fv;

	p->TranslatedPoints.Empty();
	for (auto a : gGraphs[handle].RawPoints)
	{

		if (translateGraphPoint(handle, p->defaultMarkerSize, a, fv))
		{
			p->TranslatedPoints.Add(fv);
		}
		{
			p->TranslatedPoints.Add(fv);

		}
	}








}

UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
void UMyBlueprintFunctionLibrary::returnPointSetTranslatedPoints(int32 graphHandle, int32 pointSetHandle, TArray<FVector2D> &translatedPoints, FVector2D &markerSize,
	bool & isOverFlowUp, bool & isOverFlowDown, bool& isOverFlowLeft, bool & isOverFlowRight)
{
	FVector2D pout;
	//EOverflowDirection ev = EOverflowDirection::NONE;
	FGraphData * pFg = &gGraphs[graphHandle];


	translatedPoints.Empty();

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool stuffLastPoint = false;
	FVector2D lastPoint;
	//			overflowDirection.Empty();
		/*	LogTemp:Warning: X:20.200005 maxRange : 20.000000
				LogTemp : Warning : X : 27.200031 maxRange : 25.000000
				LogTemp : Warning : X : 31.200047 maxRange : 30.000000
				LogTemp : Warning : X : 38.800076 maxRange : 35.000000*/

	for (FVector2D p : gPointSets[pointSetHandle])
	{



		//test for overflow


		if (p.X < pFg->minRangeX&&!left)
		{
			//	overflowDirection=EOverflowDirection::LEFT;
			isOverFlowLeft = true;
			left = true;
		}

		if ((p.X> pFg->maxRangeX ) && !right)
		{
			//UE_LOG(LogTemp, Warning, TEXT("X:%f maxRange:%f"), p.X, pFg->maxRangeX);
			isOverFlowRight = true;
			//overflowDirection = EOverflowDirection::RIGHT;
			right = true;



		}

		if (p.Y > pFg->maxRangeY&&!up)
		{
			isOverFlowUp = true;
			//overflowDirection=EOverflowDirection::UP;
			up = true;
		}

		if (p.Y < pFg->minRangeY&&!down)
		{
			isOverFlowDown = true;
			//overflowDirection=EOverflowDirection::DOWN;
			down = true;
		}

		//dont bother to process a point that isn't in view/frame
		//if (up || down || left ||right )
		//{
		//	//return;
		//}


		//problem is that in line drawing mode, it will act as though it didn't drop out and will connect with next in-frame point
		if (translateGraphPoint(graphHandle, gGraphs[graphHandle].defaultMarkerSize, p, pout))
		{

	//		stuffLastPoint = false;
			translatedPoints.Add(pout);
		//	opacity = 1;
		}
		else //point is not in range
		{//stuff the last 
			//if first encountring out of bounds point, save it and set flag
		//	if(!stuffLastPoint)
			//	lastPoint = pout;
			
		//	stuffLastPoint = true;

			translatedPoints.Add(pout);
		//	opacity = 0;
		}


	}

	markerSize.X = pFg->defaultMarkerSize;
	markerSize.Y = markerSize.X;
}

UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
bool UMyBlueprintFunctionLibrary::generateAxisFromID2(int32 id, TArray<FVector4> &lines)
{
	//	const int cellsize = 20;   // 20 pixels wide/high cells. 
	FVector4 fv;
	FGraphData * pFg;

	pFg= &gGraphs[id]; //TODO test for valid key


	//trying
	//float Xrange = fg.maxRangeX - fg.minRangeX;	//float Yrange = fg.maxRangeY - fg.minRangeY;


//	float Xrange = FMath::Abs(pFg->maxRangeX) + FMath::Abs(pFg->minRangeX);

//xrange and yrange are the number of ticks or lines for the axis
	float Xrange = FMath::Abs(pFg->maxRangeX - pFg->minRangeX)/pFg->scaleX;
	float Yrange = FMath::Abs(pFg->maxRangeY - pFg->minRangeY)/pFg->scaleY;


	//assume 10 -0 = 10
	//10 lines after the origin, 11 in total
	int nVXlines = Xrange;
	pFg->cellsize = ceil(pFg->width / (float)nVXlines);

	pFg->cellLengthX = pFg->width / (float)nVXlines;

	//fg.scaleX = (float)nVXlines / fg.width / (float)fg.cellsize; // ||
	int nHYlines = Yrange;
	pFg->cellHeightY = (int)(pFg->height / (float)nHYlines)  ;// = 

	//UE_LOG(LogTemp, Warning, TEXT("nvx:%d nhy:%d xrange:%f yrange:%f, cellLengthX:%d, cellLengthY:%f, width:%f, height:%f"), nVXlines, nHYlines,
//		Xrange, Yrange, pFg->cellLengthX, pFg->cellHeightY, pFg->width, pFg->height);

	int i = 0;
	for (i = 0; i <= nVXlines; i++)// ||
	{

		fv.X = i * pFg->cellLengthX;
		fv.Y = 0;
		fv.Z = i * pFg->cellLengthX;
		fv.W = pFg->height;// gridNum * fg.cellsize <= FMath::Abs(fg.height) ? gridNum * fg.cellsize : fg.height;
//		UE_LOG(LogTemp, Warning, TEXT("->X:%f Y:%f z:%f W:%f"), fv.X, fv.Y, fv.Z, fv.W);
		lines.Add(fv);

	}


	for (i = 0; i <= nHYlines; i++) // =
	{

		//draws frim right to left <----
		fv.X = pFg->width;// i * tempcellsize ; // just width.
		fv.Y = i * pFg->cellHeightY;

		//.Z is where I start relative to Left side...it is the left boundary
		fv.Z = 0; //z= endpoint Y, if endpoint Y=0 then this is along the X AXIS
		fv.W = i*pFg->cellHeightY; //really endpoint.X W=X


		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,  FString::FromInt(fv.Y));


		lines.Add(fv);


	}

//	UE_LOG(LogTemp, Warning, TEXT("****************"));
	return true;


//	int gridNum = (int)((fg.width) / fg.cellsize);
//	int numHlines = fg.height / fg.cellsize;
//
//	//gridum = 1000/100 = 100
//	//numH = 500/100 =  5
////	int i;
//	for (i = 0; i <= gridNum; i++)
//	{
//		//	DrawLine(0, i * cellsize, gridSize * cellsize, i * cellsize);
//		//DRAW HOIZONTAL LINES , X =0 Y increases
//		fv.Z = 0; //z= endpoint Y, if endpoint Y=0 then this is along the X AXIS
//
//				  //if (i*fg.cellsize <= FMath::Abs(fg.height))
//		if (i <= numHlines)
//			fv.W = i*fg.cellsize; //really endpoint.X W=X
//		else
//			goto skip;
//
//		fv.X = gridNum * fg.cellsize; // just width.
//		fv.Y = i * fg.cellsize;
//
//		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,  FString::FromInt(fv.Y));
//
//
//		lines.Add(fv);
//	skip:
//		//DrawLine(i * cellsize, 0, i * cellsize, gridSize * cellsize);
//		//Draw VERTICAL , Y=0 X increases
//		fv.X = i * fg.cellsize;
//		fv.Y = 0;
//		fv.Z = i * fg.cellsize;
//
//		//UE_LOG(LogTemp, Warning, TEXT("X:%f Y:%f z:%f W:%f"), fv.X, fv.Y, fv.Z, fv.W);
//
//
//		fv.W = gridNum * fg.cellsize <= FMath::Abs(fg.height) ? gridNum * fg.cellsize : fg.height;
//
//
//		lines.Add(fv);
//
//
//
//	}

	return true;
}
