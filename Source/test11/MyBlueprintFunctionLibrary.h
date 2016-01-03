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
	DEFAULTMARKERSIZE

};

USTRUCT()
struct FGraphData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
	float height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
	float width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
	int32 cellsize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
	float offsetX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
	float offsetY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
	float scaleX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
	float scaleY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
		float offsetLabelX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
		float offsetLabelY;

	//for negative numbers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
		float rangeOffsetX=0;

	//for negative
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
		float rangeOffsetY=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
		float defaultMarkerSize = 10;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
	TArray<FVector2D> RawPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROS")
	TArray<FVector2D> TranslatedPoints;






};

	extern std::map<int32, FGraphData> gGraphs;

UCLASS()
class TEST11_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "ROS")
		static void setGraphData(int32 handle, FGraphData graphData)
	{
		createGraph(handle, graphData.height, graphData.width, graphData.cellsize, graphData.offsetX, graphData.offsetY,graphData.scaleX,
			 graphData.scaleY, graphData.rangeOffsetX, graphData.rangeOffsetY, graphData.offsetLabelX, graphData.offsetLabelY,
			graphData.defaultMarkerSize);

	}


	UFUNCTION(BlueprintCallable, Category = "ROS")
		static void getGraphData(int32 handle, FGraphData & graphData)
	{

		graphData = gGraphs[handle];


	}




	UFUNCTION(BlueprintCallable, Category = "ROS")
		static void createGraph(int32 id, float height, float width, int32 cellsize, float offsetX, float offsetY, float scaleX,
			float scaleY, float rangeOffsetX = 0.0, float rangeOffsetY = 0.0, float offsetLabelX = 0.0, float offsetLabelY = 0.0, 
			float defaultMarkerSize=10);

	UFUNCTION(BlueprintCallable, Category = "ROS")
		static void isfirstOrSecondFloat(bool isFirst, float first, float second, float &val)
	{

		val = isFirst ? first : second;

	}

	UFUNCTION(BlueprintCallable, Category = "ROS")
		static void addGraphPoint(int32 handle, FVector2D fv)
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


	
	static void clipTranslatedPoints(int32 handle)
	{

		FGraphData *p = & gGraphs[handle];
		int cnt = p->TranslatedPoints.Num();
		FVector2D v;

		FVector2D UL, UR,LL,LR;

		UL = FVector2D(p->offsetX, p->offsetY);
		UR = FVector2D(p->width + p->offsetX, p->offsetY);
		LL = FVector2D( p->offsetX, p->height + p->offsetY);
		LR = FVector2D(p->width + p->offsetX, p->offsetY+p->height);

		
		bool bInRange=false;
	/*	std::ofstream of("c:\\temp\\gd.txt");



		of << "rectangle" << std::endl;
		of << "UL:" << UL.X << " ULY:" << UL.Y << " UR:" << UR.X << " URY:" << UR.Y << std::endl;
		of << "LL:" << LL.X << " LLY:" << LL.Y << " UL:" << LR.X << " LRY:" << LR.Y << std::endl;*/



		for (int i = 0; i < cnt;i++)
		{
			
			v = p->TranslatedPoints[i];

		//	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, v.ToString());

			//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::fr(i));
//			of << "test vec:" << std::endl;

			bInRange = v.X>UL.X && v.X<UR.X &&
				v.Y>UL.Y && v.Y<LL.Y;
			
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

	UFUNCTION(BlueprintCallable, Category = "ROS")
		static void returnGraphTranslatedPoints(int32 handle, TArray<FVector2D> &translatedPoints, FVector2D &markerSize)
	{
		
		translatedPoints = gGraphs[handle].TranslatedPoints;
		markerSize.X = gGraphs[handle].defaultMarkerSize;
		markerSize.Y = markerSize.X;


	}


	UFUNCTION(BlueprintCallable, Category = "ROS")
		static void clearGraphPoints(int32 handle)
	{
		gGraphs[handle].RawPoints.Empty();
		gGraphs[handle].TranslatedPoints.Empty();


	}

	UFUNCTION(BlueprintCallable, Category = "ROS")
		static void createDemoPoints(int32 handle)
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


	UFUNCTION(BlueprintCallable, Category = "ROS")
		static bool generateAxisFromID(int32 id, TArray<FVector4> &lines);


		UFUNCTION(BlueprintCallable, Category = "ROS")
		static bool generateAxis(int32 cellsize, int32 gridNum, float height, TArray<FVector4> &points)
	{
		//	const int cellsize = 20;   // 20 pixels wide/high cells. 
		FVector4 fv;


		int i;
		for (i = 0; i <=gridNum; i++)
		{
			//	DrawLine(0, i * cellsize, gridSize * cellsize, i * cellsize);
			//DRAW HOIZONTAL LINES , X =0 Y increases
			fv.Z = 0;		
			
			if (i*cellsize <=FMath::Abs(height))
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
			
			
			fv.W = gridNum * cellsize<=FMath::Abs(height) ? gridNum * cellsize : height;

			points.Add(fv);



		}
		return true;
	}

		UFUNCTION(BlueprintCallable, Category = "ROS")
			static void generateGraphOffsets(float height, float width, int32 cellsize, float offsetX, float offsetY, float scaleX,
				float scaleY, FVector4 line, FVector2D &PointA, FVector2D &PointB, FString & label)
		{


			PointA.X = offsetX+ line.X;
			PointA.Y = offsetY + line.Y;
			PointB.X = offsetX+ line.Z;
			PointB.Y = offsetY + line.W;			




			if (line.Y == 0 )
			{
				label=FString::FromInt((int32)((line.X/scaleX)));
			}
			else
			{
				label= FString::FromInt((int32)((height - line.Y)/scaleY));

			
			}

		}
	
		UFUNCTION(BlueprintCallable, Category = "ROS")
			static void generateGraphOffsetsFromID(int32 id, FVector4 line, FVector2D &PointA, FVector2D &PointB, FString & labelX,
				FString &labelY, bool & isLabelX, FVector2D & labelPosition, bool & isOriginLine);

		UFUNCTION(BlueprintCallable, Category = "ROS")
			static void translateGraphPoint(int32 id, float markerSize, FVector2D PointIn, FVector2D &PointOut);



		UFUNCTION(BlueprintCallable, Category = "ROS")
			static void switchStringOnBool(bool dofirst, FString first, FString second, FString & stringChosen)
		{

			stringChosen = dofirst ? first : second;
			
		}

		UFUNCTION(BlueprintCallable, Category = "ROS")
			static void switchColorOnBool(bool dofirst, FLinearColor first, FLinearColor second, FLinearColor & color)
		{

			color = dofirst ? first : second;

		}


		UFUNCTION(BlueprintCallable, Category = "ROS")
			static void switchFloatOnBool(bool dofirst, float first, float second, float & val)
		{

			val = dofirst ? first : second;


		}

#define ifdo(X) X= addValetoExisting? X+=value:value

		UFUNCTION(BlueprintCallable, Category = "ROS")
			static void changeGraphParm (int32 handle, EGraphParam eParam, float value, bool addValetoExisting=true)
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

		UFUNCTION(BlueprintCallable, Category = "ROS")
			static void recalcGraphTranslatedPoints(int32 handle)
		{


			FGraphData *p = &gGraphs[handle];
			FVector2D fv;

			p->TranslatedPoints.Empty();
			for (auto a : gGraphs[handle].RawPoints)
			{
				
				translateGraphPoint(handle,p->defaultMarkerSize, a, fv);
				p->TranslatedPoints.Add(fv);
			}
		}


};

