/*
 * OpampAnalysis.h
 *
 *  Created on: 01/mag/2016
 *      Author: enrico
 *
 *  Scritto per root 6.06.02
 */

#ifndef OPAMPAnalysis_H_
#define OPAMPAnalysis_H_

#include <string>
#include <memory>

#include <TMultiGraph.h>

#include "AdHocParameters.h"

class TGraphErrors;

class OpampAnalysis final
{
	// actual analysis class
	public:
		OpampAnalysis(const AdHocParameters &p);
		virtual ~OpampAnalysis();

		// folder name, the program expects the files to be there
		static std::string basename;

		// multigraph to store graphs of all amplitudes
		static TMultiGraph* gAll;

		void analysis();
	private:
		std::unique_ptr<TGraphErrors> g;
		AdHocParameters p;
};

#endif /* OPAMPAnalysis_H_ */
