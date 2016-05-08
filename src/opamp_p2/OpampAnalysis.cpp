/*
 * OpampAnalysis.cpp
 *
 *  Created on: 01/mag/2016
 *      Author: enrico
 *
 *  Scritto per root 6.06.02
 */

#include "OpampAnalysis.h"
#include "Graph.h"

#include <TROOT.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TF2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TFitResult.h>
#include <TFrame.h>
#include <TLegend.h>

#include <iostream>

using namespace std;

std::string OpampAnalysis::basename = "";
TMultiGraph* OpampAnalysis::gAll = new TMultiGraph();

unique_ptr<Graph> readGraph(string);

OpampAnalysis::OpampAnalysis(const AdHocParameters &p)
	:p(p)
{
	// read graph from datafile
	string name = basename + p.filename + ".txt";
	unique_ptr<Graph> gr = readGraph(name);

	// initialize TGraphErrors
	g = unique_ptr<TGraphErrors>(new TGraphErrors(gr->n(), gr->x(), gr->y(), gr->ex(), gr->ey()));

	// graph clone, to be stored in the multigraph
	TGraphErrors* gClone = dynamic_cast<TGraphErrors*>(g->Clone());
	gClone->SetFillColor(1);
	gClone->SetLineColor(2);
	gClone->SetLineWidth(1);
	gClone->SetMarkerColor(4);
	gClone->SetMarkerSize(0.7F);
	gClone->SetMarkerStyle(1);
	gAll->Add(gClone, "PL");
}

OpampAnalysis::~OpampAnalysis()
{
	// RAII
}

void OpampAnalysis::analysis()
{
	// Create canvas and update settings
	TCanvas c("Interpolazione Opamp");
	c.SetGrid();
	c.SetLogy();
	c.SetLogx();
	c.GetFrame()->SetFillColor(0);
	c.GetFrame()->SetBorderSize(12);

	// Graph settings
	g->SetFillColor(1);
	g->SetLineColor(2);
	g->SetLineWidth(1);
	g->SetMarkerColor(4);
	g->SetMarkerSize(0.7F);
	g->SetMarkerStyle(1);
	g->SetTitle("Frequency response");
	g->GetXaxis()->SetTitle("f [Hz]");
	g->GetYaxis()->SetTitle("A");
	g->Draw("AP");
	g->Print();

	// Interpolation function, works on log10(y)
	TF1* fFall = new TF1("Discesa", "[0] + [1]*log10(x)");
	fFall->SetParName(0, "q");
	fFall->SetParName(1, "m");
	fFall->SetParameter(1, -1.);
	fFall->SetLineColor(4);
	fFall->SetLineWidth(1);

	// Interpolation function, works on log10(y)
	TF1* fPlat = new TF1("Plateau", "[0]");
	fPlat->SetParName(0, "A");
	fPlat->SetParameter(0, 10.);
	fPlat->SetLineColor(2);
	fPlat->SetLineWidth(1);

	// Function to change the graph to logarithmic
	TF2 *t = new TF2("transform", "log10(y)");

	// Clone of the current graph, where t will be applied
	// this one will be interpolated
	TGraphErrors* gLog = dynamic_cast<TGraphErrors*>(g->Clone());
	gLog->Apply(t);

	// Fit the logarithmic graph, on the constant part
	TFitResultPtr rPlat = gLog->Fit(fPlat, "SM", "", 10, p.endPlat);
	rPlat->Print("V");
	for (unsigned int i = 0; i < rPlat->NPar(); ++i)
	{
		clog << rPlat->ParName(i)
			<< " " << rPlat->Parameter(i)
			<< " " << rPlat->ParError(i) << endl;
	}
	// Actual function that will be drawn
	TF1* fPlatNorm = new TF1("Plateau",
			[=](Double_t* x, Double_t *p)
			{
				return pow(10, rPlat->Parameter(0));
			},
			10, 10000000, 0);
	fPlatNorm->SetLineColor(2);
	fPlatNorm->SetLineWidth(1);
	fPlatNorm->Draw("LSAME");

	// Fit the logarithmic graph, on the "linear" part
	TFitResultPtr rFall = gLog->Fit(fFall, "S+", "", p.startFall, p.stopFall);
	rFall->Print("V");
	for (unsigned int i = 0; i < rFall->NPar(); ++i)
	{
		clog << rFall->ParName(i)
			<< " " << rFall->Parameter(i)
			<< " " << rFall->ParError(i) << endl;
	}
	// Actual function that will be drawn
	TF1* fFallNorm = new TF1("Discesa",
			[=](Double_t* x, Double_t *p)
			{
				return pow(10, rFall->Parameter(0) + rFall->Parameter(1)*log10(x[0]));
			},
			10, 10000000, 0);
	fFallNorm->SetLineColor(4);
	fFallNorm->SetLineWidth(1);
	fFallNorm->Draw("LSAME");

	// Calculate f_b
	double A = rPlat->Parameter(0) - log10(2)/2;
	double eA = rPlat->ParError(0);

	double m = rFall->Parameter(1);
	double em = rFall->ParError(1);

	double q = rFall->Parameter(0);
	double eq = rFall->ParError(0);

	double covmq = rFall->CovMatrix(0,1);

	double fb = pow(10, (A - q)/m);

	double dA = 1/m;
	double dq = -dA;
	double dm = -(A - q)/(m*m);
	double efb = fb*log(10)*
				 sqrt(dA*dA*eA*eA +
					  dq*dq*eq*eq +
					  dm*dm*em*em +
					  2*dq*dm*covmq);

	clog << "fb " << fb << " " << efb << endl;


	// Draw legend
	TLegend *leg = new TLegend(0.8, 0.8, 0.9, 0.9);
	leg->AddEntry(g.get(), "Data", "lp");
	leg->AddEntry(fFallNorm, "Discesa", "l");
	leg->AddEntry(fPlatNorm, "Plateau", "l");
	leg->Draw();

	// Save result as image on disk
	string name = "Result" + p.filename + ".tex";
	c.Print(name.c_str());

	name = "Result" + p.filename + ".svg";
	c.Print(name.c_str());

}

