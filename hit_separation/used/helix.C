#include "style.h"
using namespace style;

void helix()
{
  auto tree0 = new TChain("helix");
  for (auto split = 0; split < 38; ++split)
    tree0 -> Add(Form("~/data/spirit/run2894_s%d.hit_sep.develop.RC.1612.62c6e62.hs_task2.root",split));

  cc(); make(tp("nn",tree0,"nfound:npad","nfound!=0&&npad>30",";npad;nfound",120,0,120,120,0,120))->Draw("colz");
  cc(); make(tp("eff",tree0,"(nfound)/npad:npad","nfound!=0&&npad>30",";npad;eff",40,0,120,40,0,1))->Draw("colz");
  cc(); make(tp("cut",tree0,"(nfound)/npad","nfound!=0&&npad>30",";eff",120,0,1))->Draw();

  auto hh_all = tp("npad",tree0,"npad","npad!=0","",120,0,120);
  auto hh_found = tp("nfound",tree0,"nfound","nfound!=0","",120,0,120);

  cc();
  hh_all -> SetLineColor(kBlue);
  hh_all -> Draw();
  hh_found -> SetLineColor(kRed);
  hh_found -> Draw("same");

  auto g = new TGraph();
  for (auto bin = 1; bin <= 120; ++bin)
  {
    auto n_all = hh_all -> GetBinContent(bin);
    auto n_found = hh_found -> GetBinContent(bin);

    auto x = hh_all -> GetXaxis() -> GetBinCenter(bin);
    auto eff = Double_t(n_found)/n_all;
    cout << x << " " << eff << endl;

    g -> SetPoint(g -> GetN(), x, eff);
  }

  auto hh = new TH1D("hh","",120,0,120);//";two hit separation (mm);efficiency",nbin,min,max);
  hh -> SetMaximum(1.);
  g -> SetMarkerStyle(20);
  g -> SetMarkerSize(0.5);

  auto cvs = cc("hh");
  make(hh) -> Draw();
  make(g) -> Draw("samep");
}
