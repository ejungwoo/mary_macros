#include "style.h"
using namespace style;

void simple(int idx = 0)
{
  gstat(0);
  zcolor(1);
  fsave(1);
  fwrite(1);

  version("n4");

  auto cSigma = 0.8;
  Double_t integral_cut = 2000;

  //TCut globalcut = "n>1";
  TCut globalcut = "n>3"; //integral_cut = 1000;

  Int_t nbins = 100;
  Int_t bin35 = (Int_t)(nbins*0.35);
  Int_t bin0 = 1+bin35;
  Int_t bin1 = nbins-bin35;

  auto tree = new TChain("cluster");
  for (auto i = 0; i < 38; ++i) tree -> Add(Form("cluster_cov_data/run2894_s%d.cluster_cov.position_calibration.1612.62c6e62.cov_test2.root",i));
  for (auto i = 0; i < 35; ++i) tree -> Add(Form("cluster_cov_data/run2900_s%d.cluster_cov.position_calibration.1612.62c6e62.cov_test2.root",i));
  for (auto i = 0; i < 36; ++i) tree -> Add(Form("cluster_cov_data/run2901_s%d.cluster_cov.position_calibration.1612.62c6e62.cov_test2.root",i));
  for (auto i = 0; i < 34; ++i) tree -> Add(Form("cluster_cov_data/run2902_s%d.cluster_cov.position_calibration.1612.62c6e62.cov_test2.root",i));
  for (auto i = 0; i < 35; ++i) tree -> Add(Form("cluster_cov_data/run2902_s%d.cluster_cov.position_calibration.1612.62c6e62.cov_test2.root",i));
  for (auto i = 0; i < 41; ++i) tree -> Add(Form("cluster_cov_data/run2903_s%d.cluster_cov.position_calibration.1612.62c6e62.cov_test2.root",i));

  auto draw = [tree,globalcut,cSigma,nbins,bin0,bin1,integral_cut](TString var0, TString angle_name, TString angle_var, Double_t dangle, Double_t angle0, Double_t angle1)
  {
    TString rootname = Form("hist%s",var0.Data());
    auto graph_summary = new TGraph2D();
    graph_summary -> SetName(Form("graph_summary_%s",var0.Data()));
    graph_summary -> GetHistogram() -> SetTitle(Form(";Dirft length (mm); tan^{2}#%s",angle_name.Data()));

    for (auto angle = angle0; angle < angle1; angle+=dangle) {
      TString hname = Form("d%sdl_%.2f",var0.Data(),angle);
      TString fname = Form("d%sdl_%.2f_fit",var0.Data(),angle);
      TString title = Form("tan^{2}#%s=%.2f~%.2f;d%s;Drift length (mm)",angle_name.Data(),angle,angle+dangle,var0.Data());

      TCut cut = globalcut+TCut(Form("%s>%.2f&&%s<%.2f",angle_var.Data(),angle,angle_var.Data(),angle+dangle));
      if (var0 == "x") cut = cut + TCut("isLayer");
      if (var0 == "z") cut = cut + TCut("!isLayer");

      auto angle_rep = (angle+angle+dangle)/2;
      auto hist = tp(hname,tree,Form("l:d%s",var0.Data()),cut,title,nbins,-4,4,nbins,100,450);
      if (hist -> GetEntries() < 30000)
        continue;
      write(rootname,hist);

      auto fit_array = fitgsy_list(hist,10,cSigma);
      auto n = fit_array->GetEntries();
      auto graph_fit = new TGraphErrors();
      graph_fit -> SetName(fname);

      for (auto i = 0; i < n; ++i)
      {
        auto data = (style::hdata *) fit_array -> At(i);
        auto sigma = data -> dx;
        if (sigma < 0.01)
          continue;

        auto driftLength = data->y;
        if (var0=="y"&&angle>0.01&&driftLength>150&&driftLength<260) continue;
        if (var0=="y"&&angle>0.14&&driftLength>140&&driftLength<180) continue;

        auto h1 = data->hist;
        if (h1->Integral(bin0,bin1) > integral_cut)
        {
          graph_summary -> SetPoint(graph_summary->GetN(),driftLength,angle_rep,sigma*sigma);
          write(rootname,h1);

          graph_fit->SetPoint(graph_fit->GetN(),data->x,data->y);
          graph_fit->SetPointError(graph_fit->GetN()-1,data->dx,data->dy/2.);
        }
      }
      write(rootname,graph_fit);
    }
    write(graph_summary);
  };

  if (idx == 0) draw("y", "lambda", "tltl", 0.05, 0., 0.45);
  if (idx == 1) draw("x", "alpha", "tata",  0.05, 0., 0.45);
  if (idx == 2) draw("z", "alpha", "tata",    1., 2.,   7.);
}
