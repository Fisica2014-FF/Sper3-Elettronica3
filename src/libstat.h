#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

/////Analisi preliminare
double media(int N, double * misure)
{
	double sum=0;
	for (int i=0; i<N; i++){
		sum+=misure[i];
	}
	double media=sum/N;
	return media;
}

double scarto1(int N, double * misure)
{
	double med = media(N, misure);
	double sums=0;
	for(int i=0; i<N; i++){
		sums += (med-misure[i])*(med-misure[i]);
	}
	double sqm=sqrt(sums/N);
	return sqm;
}


double scarto2(int N, double * misure)
{
	double eqm=sqrt((N/(N-1)))*scarto1(N, misure);
	return eqm;
}

double err_media(int N, double * misure)
{
	double em=scarto2(N, misure)/sqrt(N);
	return em;
}
/////


/////Sommatorie
double sum(int N, double * x)
{
	double sum=0;
	for(int i=0; i<N; i++){
		sum+=x[i];
	}
	return sum;
}

double sumquad(int N, double * x)
{
	double sum=0;
	for(int i=0; i<N; i++){
		sum+=(x[i]*x[i]);
	}
	return sum;
}

double sumprod(int N, double * x, double * y)
{
	double sum=0;
	for(int i=0; i<N; i++){
		sum+=(x[i]*y[i]);
	}
	return sum;
}
/////

/////Funzioni interpolazione: y=a+bx
double interp_delta(int N, double * x, double * y)
{
	double delta=N*sumquad(N, x)-((sum(N, x))*(sum(N, x)));
	return delta;
}

double interp_a(int N, double * x, double * y)
{
	double a=(1/interp_delta(N, x, y))*((sumquad(N, x))*(sum(N, y))-(sum(N, x))*(sumprod (N, x, y)));
	return a;
}

double interp_b(int N, double * x, double * y)
{
	double b=(1/interp_delta(N, x, y))*(N*(sumprod(N, x, y))-((sum(N, x))*(sum(N, y))));
	return b;
}
	
double sigmay_post(int N, double * x, double * y)
{
	double sy=0;
	for (int i=0; i<N; i++){
		sy+=((interp_a(N, x, y))+((interp_b(N, x, y))*x[i])-(y[i]))*((interp_a(N, x, y))+((interp_b(N, x, y))*x[i])-(y[i]));
	}
	sy=sy/(N-2);
	sy=sqrt(sy);
	return sy;
}

double interp_sigmaa(int N, double * x, double * y)
{
	double sa=(sigmay_post(N, x, y))*sqrt((sumquad(N, x))/(interp_delta(N, x, y)));
	return sa;
}

double interp_sigmab(int N, double * x, double * y)
{
	double sb=(sigmay_post(N, x, y))*sqrt(N/(interp_delta(N, x, y)));
	return sb;
}

double covab(int N, double * x, double * y)
{
	return (-(sum(N, x)*sigmay_post(N, x, y)*sigmay_post(N, x, y)/interp_delta(N, x, y)));
}
