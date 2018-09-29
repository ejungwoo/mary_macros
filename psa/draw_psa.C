#include "style.h"
using namespace style;

void draw_psa()
{
  fsave(1);
  gstat(0);
  zcolor(0);
  cuttr(0.04);

  version("nusym18");

  auto stpulse = new STPulse("/Users/ejungwoo/spirit/spiritroot/parameters/pulser_117ns_50tb.dat");
  stpulse -> Print();
  auto f1 = stpulse -> GetPulseFunction();
  f1 -> SetParameters(1,0);

  c();
  auto frame = new TH1D("frame",";Time-bucket;Charge (ADC)",100,0,50);
  make(frame) -> Draw();
  make(f1) -> Draw("same");

  auto file = new TFile("../recodata/run2894_s0.reco.position_calibration2.1731.c6bd233.root","read");
  auto tree = (TTree *) file -> Get("cbmsim");
  TClonesArray *rawEventArray = nullptr;
  TClonesArray *hitArray = nullptr;
  TClonesArray *padHitArray = nullptr;
  tree -> SetBranchAddress("STRawEvent",&rawEventArray);
  tree -> SetBranchAddress("STHit",&hitArray);
  tree -> SetBranchAddress("STPadHit",&padHitArray);

  //for (auto event=9; event<10; ++event)
  auto event = 9;
  {
    tree -> GetEntry(event);
    auto rawEvent = (STRawEvent *) rawEventArray -> At(0);

    auto ppm = new STPadPlaneMap();
    auto numHits = hitArray -> GetEntries();
    for (auto iHit=0; iHit <numHits; ++iHit) {
      auto hit = (STHit *) hitArray -> At(iHit);
      ppm -> AddHit(hit);
    }

    Int_t   rows[] = { 60, 61, 41, 31, 31, 31, 41};
    Int_t layers[] = { 21, 40, 39, 41, 29, 20, 14};
    Double_t  x1[] = { 50,100, 30, 50,  0, 50, 50};
    Double_t  x2[] = {200,200,250,150,150,150,250};

    //for (auto row=45; row<55; ++row) for (auto layer=3; layer<5; ++layer) {
    for (auto idx=0; idx<7; ++idx) {
      auto row = rows[idx];
      auto layer = layers[idx];

      auto pad = rawEvent -> GetPad(row,layer);
      Double_t *adc = pad -> GetADC();
      //auto hist = tohist(adc,0,256);
      auto hist = tohist(adc,x1[idx],x2[idx]-1);
      //hist -> SetTitle(Form("Pad(%d,%d);Time-bucket; Charge(ADC)",row,layer));
      hist -> SetTitle(";Time-bucket; Charge(ADC)");
      auto c1 = c(Form("Pad_%d_%d",row,layer)); hist -> Draw();

      vector<STHit *> hits;
      ppm -> GetPad(row,layer) -> GetHits(&hits);
      vector<TF1*> fs;
      for (auto hit : hits) {
        auto fhit = stpulse -> GetPulseFunction(hit);
        make(fhit) -> Draw("same");
        fs.push_back(fhit);
      }
      if (fs.size()!=0) {
        auto graph_sum = sumf(fs);
        graph_sum -> SetLineColor(kBlack);
        graph_sum -> SetLineWidth(3);
        graph_sum -> Draw("same");
      }
      save(c1);
    }
  }
}
