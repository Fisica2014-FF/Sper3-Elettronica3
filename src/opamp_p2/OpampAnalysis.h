/*
 * OpampAnalysis.h
 *
 *  Created on: 01/mag/2016
 *      Author: enrico
 */

#ifndef OPAMPAnalysis_H_
#define OPAMPAnalysis_H_

#include <string>
#include <memory>

#include <TMultiGraph.h>

class TGraphErrors;

class OpampAnalysis final
{
	public:
		OpampAnalysis(std::string filename);
		virtual ~OpampAnalysis();

		static std::string basename;

		static TMultiGraph* gAll;

		void analysis();
	private:
		std::unique_ptr<TGraphErrors> g;
		std::string filename;
};

#endif /* OPAMPAnalysis_H_ */
