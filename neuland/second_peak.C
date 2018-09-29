#include "style.h"
using namespace style;

void second_peak()
{
  fsave(1);
  gstat(0);
  version("trail1");

  auto file1 = new TFile("data/second_peak_1s_11s.108Sn_trial1.root");
  auto file2 = new TFile("data/second_peak_42s_50s.108Sn_trial1.root");
  auto file3 = new TFile("data/second_peak_51s_61s.108Sn_trial1.root");
  auto file4 = new TFile("data/second_peak_92s_100s.108Sn_trial1.root");
  auto file5 = new TFile("data/second_peak_by_layer.108Sn_trial1.root");

  //file1 -> ls();
  //file2 -> ls();
  //file3 -> ls();
  //file4 -> ls();
  //file5 -> ls();

  {
    auto cvs = cc("second_peak_bar_id");
    auto h1 = (TH1 *) file1 -> Get("barid");
    h1 -> SetMinimum(0);
    h1 -> SetTitle(";Bar ID");
    make(h1) -> Draw();
    save(cvs);
  }

  {
    auto cvs = cc("second_peak_tof");
    auto h1 = (TH1 *) file1 -> Get("tof");
    h1 -> SetMinimum(0);
    h1 -> SetTitle(";TOF");
    make(h1) -> Draw();
    save(cvs);
  }

  {
    auto cvs = cc3("second_peak_tof_bar");
    cvs -> Divide(3,3,0,0);
    Int_t cdcount = 0;
    for (auto bar_id = 1; bar_id <= 9; ++bar_id) {
      auto h1 = (TH1 *) file1 -> Get(Form("tof_bar%d",bar_id));
      h1 -> SetTitle(Form("BarID = %d + 100*X;TOF",bar_id));
      cvs -> cd(++cdcount);
      make(h1) -> Draw();
    }
    save(cvs);
  }

  {
    auto cvs = cc3("second_peak_tof_bar2");
    cvs -> Divide(3,3,0,0);
    Int_t cdcount = 0;
    for (auto bar_id = 42; bar_id <= 50; ++bar_id) {
      auto h1 = (TH1 *) file2 -> Get(Form("tof_bar%d",bar_id));
      h1 -> SetTitle(Form("BarID = %d + 100*X;TOF",bar_id));
      cvs -> cd(++cdcount);
      make(h1) -> Draw();
    }
    save(cvs);
  }

  {
    auto cvs = cc3("second_peak_tof_bar3");
    cvs -> Divide(3,3,0,0);
    Int_t cdcount = 0;
    for (auto bar_id = 51; bar_id <= 59; ++bar_id) {
      auto h1 = (TH1 *) file3 -> Get(Form("tof_bar%d",bar_id));
      h1 -> SetTitle(Form("BarID = %d + 100*X;TOF",bar_id));
      cvs -> cd(++cdcount);
      make(h1) -> Draw();
    }
    save(cvs);
  }

  {
    auto cvs = cc3("second_peak_tof_bar4");
    cvs -> Divide(3,3,0,0);
    Int_t cdcount = 0;
    for (auto bar_id = 92; bar_id <= 100; ++bar_id) {
      auto h1 = (TH1 *) file4 -> Get(Form("tof_bar%d",bar_id));
      h1 -> SetTitle(Form("BarID = %d + 100*X;TOF",bar_id));
      cvs -> cd(++cdcount);
      make(h1) -> Draw();
    }
    save(cvs);
  }

  {
    auto cvs = cc3("second_peak_tof_layer");
    cvs -> Divide(3,3,0,0);
    Int_t cdcount = 0;
    for (auto layer = 0; layer < 8; ++layer) {
      auto h1 = (TH1 *) file5 -> Get(Form("tof_layer%d",layer));
      h1 -> SetTitle(Form("Layer %d",layer));
      cvs -> cd(++cdcount);
      make(h1) -> Draw();
    }
    save(cvs);
  }
}
