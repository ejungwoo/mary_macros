#include "style.h"
using namespace style;

#include "global_cut.h"

void make_cut_data()
{
  //TString vname = "depend_y_lownpad";
  //TString vname = "first_trial";
  //TString vname = "ps2";
  //TString vname = "ps2";
  version(gvname);

  TString tag = "position_calibration2.1731.c6bd233.hit_sep2";
  auto tree = new TChain("found");
  for (auto i = 0; i < 38; ++i)
    tree -> Add(Form("summary_twohit_data/%s/run2894_s%d.hit_sep.%s.root",tag.Data(),i,tag.Data()));

  auto newfile = new TFile(Form("data/hit_sep_data.%s.root",gvname.Data()),"recreate");
  auto newtree = tree -> CopyTree(cut_2hit);
  newfile -> cd();
  newtree -> Write();
}
