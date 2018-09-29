#include "style.h"
using namespace style;

void yoffset_dw_calibration()
{
  gstat(0);
  zcolor(1);

  auto file = new TFile("~/data/spirit/overview_all_calibrated.root","read");
  auto h2 = (TH2D *) file -> Get("run\%d_yoffset_calibrated5");

  auto graph = new TGraph();

  for (int bin = 0; bin < 200; bin+=2)
  {
    auto h1 = h2 -> ProjectionX(Form("project%d",bin),bin,bin+2);
    auto f1 = fitg(h1,1);
    graph -> SetPoint(graph -> GetN(), f1 -> GetParameter(1),(bin+1)*0.5-50);
  }

  auto c1 = cc("dw_calibratiion");
  make(h2) -> Draw("colz");
  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(0.2);
  graph -> Draw("samep");
  c1 -> SetLogz();

  save(c1);
}
