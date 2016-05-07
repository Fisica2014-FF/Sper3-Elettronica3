/*
 * AdHocParameters.h
 *
 *  Created on: 07/mag/2016
 *      Author: enrico
 */

#ifndef ADHOCPARAMETERS_H_
#define ADHOCPARAMETERS_H_

struct AdHocParameters
{
	// simple structure to store the parameters that change
	// according to the amplitude considered
	public:
		std::string filename;
		double endPlat;
		double startFall;
		double stopFall;
};

#endif /* ADHOCPARAMETERS_H_ */
