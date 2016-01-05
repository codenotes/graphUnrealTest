// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>

int main()
{


	float cellsize = 200;
	float height = 500;
	float width = 800;

	float X, Y, Z, W;

	//FVector4 fv;
	//FGraphData fg;

	//fg = gGraphs[id]; //TODO test for valid key
	int gridNum = (int)(width / cellsize);
	int numHlines = height / cellsize;


	int i;
	for (i = 0; i <= gridNum; i++)
	{
		//	DrawLine(0, i * cellsize, gridSize * cellsize, i * cellsize);
		//DRAW HOIZONTAL LINES , X =0 Y increases
		Z = 0;

		//if (i*fg.cellsize <= FMath::Abs(fg.height))
		if (i <= numHlines)
			W = i*cellsize;
		else
			goto skip;

		X = gridNum * cellsize;
		Y = i * cellsize;

		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,  FString::FromInt(fv.Y));


		printf("HORZ: startx:%d starty:%d endX:%d endY:%d\n", (int)X, (int)Y, (int)Z, (int)W);
		//	lines.Add(fv);
	skip:
		//DrawLine(i * cellsize, 0, i * cellsize, gridSize * cellsize);
		//Draw VERTICAL , Y=0 X increases
		X = i * cellsize;
		Y = 0;
		Z = i * cellsize;


		W = gridNum * cellsize <= fabs(height) ? gridNum * cellsize : height;


		printf("VERT: startx:%d starty:%d endX:%d endY:%d\n", (int)X, (int)Y, (int)Z, (int)W);
		//		lines.Add(fv);



	}





    return 0;
}

