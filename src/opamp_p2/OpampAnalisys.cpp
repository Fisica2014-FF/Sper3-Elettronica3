/*
 * OpampAnalisys.cpp
 *
 *  Created on: 01/mag/2016
 *      Author: enrico
 */

#include "OpampAnalisys.h"
#include "Graph.h"

#include <TROOT.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TFitResult.h>
#include <TFrame.h>
#include <TLegend.h>

#include <iostream>

using namespace std;

std::string OpampAnalisys::basename = "";

unique_ptr<Graph> readGraph(string);

OpampAnalisys::OpampAnalisys(string filename)
	:filename(filename)
{
	string name = basename + filename + ".txt";
	unique_ptr<Graph> gr = readGraph(name);

	cout << gr->n() << endl;
	g = unique_ptr<TGraphErrors>(new TGraphErrors(gr->n(), gr->x(), gr->y(), gr->ex(), gr->ey()));
}

OpampAnalisys::~OpampAnalisys()
{
	// RAII
}

void OpampAnalisys::analisys()
{
	TCanvas c("Interpolazione Opamp");
	c.SetGrid();
	c.SetLogy();
	c.SetLogx();

	g->SetFillColor(1);
	g->SetLineColor(2);
	g->SetLineWidth(1);
	g->SetMarkerColor(4);
	g->SetMarkerSize(0.7F);
	g->SetMarkerStyle(1);
	g->SetTitle("Gain");
	g->GetXaxis()->SetTitle("f [Hz]");
	g->GetYaxis()->SetTitle("A");
	g->Draw("AP");

	TF1* fFall = new TF1("Discesa", "pow(10, ([0] + [1]*log10(x)))");
	fFall->SetParName(0, "q");
	fFall->SetParName(1, "m");
	fFall->SetParameter(1, -1.);
	fFall->SetLineColor(4);
	fFall->SetLineWidth(1);

	TF1* fPlat = new TF1("Plateau", "[0]");
	fPlat->SetParName(0, "A");
	fPlat->SetParameter(0, 10.);
	fPlat->SetLineColor(2);
	fPlat->SetLineWidth(1);

	TFitResultPtr rPlat = g->Fit(fPlat, "S", "", 0, 100001);
	rPlat->Print("V");
	for (unsigned int i = 0; i < rPlat->NPar(); ++i)
	{
		clog << rPlat->ParName(i)
			<< " " << rPlat->Parameter(i)
			<< " " << rPlat->ParError(i) << endl;
	}

	TFitResultPtr rFall = g->Fit(fFall, "S+", "", 100001, 399999);
	rFall->Print("V");
	for (unsigned int i = 0; i < rFall->NPar(); ++i)
	{
		clog << rFall->ParName(i)
			<< " " << rFall->Parameter(i)
			<< " " << rFall->ParError(i) << endl;
	}

	TLegend *leg = new TLegend(0.8, 0.8, 0.9, 0.9);
	leg->AddEntry(g.get(), "Data", "lp");
	leg->AddEntry(fFall, "Discesa", "l");
	leg->AddEntry(fPlat, "Plateau", "l");
	leg->Draw();

	c.Update();
	c.GetFrame()->SetFillColor(0);
	c.GetFrame()->SetBorderSize(12);
	c.Modified();

	string name = "Result" + filename + ".tex";
	c.Print(name.c_str());
}

