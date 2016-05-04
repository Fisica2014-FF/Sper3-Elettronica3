/*
 * readGraph.cpp
 *
 *  Created on: 01/mag/2016
 *      Author: enrico
 */
#include <memory>
#include <fstream>
#include <string>
#include <cmath>

#include "Graph.h"

using namespace std;

double error(double x, double fs)
{
	return sqrt((0.06*fs)*(0.06*fs) +
			(0.02*x)*(0.02*x));
}

unique_ptr<Graph> readGraph(string filename)
{
	unique_ptr<Graph> g(new Graph());
	fstream fin(filename.c_str(), iostream::in);
	double x1, x2, y1, y2,  ex, ey, fsx, fsy;
	while(fin>>x1>>x2>>fsx>>y1>>y2>>fsy)
	{
		ex = error(x1, fsx);
		ey = error(y1, fsy);
		g->add(x1, y1, ex, ey);
		ex = error(x2, fsx);
		ey = error(y2, fsy);
		g->add(x2, y2, ex, ey);
	}

	return g;
}


