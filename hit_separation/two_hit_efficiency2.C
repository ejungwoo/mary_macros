#include "global_cut.h"
#include "style.h"
using namespace style;

void two_hit_efficiency2(Double_t y0 = -400, Double_t dy = -20.)
{
  fsave(1);
  gstat(0); 
  zcolor(1); 

  //TString vname1 = "depend_y_midq";
  //TString vname1 = "depend_y_lownpad";
  //TString vname1 = "ps2";
  //TString vname1 = "first_trial";
  //TString vname1 = "depend_y";

  auto fitFile = new TFile(Form("data/fit_hit_finding_eff.%s.root",gvname.Data()));
  auto fit_one_hit_eff = (TF1 *) fitFile -> Get("fit_hit_finding_eff");
  auto eff1 = fit_one_hit_eff -> Eval(y0+dy/2.);
  cout << eff1 << endl;

  TString vname = Form("y%d",Int_t(y0+dy/2.));
  version(vname);
  TString tag = Form("y=%d~%d 1HitEff=%.2f",Int_t(y0),Int_t(y0+dy),eff1);

  auto file = new TFile(Form("data/hit_sep_data.%s.root",gvname.Data()));
  auto tree = (TTree *) file -> Get("found");

  TCut cut = Form("y-dtrue<%f&&y-dtrue>=%f",y0,y0+dy);
  cout << cut << endl;

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
    save(cvs);
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

  //save(c2,"pdf");

  ofstream out_file(TString("data/count_found.")+vname+"."+gvname+".dat");

  auto graph = new TGraphErrors();
  for (auto bin = 1; bin <= nbin; ++bin)
  {
    auto n_all = h0 -> GetBinContent(bin);
    auto n_found = h1 -> GetBinContent(bin);

    if (n_all < 10)
      continue;

    auto x = h0 -> GetXaxis() -> GetBinCenter(bin);
    auto eff2 = (n_found/eff1)/n_all;

    graph -> SetPoint(graph->GetN(), x, eff2);
    //graph -> SetPoint(graph->GetN(), x, eff2);
    graph -> SetPointError(graph -> GetN()-1, 0, 1./sqrt(h0->GetBinContent(bin)));

    out_file << bin << " " << n_found/eff1 << " " << n_all << endl;
  }

  graph -> SetMarkerStyle(20);
  graph -> SetMarkerSize(0.5);
  graph -> SetFillColor(kGreen);

  auto line = new TLine();
  line -> SetLineStyle(2);
  line -> SetLineColor(kRed);

  auto frame = new TH1D("frame",tag+";Two-hit separation distance (mm);Efficiency",nbin,min,max);
  frame -> SetMaximum(1.05);
  auto frame2 = new TH1D("frame2",";Two-hit separation distance (mm);Efficiency",nbin,min,zoom_max);
  frame2 -> SetMaximum(1.05);

  auto c3 = cc("hit_sep_eff");
  make(frame) -> Draw();
  make(graph) -> Draw("e3");
  make(graph) -> DrawClone("lx");
  line -> DrawLine(0,1,400,1);
  save(c3,"png");

  auto c4 = cc("hit_sep_eff_zoomed");
  make(frame2) -> Draw();
  make(graph) -> DrawClone("e3");
  make(graph) -> DrawClone("lx");
  line -> DrawLine(0,1,20,1);
  //save(c4,"pdf");

  //auto file_temp = new TFile("file_temp.root","recreate");
  //frame -> Write();
  //graph -> Write("graph");
}
