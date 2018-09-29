#include "style.h"
using namespace style;

void psa_step()
{
  fsave(1);
  gstat(0);
  zcolor(0);
  cuttr(0.04);
  version("nusym18");

  auto stpulse = new STPulse("/Users/ejungwoo/spirit/spiritroot/parameters/pulser_117ns_50tb.dat");
  auto file = new TFile("../recodata/run2894_s0.reco.position_calibration2.1731.c6bd233.root","read");
  auto tree = (TTree *) file -> Get("cbmsim");
  TClonesArray *rawEventArray = nullptr;
  TClonesArray *hitArray = nullptr;
  tree -> SetBranchAddress("STRawEvent",&rawEventArray);
  tree -> SetBranchAddress("STHit",&hitArray);

  tree -> GetEntry(9);
  auto rawEvent = (STRawEvent *) rawEventArray -> At(0);

  auto ppm = new STPadPlaneMap();
  auto numHits = hitArray -> GetEntries();
  for (auto iHit=0; iHit <numHits; ++iHit) {
    auto hit = (STHit *) hitArray -> At(iHit);
    ppm -> AddHit(hit);
  }

  auto row = 31;
  auto layer = 41;
  auto ii=0;
  auto ff=256;
  auto ii2=50;
  auto ff2=150;
  auto colorRM = kGreen;

  vector<STHit *> hits;
  ppm -> GetPad(row,layer) -> GetHits(&hits);

  /*****************/

  TCanvas *c1;

  auto pad = rawEvent -> GetPad(row,layer);
  Double_t *adc = pad -> GetADC();
  auto hist = tohist(adc,ii,ff);
  hist -> SetTitle(";Time-bucket;Charge (ADC)");
  hist -> SetMaximum(560);
  hist -> GetXaxis() -> SetRangeUser(ii2,ff2);
  hist -> SetLineColor(kBlack);
  c1 = c("psa_raw"); hist -> Draw();
  save(c1);

  /*****************/

  c1 = c("psa_fit1"); hist -> DrawCopy();
  auto fhit1 = stpulse -> GetPulseFunction(hits.at(0));
  make(fhit1) -> Draw("same");
  save(c1);

  /*****************/

  hist = (TH1D *) hist -> Clone("h1");
  for (auto i=ii; i<ff; ++i) hist -> SetBinContent(i+1,hist->GetBinContent(i+1)-fhit1->Eval(i+0.5));
  hist -> SetMaximum(560);
  hist -> GetXaxis() -> SetRangeUser(ii2,ff2);
  c1 = c("psa_fit1_remove"); hist -> DrawCopy();
  fhit1 = (TF1 *) fhit1 -> Clone(); fhit1 -> SetLineColor(colorRM); fhit1 -> SetLineStyle(1); make(fhit1) -> Draw("same");
  save(c1);

  /*****************/

  c1 = c("psa_fit2"); hist -> DrawCopy();
  auto fhit2 = stpulse -> GetPulseFunction(hits.at(1));
  make(fhit2) -> Draw("same");
  save(c1);

  /*****************/

  hist = (TH1D *) hist -> Clone("h1");
  for (auto i=ii; i<ff; ++i) hist -> SetBinContent(i+1,hist->GetBinContent(i+1)-fhit2->Eval(i+0.5));
  hist -> SetMaximum(560);
  hist -> GetXaxis() -> SetRangeUser(ii2,ff2);
  c1 = c("psa_fit2_remove"); hist -> DrawCopy();
  fhit2 = (TF1 *) fhit2 -> Clone(); fhit2 -> SetLineColor(colorRM); fhit2 -> SetLineStyle(1); make(fhit2) -> Draw("same");
  save(c1);

  /*****************/

  c1 = c("psa_fit3"); hist -> DrawCopy();
  auto fhit3 = stpulse -> GetPulseFunction(hits.at(2));
  make(fhit3) -> Draw("same");
  save(c1);

  /*****************/

  hist = (TH1D *) hist -> Clone("h1");
  for (auto i=ii; i<ff; ++i) hist -> SetBinContent(i+1,hist->GetBinContent(i+1)-fhit3->Eval(i+0.5));
  hist -> SetMaximum(560);
  hist -> GetXaxis() -> SetRangeUser(ii2,ff2);
  c1 = c("psa_fit3_remove"); hist -> Draw();
  save(c1);
}
