#include"head.h"

//using namespace std;
//using namespace cv;

double calError(double vec[],vector<cv::Point> points)
{
	double sum = 0.0;
	double x,y;
	double r,y1,y2;
	//a*y^2+b*y = a1*x^3+b1*x^2+c1*x+d1
	double a = vec[0];
	double b = vec[1];
	double a1 = vec[2];
	double b1 = vec[3];
	double c1 = vec[4];
	double d1 = vec[6];

	vector<cv::Point>::iterator iter;
	for(iter = points.begin();iter != points.end();iter++)
	{
		x = iter->x;
		y = iter->y;
		r = -(a1*x*x*x+b1*x*x+c1*x+d1);
		//a*y^2 +b*y +r = 0
		y1 = -b+sqrt(b*b-4*a*r)/(2*a);
		y2 = -b-sqrt(b*b - 4*a*r)/(2*a);
		//y1 = -r/b;
		//y2 = -r/b;
		if(fabs(y-y1)<fabs(y-y2))
		{
			sum += fabs(y-y1)*fabs(y-y1);
		}
		else
		{
			sum += fabs(y-y2)*fabs(y-y2);
		}
		//sum += fabs(y-y2)*fabs(y-y2);
	}
	return sqrt(sum);
}



vecs geneticoptimize(vector<cv::Point> points,int popsize,double step,double mutprob,double elite,int maxiter)
{
	
	vector<vecs> pop;
	vecs vec;
	// firstvec;
	int samecount = 0;
	double oldscore = 9999999999;
	for(int i = 0;i<popsize;i++)
	{
		vecs firstvec;
		for(int j=0;j<6;j++)
		{
			firstvec.vec[j] = rand()%1000;
			
		}
		firstvec.cost = calError(firstvec.vec,points);
		pop.push_back(firstvec);
	}
	
	int totallite = popsize*elite;
	for(int i=0;i<maxiter;i++)
	{
		sort(pop.begin(),pop.end(),mycmp);
		if(oldscore>calError(pop[0].vec,points))
		{
			oldscore = calError(pop[0].vec,points);
			samecount = 0;
		}
		else
			samecount ++;
			
		///*if(!(i%100))
		//{
		//	cout<<i<<" times,score= "<<oldscore<<endl;
		//	cout<<pop[0].cost<<" "<<pop[1].cost<<endl;
		//}*/
		if(samecount>20)break;
		vector<vecs> newpop;
		for(int j=0;j<totallite;j++)
		{
			newpop.push_back(pop[j]);
		}
		while(newpop.size()<popsize)
		{
			double t = randf();
			//cout<<"t= "<<t<<endl;
			if(t<mutprob)
			{
				//cout<<'*';
				int j = rand()%newpop.size();
				newpop.push_back(mutate(newpop[j],step,points));

			}
			else
			{
				//cout<<'.';
				int j = rand()%newpop.size();
				int k = rand()%newpop.size();
				newpop.push_back(crossover(newpop[j],newpop[k],points));
			}
		}
		pop = newpop;
	}
	vec = pop[0];
	/*for(int i=0;i<6;i++)
	{
		cout<<vec.vec[i]<<' ';
	}
	cout<<endl;*/
	cout<<"a= "<<vec.vec[0]<<" b= "<<vec.vec[1]<<" a1= "<<vec.vec[2]<<" b1= "<<vec.vec[3]<<" c1="<<vec.vec[4]<<" d1= "<<vec.vec[5]<<endl;
	cout<<"final error: "<<vec.cost<<endl;
	return vec;

}


vecs mutate(vecs vec,double step,vector<cv::Point> points)
{
	
	int i =rand() % 6;
	vec.vec[i] += -step + randf()*2*step;
	vec.cost = calError(vec.vec,points);
	return vec;
}

vecs crossover(vecs vec1,vecs vec2,vector<cv::Point> points)
{
	
	vecs vec;
	int j = rand()%6;
	for(int i=0;i<6;i++)
	{
		if(i<j)
			vec.vec[i] = vec1.vec[i];
		else
			vec.vec[i] = vec2.vec[i];
	}
	vec.cost = calError(vec.vec,points);
	return vec;
}


double randf() 
{ 
	
	int t = rand();
	//cout<<"t="<<t<<endl;
    return (double)(t/(double)RAND_MAX); 
} 

bool mycmp(vecs vec1,vecs vec2)
{
	return vec1.cost<vec2.cost;
}
