﻿#include "head.h"

//using namespace std;
using namespace cv;

void findEdgePoint(vector<Point> &left,vector<Point> &right,IplImage* img);//从上到下扫描图像并找出边缘点
void showpoint(vector<Point> p);//显示边缘点
void draw(IplImage* img,Vec4f line);//画出边线
void draw(IplImage* img,vecs vec);

int main()
{
	srand((unsigned) time(NULL));
	IplImage* pImg;
	string filename;
	ifstream reader;
	vector<Point> left,right;
	Vec4f leftLine,rightLine;
	vecs left2,right2;

	reader.open("filelist.txt");		//读取图片路径
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
				//平滑并扫描边缘点
				cvSmooth(pImg,pImg);
				findEdgePoint(left,right,pImg);

				//显示点
				cout<<"Left point:";
				showpoint(left);
				cout<<"Right point:";
				showpoint(right);
				cout<<endl<<endl<<endl;

				left2 = geneticoptimize(left);
				right2 = geneticoptimize(right);

				//拟合并绘制
				//fitLine(Mat(left),leftLine,CV_DIST_L2,0,0.01,0.01);
				//fitLine(Mat(right),rightLine,CV_DIST_L2,0,0.01,0.01);
				draw(pImg,left2);
				draw(pImg,right2);
				cvShowImage("line",pImg);

				cvWaitKey();
				//cvDestroyAllWindows();
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



	for(int y=0;y<height;y+= 10)
	{
		
		uchar* ptr = (uchar*)(img->imageData+y*img->widthStep);
		//扫描左侧边缘
		for(int x=0;x<leftpoint;x++)
		{
			if(ptr[3*x]<10&&ptr[3*x+1]>250&&ptr[3*x+2]<10)
			{
				if(leftpoint-x>80)
				{
					left.clear();
				}
				left.push_back(Point(x,y));
				leftpoint = x;
				break;
			}
		}

		//扫描右侧边缘
		for(int x=width-1;x>rightpoint;x--)
		{
			if(ptr[3*x]<10&&ptr[3*x+1]>250&&ptr[3*x+2]<10)
			{
				if(x-rightpoint>80)
				{
					right.clear();
				}
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
	if(k==0)
	{return;}
	b = line[3]-line[2]*k;
	int x0 = 0;
	int y0 = k*x0+b;
	int x1 = img->width;
	int y1 = k*x1+b;
	cvLine(img,cvPoint(x0,y0),cvPoint(x1,y1),Scalar(0,0,255),3);
}

void draw(IplImage* img,vecs vec)
{
	int width = img->width;
	int x,y1,y2;
	double a = vec.vec[0];
	double b = vec.vec[1];
	double a1 = vec.vec[2];
	double b1 = vec.vec[3];
	double c1 = vec.vec[4];
	double d1 = vec.vec[5]; 
	double r;
	for(x=0;x<width;x++)
	{
		r = -(a1*x*x*x+b1*x*x+c1*x+d1);
		//a*y^2 +b*y +r = 0
		y1 = -b+sqrt(b*b-4*a*r)/(2*a);
		y2 = -b-sqrt(b*b - 4*a*r)/(2*a);
		//y2 = -r/b;
		cvCircle(img,cvPoint(x,y1),1,Scalar(0,0,255));
		cvCircle(img,cvPoint(x,y2),1,Scalar(0,0,255));

	}
}


