#include "StdAfx.h"
#include "Traitement.h"

void CTraitement::DrawCross(Mat frame, Point p, Vec3b pixcolor)
{
	if(p.x >= 0 && p.y >= 0)
	{
		line(frame, Point(p.x-10, p.y), Point(p.x+10, p.y), Scalar(pixcolor[0], pixcolor[1], pixcolor[2]), 3,1);
		line(frame, Point(p.x,p.y-10), Point(p.x, p.y+10), Scalar(pixcolor[0], pixcolor[1], pixcolor[2]),3,1);
	}
	
}

Mat CTraitement::ConvertToGray(Mat frame)
{
	cv::Mat greyMat;
	cv::cvtColor(frame, greyMat, CV_BGR2GRAY);
	return greyMat;
}

void CTraitement::CreateDiagram(int rownumber, Mat frame)
{
	
	Mat Diagram(480,640,CV_8U);
	Diagram.setTo(Scalar(255));
	int xAxis=40,yAxis=460;
	Point previousPoint, currentPoint;

	line(Diagram, Point(xAxis, 20), Point(xAxis, 460), Scalar(0), 2,1);
	line(Diagram, Point(40, yAxis), Point(620, yAxis), Scalar(0), 2,1);

	line(Diagram, Point(36, 374), Point(44, 374), Scalar(0), 2,1);
	line(Diagram, Point(36, 287), Point(44, 287), Scalar(0), 2,1);
	line(Diagram, Point(36, 201), Point(44, 201), Scalar(0), 2,1);
	line(Diagram, Point(36, 115), Point(44, 115), Scalar(0), 2,1);
	line(Diagram, Point(36, 29), Point(44, 29), Scalar(0), 2,1);

	putText(Diagram, "0", Point(5,465), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));
	putText(Diagram, "50", Point(5,379), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));
	putText(Diagram, "100", Point(5,292), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));
	putText(Diagram, "150", Point(5,206), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));
	putText(Diagram, "200", Point(5,120), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));
	putText(Diagram, "250", Point(5,34), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));


	//1,7254901960784313725490196078431 = 1 brightness
	//0 = 460, 50=374, 100=287, 150=201, 200=115, 250=29
	int yPixel, xPixel;
	int pixValue;
	int maxValue=0,minValue=999999999;
	
	for(int i=0; i<frame.cols;i++)
	{
		pixValue = frame.data[currentPoint.y*frame.cols + currentPoint.x]; //0-255
		if (pixValue > maxValue)
		{
			maxValue = pixValue;
		}
		if (pixValue < minValue)
		{
			minValue = pixValue;
		}		
		yPixel = yAxis - (1.7254901960784313725490196078431 * pixValue);
		xPixel = xAxis + (((double)i/frame.cols))*580.0;

		currentPoint = Point(xPixel, yPixel);
		if (i>1)
		{
			
			line(Diagram, previousPoint, currentPoint, Scalar(0), 1,1);
		}
		previousPoint = currentPoint;
	}

	int midValue = minValue+((maxValue-minValue)/2.0);
	yPixel = yAxis - (1.7254901960784313725490196078431 * (midValue));
	line(Diagram, Point(40, yPixel), Point(620, yPixel), Scalar(0), 1,1);
	char msg[10];
	memset(msg,0,10);
	sprintf(msg,"%i",midValue);
	putText(Diagram, msg, Point(580,yPixel-5), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,200));
	
	imshow("Diagramme",Diagram);
}

void CTraitement::DrawEdges(Mat frame , int * edges)
{
	int left = edges[0]; //x
	int right = edges[1]; //x
	int top = edges[2]; //y
	int bottom = edges[3]; //y

	line(frame, Point(0, top), Point(frame.cols-1, top), Scalar(0, 0, 0), 1,1);
	line(frame, Point(0, bottom), Point(frame.cols-1, bottom), Scalar(0, 0, 0),1,1);
	line(frame, Point(left, 0), Point(left, frame.rows -1), Scalar(0, 0, 0), 1,1);
	line(frame, Point(right, 0), Point(right, frame.rows -1), Scalar(0, 0, 0),1,1);	
}

void CTraitement::FindEdges(Mat gray, int* edges)
{	
	int* LinesSum = new int[gray.rows];
	int* ColsSum = new int[gray.cols];

	//init arrays
	for (int i=0; i<gray.rows; i++)
	{
		LinesSum[i]=0;
	}
	for (int i=0; i<gray.cols; i++)
	{
		ColsSum[i]=0;
	}
	
	for (int i=0; i<gray.rows; i++)
	{
		for (int j=0; j<gray.cols; j++)
		{
			LinesSum[i]+=gray.data[i*gray.cols + j];
		}
	}

	for (int i=0; i<gray.cols; i++)
	{
		for (int j=0; j<gray.rows; j++)
		{
			ColsSum[i]+=gray.data[j*gray.cols + i];
		}
	}

	int thresholdA = 500;
	
	int left=-1,right=-1,top=-1,bottom=-1;
	int i=1;
	while (i<gray.cols && left == -1)
	{	
		if (abs(ColsSum[i]-ColsSum[i-1]) > thresholdA)
		{
			left = i;
		}
		i++;
	}

	i = gray.cols -1;
	while (i>0 && right == -1)
	{
		if (abs(ColsSum[i]-ColsSum[i-1]) > (thresholdA))
		{
			right = i;
		}
		i--;
	}

	i=1;
	while (i<gray.rows && top == -1)
	{
		if (abs(LinesSum[i]-LinesSum[i-1]) >(thresholdA))
		{
			top = i;
		}
		i++;
	}

	i = gray.rows -1;
	while (i>0 && bottom == -1)
	{
		if (abs(LinesSum[i]-LinesSum[i-1]) > (thresholdA))
		{
			bottom = i;
		}
		i--;
	}
	
	delete LinesSum;
	delete ColsSum;

	edges[0] = left;
	edges[1] = right;
	edges[2] = top;
	edges[3] = bottom;
}

int GetPixelBrightness(Mat frame, Point position)
{
	return frame.at<uchar>(Point(position.x, position.y));
}

vector<Point> CTraitement::FindRectangle(Mat frame)
{
	Point pointTop, pointLeft, pointRight, pointBottom;
	bool topCornerFound = false, leftCornerFound = false, rightCornerFound = false, bottomCornerFound = false ;
	bool startObjectFound;
	int x,y;
	int blackThreshold = 20;
	Point currentPoint;
	vector<Point> corners;

	//search top point
	y=0;
	while (y < frame.rows && !topCornerFound)
	{
		x=0;
		while (x < frame.cols && !topCornerFound)
		{
			currentPoint = Point(x,y);
			if (GetPixelBrightness(frame, currentPoint) < blackThreshold)
			{
				topCornerFound = true;
				pointTop = currentPoint;
			}
			x++;
		}
		y++;
	}
	
	//search left point
	x=0;
	startObjectFound=false;
	while (x < frame.cols && !leftCornerFound)
	{
		y=0;
		while (y < frame.rows && !leftCornerFound)
		{
			currentPoint = Point(x,y);
			if (GetPixelBrightness(frame, currentPoint) < blackThreshold)
			{				
				startObjectFound=true;				
			}
			if(startObjectFound && GetPixelBrightness(frame, currentPoint) >= blackThreshold)
			{
				leftCornerFound = true;
				pointLeft = currentPoint;
			}
			y++;
		}
		x++;
	}

	//search bottom point
	y=frame.rows-1;
	while (y >= 0 && !bottomCornerFound)
	{
		x=frame.cols-1;
		while (x >= 0 && !bottomCornerFound)
		{
			currentPoint = Point(x,y);
			if (GetPixelBrightness(frame, currentPoint) < blackThreshold)
			{
				bottomCornerFound = true;
				pointBottom = currentPoint;
			}
			x--;
		}
		y--;
	}

	//search right point
	x=frame.cols-1;
	startObjectFound=false;
	while (x >= 0 && !rightCornerFound)
	{
		y=frame.rows-1;
		while (y >= 0 && !rightCornerFound)
		{
			currentPoint = Point(x,y);
			if (GetPixelBrightness(frame, currentPoint) < blackThreshold)
			{				
				startObjectFound=true;				
			}
			if(startObjectFound && GetPixelBrightness(frame, currentPoint) >= blackThreshold)
			{
				rightCornerFound = true;
				pointRight = currentPoint;
			}
			y--;
		}
		x--;
	}

	if(topCornerFound && bottomCornerFound && leftCornerFound && rightCornerFound)
	{
		//top,left,bottom,right
		corners.push_back(pointTop);
		corners.push_back(pointLeft);
		corners.push_back(pointBottom);
		corners.push_back(pointRight);
	}
	return corners;
}

void CTraitement::FindAngleAndDrawCrosses(Mat frame, vector<Point> corners)
{
	Point pointTop, pointLeft, pointRight, pointBottom;
	if(corners.size() == 4)
	{
		for (int i =0;i<4;i++)
		{
			DrawCross(frame,corners.at(i),0);
		}

		//top,left,bottom,right
		pointTop = corners.at(0);
		pointLeft = corners.at(1);
		
		double dy = pointLeft.y - pointTop.y;
		double dx = pointTop.x - pointLeft.x;
		
		double angle;

		angle = (pointLeft != pointTop) ? atan2((double)(pointLeft.x - pointTop.x), (double)(pointLeft.y - pointTop.y))* 180.0 / 3.1415926535 : 90;
		
		char msg[20];
		sprintf(msg,"Angle = %i deg",(int)angle);
		putText(frame, msg,Point(500,540), CV_FONT_HERSHEY_COMPLEX, 0.5,  Scalar(0,0,0));
	}
}

Mat CTraitement::Sobelize(Mat frame, int edge)
{
		Mat result = Mat(frame.rows,frame.cols, CV_8U);		
		int color;
		int colorTop;
		int colorBack;
		edge = 8;

		for (int col = 1; col<result.cols-1;col++)
		{
			for (int row = 1; row<result.rows-1;row++)
			{
				color = frame.at<uchar>(row,col);
				colorTop = frame.at<uchar>(row,col-1);
				colorBack = frame.at<uchar>(row-1,col);

				if(abs(color - colorTop) > edge ||abs(color - colorBack) > edge)
				{
					result.at<uchar>(Point(col,row)) = 255;
				}
				else
				{
					result.at<uchar>(Point(col,row)) = 0;
				}
			}
		}
		return result;
}