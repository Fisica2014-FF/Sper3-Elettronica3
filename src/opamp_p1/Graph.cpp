/*
 * Graph.cpp
 *
 *  Created on: 01/mag/2016
 *      Author: enrico
 */

#include "Graph.h"

Graph::Graph()
	:num(0)
{
}

Graph::~Graph()
{
}

void Graph::add(double x, double y, double ex, double ey)
{
	vx.push_back(x);
	vy.push_back(y);
	vex.push_back(ex);
	vey.push_back(ey);
	++num;
}

