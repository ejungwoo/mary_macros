#include "style.h"
using namespace style;

void resolution(int idx = 5)
{
  fsave(1);
  gstat(0);
  fstat(1);
  zcolor(1);

  TString tag, title;

  if (idx == 0) {
    tag = "develop.1710.62716a4.original_cov";
    title = "1. Current #sigma^{2}";
  }
  else if (idx == 1) {
    tag = "develop.1710.62716a4.set_cov";
    title = "2. (#angle,l)-dependent #sigma^{2}";
  }
  else if (idx == 2) {
    tag = "develop.1710.62716a4.set_covn";
    title = "3. (n)-dependent #sigma^{2}";
  }
  else if (idx == 3) {
    tag = "develop.1710.62716a4.set_covna";
    title = "4. (#angle,l,n)-dependent #sigma^{2} (+Min. cut)";
  }
  else if (idx == 4) {
    tag = "develop.1710.62716a4.set_covna2";
    title = "5. (#angle,l,n)-dependent #sigma^{2} (+Min. cut) (+BDC)";
  }
  else if (idx == 5) {
    tag = "develop.1733.e8e67e6.original";
    title = "develop.1733 NEW mom-seed";
  }
  else if (idx == 6) {
    tag = "develop.1733.e8e67e6.old_momseed";
    title = "develop.1733 OLD mom-seed";
  }
  else if (idx == 7) {
    tag = "position_calibration2.1731.c6bd233.set_cov";
    title = "cov.1733 OLD mom-seed";
  }
  
  version(tag);

  auto tree = new TChain("events");
  for (auto run : {2894,2900,2901,2902,2903})
  //for (auto run : {2894})
    tree -> AddFile(Form("summary_data/%s/summary_event_run%d.%s.root",tag.Data(),run,tag.Data()));

  auto c1 = cc("vertex_mult");
  auto h1 = tp(tree,"ntracks","","ntracks",title+";track-multiplicity",100,0,100);
  h1 -> Draw();
  h1 -> SetMaximum(6000);
  h1 -> SetStats(1);
  save(c1,"pdf");

  auto hx = tp(tree,"vx:ntracks","","vertex_multx",title+";track-multiplicity;vertex-x",40,30,70,40,-20,20);    /* hx -> SetMaximum(250);*/ cc("mult_vx"); hx -> SetStats(1); hx -> Draw("colz"); save(gPad);
  auto hy = tp(tree,"vy:ntracks","","vertex_multy",title+";track-multiplicity;vertex-y",40,30,70,40,-250,-200);/* hy -> SetMaximum(250);*/ cc("mult_vy"); hy -> SetStats(1); hy -> Draw("colz"); save(gPad);
  auto hz = tp(tree,"vz:ntracks","","vertex_multz",title+";track-multiplicity;vertex-z",40,30,70,40,-30,0);   /* hz -> SetMaximum(250);*/ cc("mult_vz"); hz -> SetStats(1); hz -> Draw("colz"); save(gPad);

  //auto hx = tp(tree,"vx:ntracks","","vertex_multx",title+";track-multiplicity;vertex-x",40,30,70,40,-8,12);      /* hx -> SetMaximum(250);*/ cc("mult_vx"); hx -> SetStats(1); hx -> Draw("colz"); save(gPad);
  //auto hy = tp(tree,"vy:ntracks","","vertex_multy",title+";track-multiplicity;vertex-y",40,30,70,40,-232,-218);/* hy -> SetMaximum(250);*/ cc("mult_vy"); hy -> SetStats(1); hy -> Draw("colz"); save(gPad);
  //auto hz = tp(tree,"vz:ntracks","","vertex_multz",title+";track-multiplicity;vertex-z",40,30,70,40,-18,-8);   /* hz -> SetMaximum(250);*/ cc("mult_vz"); hz -> SetStats(1); hz -> Draw("colz"); save(gPad);

  auto c4 = cc("figure_vertex_resolution_multiplicity"); 
  auto frame = new TH1D("frame",title+";Track multiplicity;Resolution (mm)",100,30,70);
  frame -> SetMaximum(8);
  frame -> SetMinimum(0);
  make(frame) -> Draw();
  auto l = new TLegend();
  auto graphx = fitgsx(hx,8,1.0,1000,1); graphx -> SetMarkerStyle(20); graphx -> SetMarkerColor(kRed);  /*graphx -> Draw("e3");*/ graphx -> Draw("samelpx"); l -> AddEntry(graphx,"x-resolution","flp");
  auto graphy = fitgsx(hy,8,1.0,1000,1); graphy -> SetMarkerStyle(21); graphy -> SetMarkerColor(kBlue); /*graphy -> Draw("e3");*/ graphy -> Draw("samelpx"); l -> AddEntry(graphy,"y-resolution","flp");;
  auto graphz = fitgsx(hz,8,1.0,1000,1); graphz -> SetMarkerStyle(22); graphz -> SetMarkerColor(kGreen+1);   /*graphz -> Draw("e3");*/ graphz -> Draw("samelpx"); l -> AddEntry(graphz,"z-resolution","flp");
  make(l)->Draw();
  save(c4,"pdf");

  auto ccx = cc("vertex_x");
  auto h_vx = tp("vertex_x",tree,"vx","",title+";vertex-x",100,-10,15);
  h_vx -> Draw("colz");
  auto fvx = fitg(h_vx,0.8);
  fvx -> Draw("same");
  save(ccx,"pdf");

  auto ccy = cc("vertex_y");
  auto h_vy = tp("vertex_y",tree,"vy","",title+";vertex-y",100,-235,-215);
  h_vy -> Draw("colz");
  auto fvy = fitg(h_vy,0.8);
  fvy -> Draw("same");
  save(ccy,"pdf");

  auto ccz = cc("vertex_z");
  auto h_vz = tp("vertex_z",tree,"vz","",title+";vertex-z",100,-20,-5);
  h_vz -> Draw("colz");
  auto fvz = fitg(h_vz,0.8);
  fvz -> Draw("same");
  save(ccz,"pdf");
}
