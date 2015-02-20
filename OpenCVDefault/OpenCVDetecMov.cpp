#include "stdafx.h"
#include "device_launch_parameters.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include "Traitement.h"

using namespace std;
using namespace cv;

Mat frameSnapshot;
Vec3b bgrPixel;
bool SnapshotMode;
int MouseX;
int MouseY;
bool hKeyPressed;
Vec3b currentPixColor;
Mat frame;

#define ENTER_KEY 13
#define TAB_KEY 9
#define R_KEY 114
#define C_KEY 99
#define H_KEY 104
#define S_KEY 115
#define T_KEY 116
#define F_KEY 102
#define G_KEY 103
#define ESCAPE_KEY 27
#define SPACE_KEY 32

bool is_file_exist(string fileName)
{
	ifstream infile(fileName.c_str());
	return infile.good();
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_RBUTTONDOWN && SnapshotMode) //Right click
	{
		bgrPixel = frameSnapshot.at<Vec3b>(Point(x, y));
		printf("Couleur sauvegardee\n");				
	}
	else if  ( event == EVENT_LBUTTONDOWN && SnapshotMode) //Left click
	{
		if(hKeyPressed)
		{
			if(frameSnapshot.channels() == 3)
			{
				frameSnapshot =CTraitement::ConvertToGray(frameSnapshot);
			}
			CTraitement::CreateDiagram(MouseY,frameSnapshot);
			hKeyPressed=false;
		}
		else
		{
			CTraitement::DrawCross(frameSnapshot,Point(x,y),bgrPixel);
		}		
		
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);
		imwrite("snapshot.png", frameSnapshot, compression_params);
		
		
	}
	else if (event == EVENT_MOUSEMOVE) //Mouse move
	{
		if (x >= 0 && x < frame.cols && y >= 0 && y < frame.rows)
		{
			MouseX = x;
			MouseY = y;
			currentPixColor = frame.at<Vec3b>(Point(x, y));
		}
	}
}

int main(int argc, char** argv)
{
	MouseX = 0;
	MouseY = 0;
	Mat edges;
	SnapshotMode = false;
	hKeyPressed=false;
	bgrPixel = (0,0,0);
	currentPixColor = (0,0,0);
	
		edges = imread("breadfish.png", CV_LOAD_IMAGE_COLOR);

		imshow("edges", edges);

		VideoCapture cap(0); // open the default camera
		if(!cap.isOpened())  // check if we succeeded
			return -1;
		frameSnapshot = Mat(480,640,CV_8UC3);
		frameSnapshot.setTo(0);
		frame = Mat(480,640,CV_8UC3);
		frame.setTo(0);
		int key=0;
		string path;
		bool fileLoaded=false;
		char Message[200];
		namedWindow("frame", 1);
		setMouseCallback("frame", CallBackFunc, NULL);

		while(key != ESCAPE_KEY)
		{
			cap >> frame; // get a new frame from camera
			memset(Message,0,200);
			sprintf(Message,"(%i, %i) = (%i, %i, %i)", MouseX, MouseY, currentPixColor[2], currentPixColor[1], currentPixColor[0]); //text out

			key = waitKey(30);

			if(key==F_KEY)
			{
				cout << "Veuillez entrer le nom du fichier que vous desirez charger. (nom.extension)" << endl;
				getline(cin, path);
				if (is_file_exist(path))
				{
					frameSnapshot = imread(path, CV_LOAD_IMAGE_COLOR);
					cout << "Le fichier a ete charge." << endl;
					fileLoaded = true;
				}
				else
				{
					cout << "Le fichier na pas ete trouve." << endl;
					path="";
				}
			}

			if(key==ENTER_KEY)
			{
				vector<int> compression_params;
				compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
				compression_params.push_back(9);
				imwrite("snapshot.png", frame, compression_params);
				frame.copyTo(frameSnapshot);
			}	

			if (key==TAB_KEY || key==ENTER_KEY || (key==F_KEY && fileLoaded))
			{
				SnapshotMode = true;
				key=0;
				//mode image
				while(key != TAB_KEY) 
				{
					key = waitKey(30);

					if(key==R_KEY)
					{
						line(frameSnapshot, Point(0,0), Point(frameSnapshot.cols, frameSnapshot.rows), cv::Scalar(0, 0, 200), 1, 1); 
					}
					if(key==S_KEY)
					{
						if(frameSnapshot.channels() == 3)
						{
							frameSnapshot =CTraitement::ConvertToGray(frameSnapshot);
						}
						frameSnapshot = CTraitement::Sobelize(frameSnapshot, 8);
					}

					if(key==SPACE_KEY)
					{
						frameSnapshot.setTo(0);
					}

					if(key==G_KEY)
					{
						frameSnapshot = CTraitement::ConvertToGray(frameSnapshot);
					}

					if(key==C_KEY)
					{
						if(frameSnapshot.channels() == 3)
						{
							frameSnapshot =CTraitement::ConvertToGray(frameSnapshot);
						}
						int edges[4];
						CTraitement::FindEdges(frameSnapshot, edges);
						CTraitement::DrawEdges(frameSnapshot,edges);
						Point centerPoint = Point(edges[0]+((edges[1]-edges[0])/2),edges[2]+((edges[3] - edges[2])/2));

						//left,right,top,bottom
						CTraitement::DrawCross(frameSnapshot,centerPoint,Vec3b(255,255,255));						
					}

					if(key==H_KEY)
					{
						hKeyPressed=true;
					}

					if (key==T_KEY)
					{
						if(frameSnapshot.channels() == 3)
						{
							frameSnapshot =CTraitement::ConvertToGray(frameSnapshot);
						}
						vector<Point> corners = CTraitement::FindRectangle(frameSnapshot);						
						CTraitement::FindAngleAndDrawCrosses(frameSnapshot, corners);
					}

					imshow("frame", frameSnapshot);
				}
				SnapshotMode = false;
			}
			putText(frame, Message, Point(10,450), CV_FONT_HERSHEY_COMPLEX, 1,  cv::Scalar(0,0,200));
			imshow("frame", frame);
			
		}
		
		return 0;
}



