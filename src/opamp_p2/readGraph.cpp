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

const double fsin = 0.2;

double error(double A, double fsout)
{
	return sqrt((0.06*fsout)*(0.06*fsout) + (A*0.06*fsin)*(A*0.06*fsin) +
			2*(0.02*A)*(0.02*A));
}

unique_ptr<Graph> readGraph(string filename)
{
	unique_ptr<Graph> g(new Graph());
	fstream fin(filename.c_str(), iostream::in);
	double x, y, ey, fsy;
	while(fin>>x>>y>>fsy)
	{
		ey = error(y, fsy);
		g->add(x, y, ey);
	}
	return g;
}


