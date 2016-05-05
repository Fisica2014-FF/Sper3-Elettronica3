/*
 * Gain.cpp
 *
 *  Created on: 30/apr/2016
 *      Author: enrico
 */

#include <string>
#include <array>
#include <memory>
#include <iostream>

#include "OpampAnalisys.h"

using namespace std;

int main(int argc, char** argv)
{
	OpampAnalisys::basename = string(argv[1]);
	array<string, 2> type{"inv", "noninv"};

	for (auto i: type)
	{
		cout << i << endl;
		string filename = "/amp_" + i;
		unique_ptr<OpampAnalisys> oA(new OpampAnalisys(filename));
		oA->analisys();
		cout << "amplificatore " + i + " completato\n\n\n";
	}
	return 0;
}


