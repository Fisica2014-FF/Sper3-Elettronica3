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
	OpampAnalisys::basename =
		string(argv[1]);

	array<string, 3> type{"1", "5", "10"};

	for (auto i: type)
	{
		cout << i << endl;
		string filename = "/amp_noninv_A" + i;
		unique_ptr<OpampAnalisys> oA(new OpampAnalisys(filename));
		oA->analisys();
		cout <<"A = " << i + " completato\n\n\n";
	}
	return 0;
}


