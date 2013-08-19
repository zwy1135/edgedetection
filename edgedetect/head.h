#include"opencv2/opencv.hpp"
#include"iostream"
#include "string.h"
#include "fstream"

#include "math.h"
#include "stdlib.h"
#include "time.h"
#include "algorithm"
using namespace std;

struct vecs
{
	double cost;
	double vec[6];
};

double calError(double vec[],vector<cv::Point> points);
vecs geneticoptimize(vector<cv::Point> points,int popsize = 100,double step = 50,double mutprob = 0.2,double elite = 0.2,int maxiter = 100000);
vecs mutate(vecs vec,double step,vector<cv::Point> points);
vecs crossover(vecs vec1,vecs vec2,vector<cv::Point> points);
double randf();
bool mycmp(vecs vec1,vecs vec2);
