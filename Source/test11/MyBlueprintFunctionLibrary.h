// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EOverflowDirection: uint8
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};


UENUM(BlueprintType)
enum class EGraphParam : uint8
{
	SCALE_X,
	SCALE_Y,
	SCALE_X_AND_Y,
	Width,
	Height,
	OFFSET_X,
	OFFSET_Y,
	OFFSET_X_AND_Y,
	OFFSETLABEL_X,
	OFFSETLABEL_Y,
	OFFSETLABEL_XY,
	RANGEOFFSET_X,
	RANGEOFFSET_Y,
	RANGEOFFSET_X_AND_Y,
	DEFAULTMARKERSIZE,
	MIN_RANGE_X,
	MAX_RANGE_X,
	MIN_RANGE_Y,
	MAX_RANGE_Y

};

USTRUCT()
struct FGraphData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	int32 cellsize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float offsetX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float offsetY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float scaleX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float scaleY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
		float offsetLabelX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
		float offsetLabelY;

	//for negative numbers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
		float rangeOffsetX=0;

	//for negative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
		float rangeOffsetY=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
		float defaultMarkerSize = 10;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	TArray<FVector2D> RawPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	TArray<FVector2D> TranslatedPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float widgetHeight=0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float widgetWidth=0.0;

	bool dirty = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float minRangeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float maxRangeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float minRangeY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS_GRAPH")
	float maxRangeY;

	float cellLengthX;
	float cellHeightY;

};

	extern std::map<int32, FGraphData> gGraphs;
	extern std::map<int32, TArray<FVector2D>> gPointSets; 


UCLASS()
class TEST11_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static void setGraphData(int32 handle, FGraphData graphData);


	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static void getGraphData(int32 handle, FGraphData & graphData);


	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static void createGraphWithStructure(int32 handle, FGraphData graphData)
	{
		gGraphs[handle] = graphData;
		recalcRanges(handle);

	}

	static void recalcRanges(int32 handle)
	{

		FGraphData * pFg = &gGraphs[handle];

		//float rangeX = FMath::Abs(pFg->maxRangeX) + FMath::Abs(pFg->minRangeX);
		//float rangeY = FMath::Abs(pFg->maxRangeY) + FMath::Abs(pFg->minRangeY);

		float rangeX = FMath::Abs(pFg->maxRangeX) - FMath::Abs(pFg->minRangeX);
		float rangeY = FMath::Abs(pFg->maxRangeY) - FMath::Abs(pFg->minRangeY);


		pFg->scaleX = pFg->width / rangeX;// scaleX;// height;// scaleX;// fg.height / FMath::Abs(maxRangeX) / 2;
		pFg->scaleY = pFg->height / rangeY;//  scaleY;// width;// scaleY; // / FMath::Abs(maxRangeY) / 2;
		pFg->rangeOffsetX = pFg->minRangeX;
		pFg->rangeOffsetY = pFg->minRangeY;



	}




	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static void createGraph(int32 id, float height, float width, int32 cellsize, float offsetX, float offsetY, float scaleX,
			float scaleY, float rangeOffsetX = 0.0, float rangeOffsetY = 0.0, float offsetLabelX = 0.0, float offsetLabelY = 0.0, 
			float defaultMarkerSize=10,
			float minRangeX=-10,
			float maxRangeX=10,
			float minRangeY=-10,
			float maxRangeY=10
			);

	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static void isfirstOrSecondFloat(bool isFirst, float first, float second, float &val);

	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static void addGraphPoint(int32 handle, FVector2D fv);

	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
	static bool isPointInRect(FVector2D point,float height, float width, float offsetX, float offsetY);
	
	static void clipTranslatedPoints(int32 handle);

	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static void returnGraphTranslatedPoints(int32 handle, TArray<FVector2D> &translatedPoints, FVector2D &markerSize);


	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static void clearGraphPoints(int32 handle);

	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static void createDemoPoints(int32 handle);


	UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static bool generateAxisFromID(int32 id, TArray<FVector4> &lines);


		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
		static bool generateAxis(int32 cellsize, int32 gridNum, float height, TArray<FVector4> &points);

		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void generateGraphOffsets(float height, float width, int32 cellsize, float offsetX, float offsetY, float scaleX,
				float scaleY, FVector4 line, FVector2D &PointA, FVector2D &PointB, FString & label);
	
		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void generateGraphOffsetsFromID(int32 id, FVector4 line, FVector2D &PointA, FVector2D &PointB, FString & labelX,
				FString &labelY, bool & isLabelX, FVector2D & labelPosition, bool & isOriginLine);

		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void translateGraphPoint(int32 id, float markerSize, FVector2D PointIn, FVector2D &PointOut);



		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void switchStringOnBool(bool dofirst, FString first, FString second, FString & stringChosen);

		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void switchColorOnBool(bool dofirst, FLinearColor first, FLinearColor second, FLinearColor & color);


		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void switchFloatOnBool(bool dofirst, float first, float second, float & val);


		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void changeGraphParm (int32 handle, EGraphParam eParam, float value, bool addValuetoExisting=true);

		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void recalcGraphTranslatedPoints(int32 handle);

		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void duplicateGraph(int32 handleFrom, int32 handleTo, int32 & handleNew, bool copyPoints=false)
		{

			gGraphs[handleTo] = gGraphs[handleFrom];
			handleNew = handleTo;

			if (!copyPoints) //empty if not copyPoints=true;
			{

				gGraphs[handleTo].RawPoints.Empty();
				gGraphs[handleTo].TranslatedPoints.Empty();
			}



		}
#define GR gGraphs[handle]
		//put on event tick. will give you geometry and initialize the graph
		//autoSize =true will make graph fill out the entire space
		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void refreshGraph(int32 handle, FVector2D widgetSize, bool autoSize=true)
		{
			if(gGraphs[handle].dirty)
			{
				
				gGraphs[handle].dirty = false;

				gGraphs[handle].widgetHeight = widgetSize.Y;
				gGraphs[handle].widgetWidth = widgetSize.X;

				if (autoSize)
				{
					GR.width=widgetSize.X;
					GR.height=widgetSize.Y;


				}

				recalcGraphTranslatedPoints(handle);
				clipTranslatedPoints(handle);

			}

		}

		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void addPointToSet(int32 handle, FVector2D fv)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::FromInt(-4));

			if (gPointSets.find(handle) == gPointSets.end())
			{
				//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::FromInt(-5));
				gPointSets[handle];
			}

			gPointSets[handle].Add(fv);
			//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,  FString::FromInt(-6));

		}

		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void clearPointSet(int32 handle)
		{

			gPointSets[handle].Empty();

		}


		
		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void returnPointSet(int32 handle, TArray<FVector2D> &points )
		{

			points = gPointSets[handle];
			
		}


		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void returnPointSetTranslatedPoints(int32 graphHandle, int32 pointSetHandle,
				TArray<FVector2D> &translatedPoints, FVector2D &markerSize,
				bool & isOverFlowUp, bool & isOverFlowDown,
				bool& isOverFlowLeft, bool & isOverFlowRight)
				
				//TEnumAsByte<EOverflowDirection> & overflowDirection)
		{
			FVector2D pout;
			//EOverflowDirection ev = EOverflowDirection::NONE;
			FGraphData * pFg = &gGraphs[graphHandle];

			bool up = false;
			bool down = false;
			bool left = false;
			bool right = false;
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
					isOverFlowLeft= true;
					left = true;
				}

				if ((p.X> pFg->maxRangeX) && !right)
				{
				//	UE_LOG(LogTemp, Warning, TEXT("X:%f maxRange:%f"), p.X, pFg->maxRangeX);
					isOverFlowRight = true;
					//overflowDirection = EOverflowDirection::RIGHT;
					right = true;
				//	pFg->maxRangeX += 5;
					//pFg->minRangeX += 5;
				
					/*createGraph(graphHandle,
						pFg->height,
						pFg->width,
						pFg->cellsize,
						pFg->offsetX,
						pFg->offsetY,
						pFg->rangeOffsetX,
						pFg->rangeOffsetY,
						pFg->offsetLabelX,
						pFg->offsetLabelY,
						pFg->defaultMarkerSize,
						pFg->minRangeX,
						pFg->maxRangeX,
						pFg->minRangeX,
						pFg->maxRangeY);
*/

	//				pFg->rangeOffsetX + 5;


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

			
				translateGraphPoint(graphHandle, gGraphs[graphHandle].defaultMarkerSize, p, pout);
				
				translatedPoints.Add(pout);


			}
						
			markerSize.X = gGraphs[graphHandle].defaultMarkerSize;
			markerSize.Y = markerSize.X;
		}

		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static bool generateAxisFromID2(int32 id, TArray<FVector4> &lines)
		{
			//	const int cellsize = 20;   // 20 pixels wide/high cells. 
			FVector4 fv;
			FGraphData fg;

			fg = gGraphs[id]; //TODO test for valid key
			
			float Xrange = fg.maxRangeX - fg.minRangeX;
			float Yrange = fg.maxRangeY - fg.minRangeY;
			
			//assume 10 -0 = 10
			//10 lines after the origin, 11 in total
			int nVXlines = Xrange;
			fg.cellsize = ceil(fg.width / (float)nVXlines);
			
			fg.cellLengthX = fg.width / (float)nVXlines;

			//fg.scaleX = (float)nVXlines / fg.width / (float)fg.cellsize; // ||
			int nHYlines =  Yrange;
			fg.cellHeightY = (int)(fg.height / (float)nHYlines) ;// = //TEMPORARY!

			UE_LOG(LogTemp, Warning, TEXT("nvx:%d nhy:%d xrange:%f yrange:%f, cellsizeX:%d, cellsizeY:%f, width:%f, height:%f"), nVXlines,nHYlines,
				Xrange,Yrange, fg.cellLengthX, fg.cellHeightY,fg.width, fg.height);

			int i = 0;
			for (i = 0; i <= nVXlines; i++)// ||
			{

				fv.X = i * fg.cellLengthX;
				fv.Y = 0;
				fv.Z = i * fg.cellLengthX;
				fv.W = fg.height;// gridNum * fg.cellsize <= FMath::Abs(fg.height) ? gridNum * fg.cellsize : fg.height;
		//		UE_LOG(LogTemp, Warning, TEXT("->X:%f Y:%f z:%f W:%f"), fv.X, fv.Y, fv.Z, fv.W);
				lines.Add(fv);

			}


			for (i = 0; i <=nHYlines; i++) // =
			{

				//draws frim right to left <----
				fv.X = fg.width;// i * tempcellsize ; // just width.
				fv.Y = i * fg.cellHeightY;

				//.Z is where I start relative to Left side...it is the left boundary
				fv.Z = 0; //z= endpoint Y, if endpoint Y=0 then this is along the X AXIS
				fv.W = i*fg.cellHeightY ; //really endpoint.X W=X
		
				
				//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,  FString::FromInt(fv.Y));


				lines.Add(fv);


			}

			UE_LOG(LogTemp, Warning, TEXT("****************"));
			return true;
			
			
			int gridNum = (int)((fg.width) / fg.cellsize);
			int numHlines = fg.height / fg.cellsize;

			//gridum = 1000/100 = 100
			//numH = 500/100 =  5
		//	int i;
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

				//UE_LOG(LogTemp, Warning, TEXT("X:%f Y:%f z:%f W:%f"), fv.X, fv.Y, fv.Z, fv.W);


				fv.W = gridNum * fg.cellsize <= FMath::Abs(fg.height) ? gridNum * fg.cellsize : fg.height;


				lines.Add(fv);



			}

			return true;
		}


		UFUNCTION(BlueprintCallable, Category = "ROS_GRAPH")
			static void generateGraphOffsetsFromID2(int32 id, FVector4 line, FVector2D &PointA, FVector2D &PointB, FString & labelX,
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




};

