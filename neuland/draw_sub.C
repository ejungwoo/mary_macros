#include "functions.h"
#include "style.h"
using namespace style;

void draw_sub()
{
  fsave(1);
  zcolor(1);
  version("sub_test");

  auto zmax = 10000;
  auto zmin = 1;

  auto file0 = new TFile("/Users/ejungwoo/np/data/histograms_edep_tof.root");
  file0 -> ls();

  auto hist1 = inv((TH1D *) file0 -> Get("h_edep_tof_all_is1"));
  auto hist0 = inv((TH1D *) file0 -> Get("h_edep_tof_all_is0"));

  cc("tofe_nc"); hist1 -> SetMaximum(zmax); hist1 -> SetMinimum(zmin); hist1 -> Draw("colz"); gPad -> SetLogz(); save(gPad);
  cc("tofe_n"); hist0 -> SetMaximum(zmax); hist0 -> SetMinimum(zmin); hist0 -> Draw("colz"); gPad -> SetLogz(); save(gPad);

  auto n1 = hist1 -> GetEntries();
  auto n0 = hist0 -> GetEntries();
  auto nn = n1 + n0;

  cout << n1 << " " << Double_t(n1)/nn << endl;
  cout << n0 << " " << Double_t(n0)/nn << endl;

  auto file = new TFile("/Users/ejungwoo/np/data/h_neutron_subtracted_Inv.nsub2.root");
  auto hist = (TH2D *) file -> Get("h_neutron_subtracted_Inv");

  auto cvs = cc("tofe_c");
  hist -> Draw("colz");
  hist -> SetMinimum(zmin);
  hist -> SetMaximum(zmax);
  cvs -> SetLogz();
  save(cvs);

  cout << hist -> GetEntries() << " " << Double_t(hist->GetEntries())/nn << endl;

  auto file_add = new TFile("/Users/ejungwoo/np/data/h_neutron_added_Inv.nsub2.root");
  auto hist_add = (TH2D *) file_add -> Get("h_neutron_added_Inv");

  auto cvs_add = cc("tofe_nn");
  hist_add -> Draw("colz");
  hist_add -> SetMinimum(zmin);
  hist_add -> SetMaximum(zmax);
  cvs_add -> SetLogz();
  save(cvs_add);

  cout << hist_add -> GetEntries() << " " << Double_t(hist_add->GetEntries())/nn << endl;

  for (const char *cut_name : {"cut_proton","cut_deutron","cut_triton"}) {
    auto cut = cutg(Form("data/%s.root",cut_name),cut_name,"x","y");
    auto histc = (TH2D *) cutg(hist,cut);
    histc -> SetTitle(cut_name);
    histc -> SetMinimum(zmin);
    histc -> SetMaximum(zmax);

    auto cvs2 = cc(TString("tofe_")+cut_name);
    histc -> Draw("colz");
    cvs2 -> SetLogz(); 
    save(cvs2);

    cout << histc -> GetEntries() << " " << Double_t(histc->GetEntries())/nn << endl;
  }
}
