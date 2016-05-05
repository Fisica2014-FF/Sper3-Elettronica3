/*
 * Graph.h
 *
 *  Created on: 01/mag/2016
 *      Author: enrico
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

class Graph
{
	public:
		Graph();
		virtual ~Graph();
		double* x()
		{
			return &(vx[0]);
		};
		double* y()
		{
			return &(vy[0]);
		};
		double* ex()
		{
			return &(vex[0]);
		};
		double* ey()
		{
			return &(vey[0]);
		};

		unsigned int n()
		{
			return num;
		};

		void add(double x, double y, double ex, double ey);
	private:
		std::vector<double> vx;
		std::vector<double> vy;
		std::vector<double> vex;
		std::vector<double> vey;

		unsigned int num;
};

#endif /* GRAPH_H_ */
