#include "style.h"
using namespace style;

void position()
{
  fsave(0);
  gstat(1);

  TString tag = "develop.1710.62716a4.original_cov";
  version(tag);

  auto tree1 = new TChain("events");
  tree1 -> AddFile(Form("summary_data/%s/summary_event_run%d.%s.root",tag.Data(),2900,tag.Data()));

  auto file2 = new TFile("/Users/ejungwoo/spirit/data/beam_run2900.ridf.root");
  auto tree2 = (TTree *) file2 -> Get("TBDC");

  auto nx =  100;
  auto x1 = -30.;
  auto x2 =  30.;

  auto ny =  100;
  auto y1 = -250.;
  auto y2 = -200.;
  //auto y1 = -10;
  //auto y2 = 10;

  auto nz =  100;
  auto z1 = -30.;
  auto z2 =  10.;

  TCanvas *cvs;

  cvs = cc("compare_x");
  auto histx2 = tp(tree2,"1.*ProjX","","vertexx2",tag+";vertex-x",nx,x1,x2); histx2 -> SetLineColor(kRed);  norm_max(histx2) -> Draw("");
  auto histx1 = tp(tree1,"vx"    ,"","vertexx1",tag+";vertex-x",nx,x1,x2); histx1 -> SetLineColor(kBlue); norm_max(histx1) -> Draw("same");
  save(cvs);

  cvs = cc("compare_y");
  auto histy2 = tp(tree2,"1.*ProjY-227","","vertexy2",tag+";vertex-y",ny,y1,y2); histy2 -> SetLineColor(kRed);  norm_max(histy2) -> Draw("");
  auto histy1 = tp(tree1,"vy","","vertexy1",tag+";vertex-y",ny,y1,y2);     histy1 -> SetLineColor(kBlue); norm_max(histy1) -> Draw("same");
  save(cvs);

  cvs = cc("compare_z");
  auto histz1 = tp(tree1,"vz","","vertexz1",tag+";vertex-z",nz,z1,z2);     histz1 -> SetLineColor(kBlue); norm_max(histz1) -> Draw("");
  auto histz2 = tp(tree2,"1.*ProjZ+580","","vertexz2",tag+";vertex-z",nz,z1,z2); histz2 -> SetLineColor(kRed);  norm_max(histz2) -> Draw("same");
  save(cvs);
}
