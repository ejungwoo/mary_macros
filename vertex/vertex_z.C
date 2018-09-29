#include "style.h"
using namespace style;

void vertex_z()
{
  gstat(0);

  auto file = new TFile("~/data/spirit/summary_vertex.develop.1534.2537e2e.root");
  auto tree = (TTree *) file -> Get("vertex");
  auto tag = "develop.1534.2537e2e";

  auto draw1 = [tree,tag](TCut cut, TString name, TString title, TString formula, Int_t nx, Double_t x1, Double_t x2, bool logy = 0, bool saveFlag = 0) {
    cout << name << " << cut:" << TString(cut) << ", formula:" << formula << endl;
    auto hist = new TH1D(name,title,nx,x1,x2);
    tree -> Project(name,formula,cut);
    //auto cvs = cc2(name+"."+tag);
    free(make(hist));
    //if (logy) cvs -> SetLogy();
    //if (saveFlag) save(cvs,"png");
    return hist;
  };

  auto cvs = cc3();
  cvs -> SetLogy();

  auto hist = draw1("", "vertex-position", "Vertex position;z (mm);", "vz", 600, -600, 1344, 1);
  hist -> SetMinimum(1);
  hist -> SetMaximum(800000);
  //hist -> SetLineColor(kGray+2);
  hist -> SetLineWidth(1);
  hist -> SetLineColor(kRed+3);
  hist -> SetFillColor(kCyan-10);
  hist -> Draw();

  save(cvs,"pdf");
}
