#include "style.h"
using namespace style;

void draw_event()
{
  gstat(0);
  zcolor(0);
  cuttr();
  version("nusym18");

  auto file = new TFile("../recodata/run2894_s0.reco.position_calibration2.1731.c6bd233.root","read");
  auto tree = (TTree *) file -> Get("cbmsim");
  TClonesArray *rawEventArray = nullptr;
  TClonesArray *hitArray = nullptr;
  TClonesArray *padHitArray = nullptr;
  tree -> SetBranchAddress("STRawEvent",&rawEventArray);
  tree -> SetBranchAddress("STHit",&hitArray);
  tree -> SetBranchAddress("STPadHit",&padHitArray);

  for (auto event=9; event<10; ++event) {
  //for (auto event=0; event<10; ++event) {
    tree -> GetEntry(event);
    TString name = Form("sample_event%d",event);
    auto hist = new TH2D(name,";z (mm);x (mm)",112,0,1344,108,-432,432);
    hist -> SetMaximum(4000);
    hist -> SetContour(50);
    auto rawEvent = (STRawEvent *) rawEventArray -> At(0);
    for (auto row=0; row<108; ++row) {
      for (auto layer=0; layer<112; ++layer) {
        auto pad = rawEvent -> GetPad(row,layer);
        Double_t *adc = pad -> GetADC();
        auto gbin = hist -> GetBin(layer+1,row+1);
        hist -> SetBinContent(gbin,max(adc,256));
      }
    }
    auto cvs = cc2(name);
    make(hist) -> Draw("col");
    save(cvs);
  }
}
