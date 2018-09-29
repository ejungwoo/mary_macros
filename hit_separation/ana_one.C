#include "global_cut.h"
#include "style.h"
using namespace style;

void ana_one()
{
  gstat(0);
  //TString tag = "depend_y_lownpad";
  //TString tag = "first_trial";
  //TString tag = "ps2";
  version(gvname);
  cutt();
  cutr();

  auto file = new TFile(Form("/Users/ejungwoo/spirit/hit_separation/data/hit_finding_eff.%s.root",gvname.Data()));
  auto graph = (TGraph *) file -> Get("hit_finding_eff");

  auto cvs = c("eff_vs_y",680,400);
     int fWCvs=680;
     int fHCvs=550;
  graph -> SetMarkerStyle(1);
  //graph -> GetHistogram() -> SetTitle("Hit finding efficiency;hit-y (mm);Efficiency");
  graph -> GetHistogram() -> SetTitle(";hit-y (mm);Efficiency");
  graph -> GetHistogram() -> SetMaximum(1.);
  graph -> GetHistogram() -> SetMinimum(0.8);
  //make(graph) -> Draw("ap");

  auto frame = new TH1D("frame",";hit y-position (mm);Efficiency",100,-500,-100);
  frame -> SetMaximum(1.0);
  frame -> SetMinimum(0.78);
  make(frame) -> Draw();
  graph -> SetName("hit_finding_eff");
  graph -> SetFillColor(kGreen);
  make(graph) -> Draw("e3");
  make(graph) -> DrawClone("px");

  save(cvs,"png");

  //auto f1 = new TF1("fit_hit_finding_eff","[0] + (x<=[1])*[2]*(x-[1])*(x-[3]) + (x>[1])*[4]*(x-[1])*(x-[5])",-500,-100);
  //f1 -> SetParameters(0.8,-220,-0.000002,-250,0.0002,-500);
  //auto f1 = new TF1("fit_hit_finding_eff","[0]+[1]*(x-[2])*(x-[3])*(x-[4])",-500,-100);
  //f1 -> SetParameters(0.8,0.000002,-500,-225,250);
  auto f1 = new TF1("fit_hit_finding_eff","pol5",-500,-100);
  f1 -> SetParameters(0.8,0.000002,-500,-225,250);
  graph -> Fit(f1,"0");
  make(f1) -> Draw("same");
  write(f1);
}
