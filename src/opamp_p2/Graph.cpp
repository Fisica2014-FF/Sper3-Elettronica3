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

void Graph::add(double x, double y, double ey)
{
	vx.push_back(x);
	vy.push_back(y);
	vey.push_back(ey);
	++num;
}

