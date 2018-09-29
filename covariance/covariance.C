#include "style.h"
using namespace style;

void covariance(int idx)
{
  gstat(0);
  zcolor(1);
  fsave(0);

  auto fsave = false;
  auto fwrite = true;

  TString tata = "TMath::Tan(TMath::Pi()-alpha)*TMath::Tan(TMath::Pi()-alpha)";
  TString tltl = "TMath::Tan(lambda)*TMath::Tan(lambda)";

  TString tag = "position_calibration.1612.62c6e62.cov_test2";
  version(tag);

  auto tree = new TChain("cluster");
  for (auto i = 0; i < 38; ++i) tree -> Add(Form("cluster_cov_data/run2894_s%d.cluster_cov.%s.root",i,tag.Data()));
  for (auto i = 0; i < 35; ++i) tree -> Add(Form("cluster_cov_data/run2900_s%d.cluster_cov.position_calibration.1612.62c6e62.cov_test2.root",i));
  for (auto i = 0; i < 36; ++i) tree -> Add(Form("cluster_cov_data/run2901_s%d.cluster_cov.position_calibration.1612.62c6e62.cov_test2.root",i));

  /******************************************************************************/

  /*
  auto file0 = new TFile("data/drawings.position_calibration.1612.62c6e62.cov_test2.root","recreate");

  auto hist_tltl = tp("hist_tltl",tree,tltl,"","tan^{2}#lambda",200,0,1); hist_tltl -> Write();
  auto hist_tata = tp("hist_tata",tree,tata,"","tan^{2}#alpha",200,0,1000); hist_tata -> Write();
  auto hist_l    = tp("hist_l",tree,"l","","Drift length (mm)",200,0,500); hist_l -> Write(); 

  auto h_dydl = tp("h_dydl",tree,"l:dy","",";dy;Drift length (mm)",200,-4,4,200,100,500); make(h_dydl) -> Write();
  auto g_dydl = fitgsy(h_dydl,50,0.5); make(g_dydl) -> Write();

  auto h_dytl = tp("h_dytl",tree,tltl+":dy","",";dy;tan^{2}#lambda",200,-4,4,200,0,1); make(h_dytl) -> Write();
  auto g_dytl = fitgsy(h_dytl,50,0.5); make(g_dytl) -> Write();

  auto h_dxdl = tp("h_dxdl",tree,"l:dx","isLayer",";dx;Drift length (mm)",200,-4,4,200,100,500); make(h_dxdl) -> Write();
  auto g_dxdl = fitgsy(h_dxdl,50,0.5); make(g_dxdl) -> Write();

  auto h_dxta = tp("h_dxta",tree,tata+":dx","isLayer",";dx;tan^{2}#alpha",200,-4,4,200,0,1000); make(h_dxta) -> Write();
  auto g_dxta = fitgsy(h_dxta,50,0.5); make(g_dxta) -> Write();

  auto h_dzdl = tp("h_dzdl",tree,"l:dz","!isLayer",";dz;Drift length (mm)",200,-4,4,200,100,500); make(h_dzdl) -> Write();
  auto g_dzdl = fitgsy(h_dzdl,50,0.5); make(g_dzdl) -> Write();

  auto h_dzta = tp("h_dzta",tree,tata+":dz","!isLayer",";dz;tan^{2}#alpha",200,-4,4,200,0,2); make(h_dzta) -> Write();
  auto g_dzta = fitgsy(h_dzta,50,0.5); make(g_dzta) -> Write();

  file0 -> Cloes();
  */

  /******************************************************************************/

  auto ntata = 20;


  //auto tata_min = 0.;
  //auto tata_max = 1000;
  auto tata_min = 0.;
  auto tata_max = 0.5;
  auto dtata = (tata_max-tata_min)/ntata;



  if (idx == 0)
  {
    TString var = "dy";
    auto file = new TFile("data/dy.position_calibration.1612.62c6e62.cov_test2.root","recreate");
    auto graph = new TGraph2D();
    graph -> SetName(var+"_cov2D");

    auto nl = 10;
    auto l_min = 100.;
    auto l_max = 500.;
    auto dl = (l_max-l_min)/nl;

    auto ntltl = 10;
    auto tltl_min = 0.;
    auto tltl_max = 0.5;
    auto dtltl = (tltl_max-tltl_min)/ntltl;

    for (auto l1=l_min; l1<l_max; l1+=dl)
    {
      if (l1 > 180 && l1 < 270)
        continue;

      auto l2 = l1+dl;
      auto l_mid = (l2+l1)/2.;

      for (auto tltl1=tltl_min; tltl1<tltl_max; tltl1+=dtltl)
      {
        auto tltl2 = tltl1+dtltl;
        auto tltl_mid = (tltl2+tltl1)/2.;

        TString name = Form("l=%.2f_tan#lambda=%.2f",l_mid,tltl_mid);
        const char *cut_var1 = "l";
        const char *cut_var2 = tltl.Data();
        TCut cut = Form("%s>%.2f&&%s<%.2f&&%s>%.2f&&%s<%.2f",cut_var1,l1,cut_var1,l2,cut_var2,tltl1,cut_var2,tltl2);

        auto h1 = tp(name,tree,var,cut,name+";"+var+" Offset (mm);",200,-4,4);
        if (h1 -> GetEntries() < 1000)
          continue;

        auto f1 = fitg(h1, 0.5);
        graph -> SetPoint(graph -> GetN(), l_mid, tltl_mid, f1 -> GetParameter(2));

        cout << var << " " << l_mid << " " << tltl_mid << " " << f1 -> GetParameter(2) << endl;
      }
    }
    file -> cd();
    graph -> Write();
  }
  else if (idx == 1)
  {
    TString var = "dx";
    auto file = new TFile("data/dx.position_calibration.1612.62c6e62.cov_test2.root","recreate");
    auto graph = new TGraph2D();
    graph -> SetName(var+"_cov2D");

    for (auto l1=l_min; l1<l_max; l1+=dl)
    {
      if (l1 > 180 && l1 < 270)
        continue;
      auto l2 = l1+dl;
      auto l_mid = (l2+l1)/2.;

      for (auto tata1=tata_min; tata1<tata_max; tata1+=dtata)
      {
        auto tata2 = tata1+dtata;
        auto tata_mid = (tata2+tata1)/2.;

        TString name = Form("l=%.2f_tan#alpha=%.2f",l_mid,tata_mid);
        const char *cut_var1 = "l";
        const char *cut_var2 = tata.Data();
        TCut cut = Form("isLayer&&%s>%.2f&&%s<%.2f&&%s>%.2f&&%s<%.2f",cut_var1,l1,cut_var1,l2,cut_var2,tata1,cut_var2,tata2);

        auto h1 = tp(name,tree,var,cut,name+";"+var+" Offset (mm);",200,-4,4);
        if (h1 -> GetEntries() < 1000)
          continue;

        auto f1 = fitg(h1, 0.5);
        graph -> SetPoint(graph -> GetN(), l_mid, tata_mid, f1 -> GetParameter(2));

        cout << var << " " << l_mid << " " << tata_mid << " " << f1 -> GetParameter(2) << endl;
      }
    }
    file -> cd();
    graph -> Write();
  }
  else if (idx == 2)
  {
    TString var = "dz";
    auto file = new TFile("data/dz.position_calibration.1612.62c6e62.cov_test2.root","recreate");
    auto graph = new TGraph2D();
    graph -> SetName(var+"_cov2D");

    for (auto l1=l_min; l1<l_max; l1+=dl)
    {
      if (l1 > 180 && l1 < 270)
        continue;
      auto l2 = l1+dl;
      auto l_mid = (l2+l1)/2.;

      for (auto tata1=tata_min; tata1<tata_max; tata1+=dtata)
      {
        auto tata2 = tata1+dtata;
        auto tata_mid = (tata2+tata1)/2.;

        TString name = Form("l=%.2f_tan#alpha=%.2f",l_mid,tata_mid);
        const char *cut_var1 = "l";
        const char *cut_var2 = tata.Data();
        TCut cut = Form("!isLayer&&%s>%.2f&&%s<%.2f&&%s>%.2f&&%s<%.2f",cut_var1,l1,cut_var1,l2,cut_var2,tata1,cut_var2,tata2);

        auto h1 = tp(name,tree,var,cut,name+";"+var+" Offset (mm);",200,-4,4);
        if (h1 -> GetEntries() < 1000)
          continue;

        auto f1 = fitg(h1, 0.5);
        graph -> SetPoint(graph -> GetN(), l_mid, tata_mid, f1 -> GetParameter(2));

        cout << var << " " << l_mid << " " << tata_mid << " " << f1 -> GetParameter(2) << endl;
      }
    }
    file -> cd();
    graph -> Write();
  }
}
