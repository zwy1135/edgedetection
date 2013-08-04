#include "opencv2/opencv.hpp"
#include "iostream"
#include "string.h"
#include "fstream"

using namespace std;
using namespace cv;

IplImage* set_color(IplImage* img);
void findLines(IplImage* buf,IplImage* src);
void findedge(IplImage* buf,IplImage* src);

int main()
{
	IplImage* pImg = NULL;
	IplImage* buf = NULL;
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
				cout<<pImg->origin<<endl;
				buf = set_color(pImg);
				greenImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,1);
				cvSetImageCOI(buf,2);
				cvCopy(buf,greenImg);
				cvResetImageROI(buf);
				cvSmooth(greenImg,greenImg,CV_GAUSSIAN,17);
				pCannyImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,1);
				cvCanny(greenImg,pCannyImg,50,154,3);
				findedge(pCannyImg,pImg);
				//cvShowImage("green",pCannyImg);
				//cvNamedWindow("src",1);
				//cvNamedWindow("edge",1);
				
				cvWaitKey(0);
				
				cvDestroyAllWindows();
				//cvDestroyWindow("src");
				//cvDestroyWindow("edge");
				cvReleaseImage(&pImg);
				cvReleaseImage(&buf);
				cvReleaseImage(&greenImg);
				cvReleaseImage(&pCannyImg);
			}
		}
	}
	 
}

IplImage* set_color(IplImage* img)
{
	IplImage* buf = cvCloneImage(img);
	CvSize size = cvGetSize(img);
	int height = size.height;
	int width = size.width;
	for(int y=0;y<height;y++)
	{
		 uchar* ptr = (uchar*) (buf->imageData + y * buf->widthStep);
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
	return buf;
}

void findedge(IplImage* buf,IplImage* src)
{
	CvMemStorage* stor = cvCreateMemStorage(0);
	CvSeq* edge;
	cvFindContours(buf,stor,&edge,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	cvDrawContours(src,edge,CV_RGB(255,0,0),CV_RGB(0,0,0),0,CV_FILLED);
	/*for(int i = 0; i < MIN(edge->total,50); i++ )
	{
        CvPoint* line = (CvPoint*)cvGetSeqElem(edge,i);
        cvLine( src, line[0], line[1], CV_RGB(255,0,0), 1, 8, 0 );
     }*/
	cvShowImage("edge",src);
}

void findLines(IplImage* buf,IplImage* src)
{
	CvMemStorage* stor = cvCreateMemStorage(0);
	CvSeq* lines;
	lines = cvHoughLines2(buf,stor,CV_HOUGH_PROBABILISTIC,1,CV_PI/180,70,100,200);
	for(int i = 0; i < MIN(lines->total,50); i++ )
	{
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
        cvLine( src, line[0], line[1], CV_RGB(255,0,0), 1, 8, 0 );
     }
	cvShowImage("edge",src);
//	vector<Vec4i> lines;
//	Mat Mbuf = Mat(buf);
//	HoughLinesP(Mbuf,lines,1,CV_PI/180,30,10,101);
//	//cvHoughLines2(buf,lines,CV_HOUGH_PROBABILISTIC,1,CV_PI/180,80,30,100);
//	vector<Vec4i>::const_iterator it = lines.begin();
//	while(it!=lines.end())
//	{
//		Point pt1((*it)[0],(*it)[1]);
//		Point pt2((*it)[2],(*it)[3]);
//		cvLine(buf,pt1,pt1,Scalar(0,0,255));
//	}
//	cvShowImage("edge",buf);
}