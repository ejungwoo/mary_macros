#include "style.h"
using namespace style;

void compare()
{
  zcolor(1);
  gstat(0);
  version("final");

  auto file0 = new TFile("summary_data/overview_yoffset.yoffset_cal2.develop.RC.1612.62c6e62.root");
  auto c1 = cc("yoffset_before");   auto h1 = (TH1D *) file0 -> Get("run2900_yoffset1"); h1 -> SetTitle("After Calibration"); h1-> Draw("colz"); save(c1);
  auto c2 = cc("yoffset_before2D"); auto h2 = (TH2D *) file0 -> Get("run2900_yoffset2"); h2 -> SetTitle("After Calibration"); h2-> Draw("colz"); save(c2);
  //cc(); file0 -> Get("run2900_yoffset3") -> Draw("colz");
  //cc(); file0 -> Get("run2900_yoffset4") -> Draw("colz");
  //cc(); file0 -> Get("run2900_yoffset5") -> Draw("colz");
  //cc(); file0 -> Get("run2900_yoffset6") -> Draw("colz");

  auto file1 = new TFile("summary_data/overview_calibrated.root");
  auto c3 = cc("yoffset_after");   auto h3 = (TH1D *) file1 -> Get("run2900_yoffset1");  h3 -> SetTitle("Before Calibration"); h3 -> Draw("colz"); save(c3);
  auto c4 = cc("yoffset_after2D"); auto h4 = (TH2D *) file1 -> Get("run2900_yoffset2");  h4 -> SetTitle("Before Calibration"); h4 -> Draw("colz"); save(c4);
  //cc(); file1 -> Get("run2900_yoffset3") -> Draw("colz");
  //cc(); file1 -> Get("run2900_yoffset4") -> Draw("colz");
  //cc(); file1 -> Get("run2900_yoffset5") -> Draw("colz");
  //cc(); file1 -> Get("run2900_yoffset6") -> Draw("colz");
}
