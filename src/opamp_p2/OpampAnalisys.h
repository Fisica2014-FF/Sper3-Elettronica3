/*
 * OpampAnalisys.h
 *
 *  Created on: 01/mag/2016
 *      Author: enrico
 */

#ifndef OPAMPANALISYS_H_
#define OPAMPANALISYS_H_

#include <string>
#include <memory>

class TGraphErrors;

class OpampAnalisys final
{
	public:
		OpampAnalisys(std::string filename);
		virtual ~OpampAnalisys();

		static std::string basename;

		void analisys();
	private:
		std::unique_ptr<TGraphErrors> g;
		std::string filename;
};

#endif /* OPAMPANALISYS_H_ */
