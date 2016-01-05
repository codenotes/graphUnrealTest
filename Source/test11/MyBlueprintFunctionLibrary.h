// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */


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



};

	extern std::map<int32, FGraphData> gGraphs;

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

		float rangeX = FMath::Abs(pFg->maxRangeX) + FMath::Abs(pFg->minRangeX);
		float rangeY = FMath::Abs(pFg->maxRangeY) + FMath::Abs(pFg->minRangeY);

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



};

