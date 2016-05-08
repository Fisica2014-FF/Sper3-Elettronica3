/*
 * Gain.cpp
 *
 *  Created on: 30/apr/2016
 *      Author: enrico
 *
 *  Scritto per root 6.06.02
 */

#include <string>
#include <array>
#include <memory>
#include <iostream>

#include <TCanvas.h>
#include <TAxis.h>

#include "OpampAnalysis.h"
#include "AdHocParameters.h"

using namespace std;

int main(int argc, char** argv)
{
	// initialize folder name, where data is stored
	OpampAnalysis::basename =
		string(argv[1]);

	// initialize parameters
	array<AdHocParameters, 3> parameters{{
		{"/amp_noninv_A1", 100000, 1000000, 2000000},
		{"/amp_noninv_A5", 99999, 300001, 999999},
		{"/amp_noninv_A10", 5001, 100001, 399999}
	}};

	for (auto i: parameters)
	{
		unique_ptr<OpampAnalysis> oA(new OpampAnalysis(i));
		oA->analysis();
	}

	string name = "Result/amp_noninv_all.tex";

	// canvas to store the graphs of all amplitudes
	TCanvas *cAll = new TCanvas("Risposta in frequenza");
	cAll->SetGrid();
	cAll->SetLogx();
	cAll->SetLogy();

	OpampAnalysis::gAll->SetTitle("Frequency response");

	OpampAnalysis::gAll->Draw("A");
	OpampAnalysis::gAll->GetXaxis()->SetTitle("f [Hz]");
	OpampAnalysis::gAll->GetYaxis()->SetTitle("A");

	OpampAnalysis::gAll->Draw("A");

	cAll->Print(name.c_str());

	return 0;
}


