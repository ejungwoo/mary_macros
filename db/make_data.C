#include "style.h"
#include "../hit_separation/global_cut.h"
using namespace style;

void make_data()
{
  version(gvname);

  auto file_found = new TFile("run2894_s0.hit_sep.position_calibration2.1731.c6bd233.root","read");
  auto tree = (TTree *) file_found -> Get("found");

  auto newfile = new TFile("cut_data.root","recreate");
  auto newtree = tree -> CopyTree(cut_2hit);
  newfile -> cd();
  newtree -> Write();
}
