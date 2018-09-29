#include "style.h"
using namespace style;

void yoffset2()
{
  auto file = new TFile("~/data/spirit/overview_yoffset.yoffset_cal2.develop.RC.1612.62c6e62.root");

  gstat(0);
  zcolor(1);
  fsave(0);

  TCanvas *c1;
  Int_t idx = 0;
  TH1 *hist;

  for (auto run : {2900,2901,2902,2903,2904,2905,2907})
  {
    c1 = cc2(Form("run%d_calibrated1",run),1100,750); c1 -> Divide(2,2); idx = 0;
    make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset2",run)); hist -> SetTitle("dy vs layer;;"); hist -> Draw("colz");
    make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset3",run)); hist -> SetTitle("dy vs layer;;"); hist -> Draw("colz");
    make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset7",run)); hist -> SetTitle("dy vs dw;;"); hist -> Draw("colz");
    make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset8",run)); hist -> SetTitle("dy vs dw;;"); hist -> Draw("colz");
    save(c1);

    c1 = cc(Form("run%d_calibrated2",run),1100,750); c1 -> Divide(2,2); idx = 0;
    make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset1",run)); hist -> SetTitle("dy;;"); hist -> Draw("colz");
    make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset4",run)); hist -> SetTitle("dy vs row @ layer = 50;;"); hist -> Draw("colz");
    make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset5",run)); hist -> SetTitle("dy vs row @ layer = 30;;"); hist -> Draw("colz");
    make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset6",run)); hist -> SetTitle("dy vs row @ layer = 10;;"); hist -> Draw("colz");
    save(c1);
  }
}
