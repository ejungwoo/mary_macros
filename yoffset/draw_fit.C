#include "style.h"
using namespace style;

void draw_fit()
{
  gstat(0);
  fstat(1);

  auto file = new TFile("~/data/spirit/fit_yoffset_layer50.root");
  auto hist = (TH1D *) file -> Get("hist_layer50_row50");
  hist -> SetTitle("layer=50, row=50;dy");
  auto f1 = (TF1 *) file -> Get("f1");

  cc(); make(hist) -> Draw(); make(f1) -> Draw("samel");
}
