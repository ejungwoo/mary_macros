#include "style.h"
using namespace style;

#include "global_cut.h"

void one_hit_efficiency()
{
  gstat(0); 
  //version("first_trial");
  //version("depend_y_lownpad");
  //version("ps2");
  version(gvname);

  TString tag = "position_calibration2.1731.c6bd233.hit_sep1";
  auto tree = new TChain("found");
  for (auto i = 0; i < 38; ++i)
    tree -> Add(Form("summary_onehit_data/%s/run2894_s%d.hit_sep.%s.root",tag.Data(),i,tag.Data()));

  //TCut cut_total = "layer>=20&&layer<80&&q1>200&&y<-100&&row>=10&&row<98&&(row<50||row>52)&&q1<3400&&npad1>30";
  //TCut cut_found = "layer>=20&&layer<80&&q1>200&&y<-100&&row>=10&&row<98&&(row<50||row>52)&&q1<3400&&npad1>30&&found";

  auto graph = new TGraphErrors();

  Double_t dy = -20.;
  for (Double_t y0 = -100.; y0 > -499.; y0 += dy)
  {
    TCut cut_total_dy = cut_total + TCut(Form("y<%f&&y>%f",y0,y0+dy));
    TCut cut_found_dy = cut_found + TCut(Form("y<%f&&y>%f",y0,y0+dy));
    auto n0 = tree -> GetEntries(cut_total_dy);
    auto n1 = tree -> GetEntries(cut_found_dy);
    auto eff = Double_t(n1)/n0;
    cout << y0 << ": " << n1 << "/" << n0 << " = " << eff << endl;
    if (n0 < 10 || n1 < 10)
      continue;

    graph -> SetPoint(graph -> GetN(), y0+dy/2., eff);
    graph -> SetPointError(graph -> GetN()-1, 0., 1./sqrt(n0));
  }

  cc();
  auto frame = new TH1D("frame",";y (mm);Efficiency",100,-500,-100);
  frame -> SetMaximum(1.00);
  frame -> SetMinimum(0.5);
  make(frame) -> Draw();
  graph -> SetName("hit_finding_eff");
  graph -> SetFillColor(kGreen);
  make(graph) -> Draw("e3");
  make(graph) -> DrawClone("plx");
  write(graph);

  cc();
  auto hist0 = tp("y0",tree,"y",cut_total ,";y",200,-500,0); hist0 -> SetLineColor(kBlack); make(hist0) -> Draw("");
  auto hist1 = tp("y1",tree,"y",cut_found,";y",200,-500,0); hist1 -> SetLineColor(kBlue); make(hist1) -> Draw("same");

  cc();
  hist0 = tp("q10",tree,"q1",cut_total ,";q1",200,0,4000); hist0 -> SetLineColor(kBlack); make(hist0) -> Draw("");
  hist1 = tp("q11",tree,"q1",cut_found,";q1",200,0,4000); hist1 -> SetLineColor(kBlue); make(hist1) -> Draw("same");

  //cc(); make(tp("layer",tree,"layer",cut_total,";layer",112,0,112)) -> Draw();
  //cc(); make(tp("row",tree,"row",cut_total,";row",108,0,108)) -> Draw();
  //cc(); make(tp("npad1",tree,"npad1",cut_total,";npad1",120,0,120)) -> Draw();
  //cc(); make(tp("q1",tree,"q1",cut_total,";q1",4000,0,4000)) -> Draw();
  return;
}
