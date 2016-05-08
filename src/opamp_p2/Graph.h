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
	// class storing vector of points as vector of doubles
	// and can return pointers to the data, for root's TGraph
	public:
		Graph();
		virtual ~Graph();
		double* x()
		{
			return vx.data();
		};
		double* y()
		{
			return vy.data();
		};
		double* ex()
		{
			return vex.data();
		};
		double* ey()
		{
			return vey.data();
		};

		unsigned int n()
		{
			return num;
		};

		void add(double x, double y, double ey);

	private:
		std::vector<double> vx;
		std::vector<double> vy;
		std::vector<double> vex;
		std::vector<double> vey;

		unsigned int num;
};

#endif /* GRAPH_H_ */
