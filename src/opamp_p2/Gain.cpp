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

#include <TCanvas.h>
#include <TAxis.h>

#include "OpampAnalysis.h"

using namespace std;

int main(int argc, char** argv)
{
	OpampAnalysis::basename =
		string(argv[1]);

	array<string, 3> type{"1", "5", "10"};

	for (auto i: type)
	{
		cout << i << endl;
		string filename = "/amp_noninv_A" + i;
		unique_ptr<OpampAnalysis> oA(new OpampAnalysis(filename));
		oA->analysis();
		cout <<"A = " << i + " completato\n\n\n";
	}

	string name = "Result/amp_noninv_all.tex";

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


