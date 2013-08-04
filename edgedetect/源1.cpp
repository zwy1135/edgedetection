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

int main()
{
	IplImage* pImg;
	string filename;
	ifstream reader;
	vector<Point> left,right;

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
	int leftpoint;
	int rightpoint;


	for(int y=0;y<height;y+=10)
	{
		leftpoint = width;
		rightpoint = 0;
		uchar* ptr = (uchar*)(img->imageData+y*img->widthStep);
		for(int x=0;x<leftpoint;x++)
		{
			if(ptr[3*x]<10&&ptr[3*x+1]>250&&ptr[3*x+2]<10)
			{
				//TODO:add point.
				left.push_back(Point(x,y));
				leftpoint = x;
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
