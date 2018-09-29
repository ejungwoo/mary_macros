#include "style.h"
using namespace style;

void two_hit_efficiency()
{
  auto fsave = false;
  //auto one_hit_eff = 0.904453; //using run2894_s0.hit_sep.develop.RC.1612.62c6e62.hs_task3.root
  auto fitFile = new TFile("fit_hit_finding_eff.root");
  auto fit_one_hit_eff = (TF1 *) fitFile -> Get("fit_hit_finding_eff");

  gstat(0); 
  zcolor(1); 

  auto tree = new TChain("found");
  for (auto split = 0; split < 38; ++split)
    tree -> Add(Form("summary_data/run2894_s%d.hit_sep.develop.RC.1612.62c6e62.hs_task2.root",split));

  //TCut cut = "layer>20&&layer<80&&q1>200&&q2>200&y<-100&&y-dtrue>-500&&(row<44||row>63)&&q1<3400&&q2<3400";
  //TCut cut = "layer>20&&layer<80&&q1>200&&q2>200&y<-100&&y-dtrue>-500&&(row<48||row>54)&&q1<3400&&q2<3400";
  //TCut cut = "layer>20&&layer<80&&q1>200&&q2>200&y<-100&&y-dtrue>-500&&q1<3400&&q2<3400";
  //TCut cut = "layer>20&&layer<80&&q1>200&&q2>200&y<-100&&y-dtrue>-500&&row>20&&row<80&&q1<3400&&q2<3400";
  //TCut cut = "layer>20&&layer<80&&q1>200&&q2>200&y<-100&&y-dtrue>-500&&q1<3400&&q2<3400";
  //TCut cut = "layer>20&&layer<80&&q1>200&&q2>200&y<-100&&y-dtrue>-500&&(row<48||row>54)&&q1<3400&&q2<3400";
  //TCut cut = "layer>20&&layer<80&&q1>200&&q2>200&y<-100&&y-dtrue>-500&&(row<45||row>56)&&q1<3400&&q2<3400";
  //TCut cut = "layer>20&&layer<80&&q1>200&&q2>200&y<-100&&y-dtrue>-500&&row>=10&&row<98&&(row<51||row>52)&&q1<3400&&q2<3400";

  TCut cut = "layer>=20&&layer<80&&q1>200&&q2>200&&y<-100&&y-dtrue>-500&&row>=10&&row<98&&(row<50||row>52)&&q1<3400&&q2<3400&&npad1>30&&npad2>30";
  //TCut cut = "layer>=20&&layer<80&&q1>200&&q2>200&&y<-100&&y-dtrue>-500&&row>=10&&row<98&&(row<50||row>52)&&q1<3400&&q2<3400&&npad1>30&&npad2>30&&abs(q1-q2)/q1<0.5";

  auto dmax = 400;
  TString ymane[] = {TString("q1"),TString("q2"),TString("y"),TString("dreco"),TString("layer"),TString("row"),TString("npad1"),TString("npad2"),TString("(npad2+npad1)/2")};
  TString var[] = {TString("q1"),TString("q2"),TString("y"),TString("dreco"),TString("layer"),TString("row"),TString("npad1"),TString("npad2"),TString("(npad2+npad1)/2")};

  const char *yname[] = {"First hit charge (ADC)","Second hit charge (ADC)","First hit y-position (mm)","Reconstructed hit distance (mm)","TPC layer","TPC row","# of hits in 1^{st} track","# of hits in 2^{nd} track","Average # of hits"};
  const char *varc[] = {"q1","q2","y","dreco","layer","row","npad1","npad2","(npad2+npad1)/2"};
  Int_t n[] = {200,200,200,200,112,108,120,120,120};
  Int_t ymin[] = {0,0,-500,0,0,0,0,0,0};
  Int_t ymax[] = {4000,4000,0,dmax,112,108,120,120,120};

  if(0)
  for (auto i = 0; i < 9; ++i) {
    TString formula = TString(varc[i])+":dtrue";
    TString name = formula; name.ReplaceAll(":","_vs_");
    name.ReplaceAll("(","LL");
    name.ReplaceAll(")","RR");
    name.ReplaceAll("+","_plus_");
    name.ReplaceAll("/","_div_");

    auto hist = tp(name,tree,formula,cut,TString(";Two-hit separation distance (mm);")+yname[i],200,0,dmax,n[i],ymin[i],ymax[i]);

    auto cvs = cc(name);
    if (formula.Index("dreco")>=0||formula.Index("npad")>=0)
      cvs -> SetLogz();
    hist -> Draw("colz");
    if (fsave) save(cvs);
  }

  auto nbin = 120;
  auto min = 0.;
  auto max = 400.;
  auto zoom_max = 20.;

  auto h0 = tp("h0",tree,"dtrue",cut,";Two-hit separation distance (mm);",nbin,min,max);
  auto h1 = tp("h1",tree,"dtrue",cut+TCut("found"),"found;Two-hit separation distance (mm);",nbin,min,max);
  auto h2 = tp("h2",tree,"dtrue",cut+TCut("!found"),"not found;Two-hit separation distance (mm);",nbin,min,max);

  auto c2 = cc("number_of_found_pad");
  h0 -> SetLineColor(kBlack);
  h1 -> SetLineColor(kBlue);
  h2 -> SetLineColor(kRed);
  h0 -> Draw();
  h1 -> Draw("same");
  h2 -> Draw("same");

  auto l = new TLegend();
  l -> AddEntry(h0,"Total","l");
  l -> AddEntry(h1,"Found","l");
  l -> AddEntry(h2,"Not found","l");
  make(l) -> Draw();

  if (fsave) save(c2,"pdf");

  auto graph = new TGraphErrors();
  for (auto bin = 1; bin <= nbin; ++bin)
  {
    auto n_all = h0 -> GetBinContent(bin);
    auto n_found = h1 -> GetBinContent(bin);

    auto x = h0 -> GetXaxis() -> GetBinCenter(bin);
    auto eff = Double_t(n_found)/n_all;

    graph -> SetPoint(graph->GetN(), x, eff/fit_one_hit_eff);
    graph -> SetPointError(graph -> GetN()-1, 0, 1./sqrt(h0->GetBinContent(bin)));

    //cout << x << " " << eff << " " << 1./sqrt(h0->GetBinContent(bin)) << endl;
  }

  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(0.5);
  graph -> SetFillColor(kGreen);

  auto line = new TLine();
  line -> SetLineStyle(2);
  line -> SetLineColor(kRed);

  auto frame = new TH1D("frame",";Two-hit separation distance (mm);Efficiency",nbin,min,max);
  frame -> SetMaximum(1.05);
  auto frame2 = new TH1D("frame2",";Two-hit separation distance (mm);Efficiency",nbin,min,zoom_max);
  frame2 -> SetMaximum(1.05);

  auto c3 = cc("hit_sep_eff");
  make(frame) -> Draw();
  make(graph) -> Draw("e3");
  make(graph) -> DrawClone("lx");
  line -> DrawLine(0,1,400,1);
  if (fsave) save(c3,"pdf");

  auto c4 = cc("hit_sep_eff_zoomed");
  make(frame2) -> Draw();
  make(graph) -> DrawClone("e3");
  make(graph) -> DrawClone("lx");
  line -> DrawLine(0,1,20,1);
  if (fsave) save(c4,"pdf");

  //auto file_temp = new TFile("file_temp.root","recreate");
  //frame -> Write();
  //graph -> Write("graph");
}
