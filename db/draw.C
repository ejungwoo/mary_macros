#include "style.h"
using namespace style;

void draw()
{
  gstat(0);
  version("run2894_0");

  auto file_found = new TFile("cut_data.root","read");
  auto tree_found = (TTree *) file_found -> Get("found");
  Short_t row, layer;
  Int_t event, recoid1, recoid2, trueid1, trueid2;
  Bool_t found;
  Float_t q1, q2, dreco, dtrue;
  tree_found -> SetBranchAddress("found",&found);
  tree_found -> SetBranchAddress("event",&event);
  tree_found -> SetBranchAddress("row",&row);
  tree_found -> SetBranchAddress("layer",&layer);
  tree_found -> SetBranchAddress("recoid1",&recoid1);
  tree_found -> SetBranchAddress("recoid2",&recoid2);
  tree_found -> SetBranchAddress("trueid1",&trueid1);
  tree_found -> SetBranchAddress("trueid2",&trueid2);
  tree_found -> SetBranchAddress("q1",&q1);
  tree_found -> SetBranchAddress("q2",&q2);
  tree_found -> SetBranchAddress("dreco",&dreco);
  tree_found -> SetBranchAddress("dtrue",&dtrue);
  tree_found -> Print();

  auto file_reco = new TFile("run2894_s0.reco.position_calibration2.1731.c6bd233.root","read");
  auto tree_reco = (TTree *) file_reco -> Get("cbmsim");
  TClonesArray *rawEventArray = nullptr;
  TClonesArray *hitArray = nullptr;
  TClonesArray *padHitArray = nullptr;
  tree_reco -> SetBranchAddress("STRawEvent",&rawEventArray);
  tree_reco -> SetBranchAddress("STHit",&hitArray);
  tree_reco -> SetBranchAddress("STPadHit",&padHitArray);

  auto line_true = new TArrow(); line_true -> SetLineColor(kPink);
  auto line_reco = new TArrow(); line_reco -> SetLineColor(kBlue);

  auto ytotb = 1./(-40*5.43/100);

  auto n = tree_found -> GetEntries();
  for (auto iFound = 0; iFound < n; ++iFound)
  {
    tree_found -> GetEntry(iFound);
    tree_reco -> GetEntry(event);

    if (found || dtrue < 50.)
      continue;

    cout << iFound << ": " << (found?"found ":"not-found ") << event << " (" << row << "," << layer << ") d=" << dtrue << " qdiff=" << abs(q1-q2) << " <q>=" << (q1+q2)/2. << endl;

    auto rawEvent = (STRawEvent *) rawEventArray -> At(0);
    auto pad = (STPad *) rawEvent -> GetPad(row,layer);
    Double_t *adc = pad -> GetADC();
    TString name = Form("sample%d",iFound);
    TString title = Form("sample%d",iFound);
    //TString title = Form("sample%d : %s",iFound,found?"found":"not found");
    auto hist = tohist(adc,256,name,title+";Time bucket;Charge (ADC)");
    hist -> SetLineColor(kBlack);

    auto cvs = c(name);
    hist -> Draw();

    auto chargemax = 1.1*max(hist);

    vector<Double_t> charges;
    for (auto hitID : {trueid1,trueid2}) {
      if (hitID < 0)
        continue;
      auto hit = (STHit *) padHitArray -> At(hitID);
      auto charge = hit -> GetCharge();
      auto arrowsize = 0.03;
      charges.push_back(charge);
      if (charge > chargemax) {
        charge = chargemax;
        arrowsize = 0.08;
      }
      auto y = hit -> GetY();
      auto tb = y*ytotb;
      line_true -> DrawArrow(tb,0,tb,charge,arrowsize,">");
    }

    for (auto hitID : {recoid1,recoid2}) {
      if (hitID < 0)
        continue;
      auto hit = (STHit *) hitArray -> At(hitID);
      auto charge = hit -> GetCharge();
      auto arrowsize = 0.03;
      if (charge > chargemax) {
        charge = chargemax;
        arrowsize = 0.08;
      }
      auto tb = hit -> GetTb();
      line_reco -> DrawArrow(tb,charge,tb,charge*(1.1),arrowsize,"<");
    }

    auto l = new TLegend();
    l -> AddEntry((TObject *)nullptr,Form("row%d,layer%d",row,layer),"");
    l -> AddEntry(hist,"raw data","l");
    l -> AddEntry(line_true,"reco track position","l");
    l -> AddEntry(line_true,Form("Q1,Q2=%.0f,%.0f",charges[0],charges[1]),"");
    l -> AddEntry(line_reco,"reco hit position","l");
    make(l) -> Draw();

    save(cvs);
  }
}
