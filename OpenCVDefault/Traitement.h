#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <list>
#include <vector>


using namespace std;
using namespace cv;

class CTraitement
{

public:	
	static void DrawCross(cv::Mat,Point, Vec3b);
	static Mat ConvertToGray(cv::Mat);
	static void FindEdges(Mat, int*);
	static void CreateDiagram(int rownumber, Mat frame);
	static vector<Point> FindRectangle(Mat frame);	
	static void CTraitement::DrawEdges(Mat frame , int * edges);
	static list<Point> GetContouring(int middleX, int middleY, Mat frame);
	static void DrawContouring(Mat frame, list<Point> borderPoints);
	static void FindAngleAndDrawCrosses(Mat frame, vector<Point> corners);
	static Mat Sobelize(Mat frame, int edgeLevel);
};

