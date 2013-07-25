#include "opencv2/opencv.hpp"
#include "iostream"
#include "string.h"
#include "fstream"

using namespace std;
using namespace cv;

void set_color(IplImage* img);

int main()
{
	IplImage* pImg = NULL;
	IplImage* greenImg = NULL;
	IplImage* pCannyImg = NULL;
	string filename;
	ifstream reader;
	
	
	reader.open("filelist.txt");
	if(reader.is_open())
	{	
		while(reader>>filename)
		{
			const char *cp=filename.c_str();
			cout<<filename<<endl;
			if(0 != (pImg = cvLoadImage(cp,1)))
			{
				cvShowImage("src",pImg);
				set_color(pImg);
				greenImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,1);
				cvSetImageCOI(pImg,2);
				cvCopy(pImg,greenImg);
				cvResetImageROI(pImg);
				pCannyImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,1);
				cvCanny(greenImg,pCannyImg,50,154,3);
				
				cvShowImage("edge",pCannyImg);
				//cvNamedWindow("src",1);
				//cvNamedWindow("edge",1);
				
				cvWaitKey(0);
				
				cvDestroyWindow("src");
				cvDestroyWindow("edge");
				cvReleaseImage(&pImg);
				cvReleaseImage(&greenImg);
				cvReleaseImage(&pCannyImg);
			}
		}
	}
	 
}

void set_color(IplImage* img)
{
	CvSize size = cvGetSize(img);
	int height = size.height;
	int width = size.width;
	for(int y=0;y<height;y++)
	{
		 uchar* ptr = (uchar*) (img->imageData + y * img->widthStep);
		 for(int x=0;x<width;x++)
		 {
			 if(!(ptr[3*x]<10&&ptr[3*x+1]>250&&ptr[3*x+2]<10))
			 {
				 ptr[3*x]=0;
				 ptr[3*x+1]=0;
				 ptr[3*x+2]=0;
			 }
		 }
	}
}