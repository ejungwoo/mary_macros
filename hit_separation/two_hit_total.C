#include "global_cut.h"
#include "style.h"
using namespace style;

void two_hit_total()
{
  gstat(0);
  fsave(1);
  //TString vname1 = "depend_y";
  //TString vname1 = "depend_y_midq";
  //TString vname1 = "depend_y_lownpad";
  //TString vname1 = "first_trial";
  //TString vname1 = "ps2";
  version(gvname);
  Double_t dy = -20.;

  cutt();
  cutr();

  Int_t n0sum[120] = {0};
  Double_t n1sum[120] = {0};

  for (auto y0=-100.; y0>-499.; y0+=dy)
  {
    ifstream file(Form("data/count_found.y%d.%s.dat",Int_t(y0+dy/2.),gvname.Data()));

    Double_t n1;
    Int_t i, n0;
    while (file >> i >> n1 >> n0) {
      //cout << i << " " << n0 << " " << n1 << endl;
      n0sum[i-1] += n0;
      n1sum[i-1] += n1;
    }
  }

  auto nbin = 120;
  auto min = 0.;
  auto max = 400.;
  auto dbin = (max-min)/nbin;
  auto zoom_max = 20.;

  auto graph = new TGraphErrors();
  for (auto bin = 0; bin < nbin; ++bin)
  {
    //cout << bin << " " << n1sum[bin] << " / " << n0sum[bin] << endl;
    if (n0sum[bin]<10)
      continue;
    graph -> SetPoint(graph->GetN(), (bin+0.5)*dbin, n1sum[bin]/n0sum[bin]);
    graph -> SetPointError(graph -> GetN()-1, 0, 1./sqrt(n0sum[bin]));
  }

  auto f1 = new TF1("max","[0]",0,400);
  make(f1);
  f1 -> SetParameter(0,0.9);
  f1 -> SetLineColor(kRed);
  f1 -> SetLineWidth(3);
  graph -> Fit(f1,"0","",200,400);

  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(0.5);
  graph -> SetFillColor(kGreen);
  auto line = new TLine();
  line -> SetLineColor(kRed);

  auto c1 = cc("hit_sep_eff");
  auto frame = new TH1D("frame",";Two-hit separation distance (mm);Efficiency",nbin,min,max);
  frame -> SetMaximum(1.00);
  make(frame) -> Draw();
  make(graph) -> Draw("e3");
  f1 -> Draw("same");
  make(graph) -> DrawClone("lx");
  //line -> DrawLine(0,1,400,1);

  save(c1,"png");

  auto frame2 = new TH1D("frame2",";Two-hit separation distance (mm);Efficiency",nbin,min,zoom_max);
  frame2 -> SetMaximum(1.00);
  auto c2 = cc("hit_sep_eff_zoomed");
  make(frame2) -> Draw();
  make(graph) -> DrawClone("e3");
  make(graph) -> DrawClone("lx");
  f1 -> DrawCopy("same");
  //line -> DrawLine(0,1,20,1);

  save(c2,"pdf");
}
