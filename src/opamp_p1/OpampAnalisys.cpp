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

string OpampAnalisys::basename ="";

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
	// TODO Auto-generated destructor stub
}

void OpampAnalisys::analisys()
{
	TCanvas c("Interpolazione Opamp");
	c.SetGrid();

	g->SetFillColor(1);
	g->SetLineColor(2);
	g->SetLineWidth(1);
	g->SetMarkerColor(4);
	g->SetMarkerSize(0.7F);
	g->SetMarkerStyle(1);
	g->SetTitle("Gain");
	g->GetXaxis()->SetTitle("V_{out} [V]");
	g->GetYaxis()->SetTitle("V_{in} [V]");
	g->Draw("AP");

	TF1* f = new TF1("fit", "[0]+[1]*x");
	f->SetParName(1, "m");
	f->SetParName(0, "q");
	f->SetLineColor(4);
	f->SetLineWidth(1);

	TFitResultPtr r = g->Fit(f, "S", "", -1.5, 1.5);
	r->Print("V");
	for (unsigned int i = 0; i < r->NPar(); ++i)
	{
		clog << r->ParName(i)
			<< " " << r->Parameter(i)
			<< " " << r->ParError(i) << endl;
	}

	TLegend *leg = new TLegend(0.8, 0.8, 0.9, 0.9);
	leg->AddEntry(g.get(), "Data", "lp");
	leg->AddEntry(f, "Fit", "l");
	leg->Draw();

	c.Update();
	c.GetFrame()->SetFillColor(0);
	c.GetFrame()->SetBorderSize(12);
	c.Modified();

	string name = "Result" + filename + ".tex";
	c.Print(name.c_str());
}

