#include"opencv2/opencv.hpp"
#include"iostream"
#include "string.h"
#include "fstream"

#include "string.h"
#include "fstream"

using namespace std;
using namespace cv;

void findEdgePoint(vector<Point> &left,vector<Point> &right,IplImage* img);
void showpoint(vector<Point> p);
void draw(IplImage* img,Vec4f line);

int main()
{
	IplImage* pImg;
	string filename;
	ifstream reader;
	vector<Point> left,right;
	Vec4f leftLine,rightLine;

	reader.open("filelist.txt");
	if(reader.is_open())
	{
		while(reader>>filename)
		{
			left.clear();
			right.clear();
			const char* cp = filename.c_str();
			cout<<filename<<endl;
			if(0 != (pImg=cvLoadImage(cp)))
			{
				cvShowImage("src",pImg);
				findEdgePoint(left,right,pImg);
				//TODO:显示点并拟合直线。
				cout<<"Left point:";
				showpoint(left);
				cout<<"Right point:";
				showpoint(right);
				fitLine(Mat(left),leftLine,CV_DIST_L2,0,0.01,0.01);
				fitLine(Mat(right),rightLine,CV_DIST_L2,0,0.01,0.01);
				//cout<<leftLine[3]<<endl;
				//cvLine(pImg,cvPoint(leftLine[0],leftLine[1]),cvPoint(leftLine[2],leftLine[3]),Scalar(0,0,255));
				//cvLine(pImg,cvPoint(rightLine[0],rightLine[1]),cvPoint(rightLine[2],rightLine[3]),Scalar(0,0,255));
				draw(pImg,leftLine);
				draw(pImg,rightLine);
				cvShowImage("line",pImg);

				cvWaitKey();
				cvDestroyAllWindows();
				cvReleaseImage(&pImg);
			}
			else
				cout<<"cannot open Image."<<endl;
		}
	}
	else
		cout<<"cannot open file"<<endl;

}

void findEdgePoint(vector<Point> &left,vector<Point> &right,IplImage* img)
{
	int height = img->height;
	int width = img->width;
	int leftpoint = width;
	int rightpoint = 0;



	for(int y=0;y<height;y++)
	{
		
		uchar* ptr = (uchar*)(img->imageData+y*img->widthStep);
		for(int x=0;x<leftpoint;x++)
		{
			if(ptr[3*x]<10&&ptr[3*x+1]>250&&ptr[3*x+2]<10)
			{
				//TODO:add point.
				left.push_back(Point(x,y));
				leftpoint = x;
				cout<<leftpoint<<endl;
				break;
			}
		}
		for(int x=width-1;x>rightpoint;x--)
		{
			if(ptr[3*x]<10&&ptr[3*x+1]>250&&ptr[3*x+2]<10)
			{
				right.push_back(Point(x,y));
				rightpoint = x;
				break;
			}
		}
	}
}


void showpoint(vector<Point> p)
{
	int x,y;
	vector<Point>::iterator iter;
	for(iter = p.begin();iter != p.end();iter++)
	{
		x = iter->x;
		y = iter->y;
		cout<<'('<<x<<','<<y<<"), ";
	}
	cout<<endl;
}



void draw(IplImage* img,Vec4f line)
{
	double k,b;
	k = line[1]/line[0];
	b = line[3]-line[2]*k;
	int x0 = 0;
	int y0 = k*x0+b;
	int x1 = img->width;
	int y1 = k*x1+b;
	cvLine(img,cvPoint(x0,y0),cvPoint(x1,y1),Scalar(0,0,255));
	//cout<<"line[0]="<<line[0]<<"line[1]="<<line[1]<<"line[2]="<<line[2]<<"line[3]="<<line[3]<<endl;
	cout<<"x1="<<x1<<"y1="<<y1<<"x0="<<x0<<"y0="<<y0<<endl;
	//cout<<"k="<<k<<" b="<<b<<endl;
}


