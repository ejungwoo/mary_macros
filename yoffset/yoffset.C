#include "style.h"
using namespace style;

void yoffset()
{
  gstat(0);
  zcolor(1);
  fstat(1);

  TCanvas *c1;
  Int_t idx = 0;
  TH1 *hist;

  if (1)
    //for (auto run : {2900, 2901, 2902, 2903, 2904, 2905})
    for (auto run : {2900})
    {
      auto file = new TFile(Form("~/data/spirit/overview%d_calibrated.root",run),"read");
      file -> ls();
      {
        auto h1 = (TH1 *) file -> Get(Form("run%d_yoffset_calibrated1",run));
        h1 -> SetTitle(";#Deltay (mm);");

        auto f1 = fitg(h1,0.5);

        cc();
        h1 -> Draw("");
        f1 -> Draw("samel");
      }
      //c1 = cc2(Form("run%d_calibrated",run),1100,750); c1 -> Divide(2,2); idx = 0;
      //make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset_calibrated1",run)); hist -> SetTitle("cal. dy;;"); hist -> Draw("");
      //make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset_calibrated2",run)); hist -> SetTitle("cal. dy vs layer;;"); hist -> Draw("colz");
      //make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset_calibrated4",run)); hist -> SetTitle("cal. dy vs row @ layer == 0;;"); hist -> Draw("colz");
      //make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset_calibrated5",run)); hist -> SetTitle("cal. dy vs dw;;"); hist -> Draw("colz");
      //save(c1);
    }

  if (0) {
    //auto file = new TFile("~/data/spirit/overview.root","read");
    auto file = new TFile("~/data/spirit/overview2907.root","read");

    //for (auto run : {2900, 2901, 2902, 2903, 2904, 2905})
    for (auto run : {2907})
    {
      c1 = cc2(Form("run%d_1",run),1100,750); c1 -> Divide(2,2); idx = 0;
      make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset2",run)); hist -> SetTitle("dy vs layer;;"); hist -> Draw("colz");
      make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset3",run)); hist -> SetTitle("dy vs layer;;"); hist -> Draw("colz");
      make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset7",run)); hist -> SetTitle("dy vs dw;;"); hist -> Draw("colz");
      make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset8",run)); hist -> SetTitle("dy vs dw;;"); hist -> Draw("colz");
      save(c1);

      c1 = cc(Form("run%d_2",run),1100,750); c1 -> Divide(2,2); idx = 0;
      make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset1",run)); hist -> SetTitle("dy;;"); hist -> Draw("colz");
      make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset4",run)); hist -> SetTitle("dy vs row @ layer = 50;;"); hist -> Draw("colz");
      make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset5",run)); hist -> SetTitle("dy vs row @ layer = 30;;"); hist -> Draw("colz");
      make(c1 -> cd(++idx)); hist = (TH1 *) file -> Get(Form("run%d_yoffset6",run)); hist -> SetTitle("dy vs row @ layer = 10;;"); hist -> Draw("colz");
      save(c1);
    }
  }
}
