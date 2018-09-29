#include "style.h"
using namespace style;

void edep_tof()
{
  gstat(0);
  fstat(1);
  zcolor(1);
  fsave(0);

  version("etof");

  bool tof_start_at_20 = false;
  bool ignore_below_35 = true;
  bool draw_n_distributions = false;
  bool draw_all_xcut = false;

  auto file = new TFile("data/histograms_edep_tof.root");
  
  auto hraw = (TH2D *) file -> Get("h_edep_tof");
  auto hall = (TH2D *) file -> Get("h_edep_tof_all_is0");
  auto hnal = (TH2D *) file -> Get("h_edep_tof_all_is1");

  auto c_raw = cc(hraw->GetName()); hraw -> Draw("colz"); gPad -> SetLogz(); save(c_raw);
  auto c_all = cc(hall->GetName()); hall -> Draw("colz"); hall -> SetTitle("veto_all==0 -> No charged particle;edep;TOF"); gPad -> SetLogz(); save(c_all);
  auto c_nal = cc(hnal->GetName()); hnal -> Draw("colz"); hnal -> SetTitle("veto_all==1 -> At least 1 charged particle;edep;TOF"); gPad -> SetLogz(); save(c_nal);

  auto hall_y = make(cutx(hraw,1,251));

  auto nbins = 250;
  //auto dbin = 2;
  auto dbin = 5;
  auto binmin = 1;
  //auto binmax = 90;
  //auto binmax = 30;
  //auto binmin = 26;
  //auto binmax = 76;
  auto binmax = 150;

  if (tof_start_at_20) {
    hraw -> GetXaxis() -> SetRangeUser(20,500); 
    hall -> GetXaxis() -> SetRangeUser(20,500); 
    hnal -> GetXaxis() -> SetRangeUser(20,500); 
  }

  Color_t colors[] = {
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
    kOrange+4,kRed,kPink+10,kMagenta+1,kViolet-3,kBlue,kAzure-2,kCyan-4,kTeal+8,kGreen+2,kYellow-3,kYellow-7,
  };

  TCanvas *c_alls = nullptr;
  TLegend *lg = nullptr;
  if (draw_n_distributions) {
    c_alls = cc();
    hall_y -> SetLineColor(colors[0]);
    hall_y -> SetLineWidth(2);
    if (ignore_below_35)
      for (auto i = 1; i <= 35; ++i)
        hall_y -> SetBinContent(i,0);

    norm_max(hall_y) -> Draw();
    lg = new TLegend();
    lg -> AddEntry(hall_y,"Raw","l");
  }

  auto hsub_all = new TH1D("hsub_all","",200,0,200);
  auto graph = new TGraphErrors();
  auto count = 1;
  for (auto bin1 = binmin; bin1 < binmax; bin1+=dbin)
  {
    auto bin2 = bin1 + dbin-1;

    auto x1 = hall->GetXaxis()->GetBinLowEdge(bin1);
    auto x2 = hall->GetXaxis()->GetBinUpEdge(bin2);

    auto boxraw = new TGraph();
    auto hraw_cut = cutx(hraw,bin1,bin2,boxraw);
    hraw_cut -> SetLineColor(kGray);
    c_raw -> cd();
    boxraw -> SetLineColor(kGray);
    boxraw -> SetLineWidth(1);
    boxraw -> Draw("same");

    auto boxall = new TGraph();
    auto hall_cut = make(cutx(hall,bin1,bin2,boxall));
    hall_cut -> SetLineColor(kPink);
    c_all -> cd();

    auto boxnal = new TGraph();
    auto hnal_cut = make(cutx(hnal,bin1,bin2,boxnal));
    hnal_cut -> SetLineColor(kBlue);
    c_nal -> cd();

    Double_t refy=0., ref2=0.;
    auto binc_min = 41;
    auto binc_max = 51;

    if (x1 < 20) {
      binc_min = 105;
      binc_max = 115;
    }
    else if (x1 < 30) {
      binc_min = 100;
      binc_max = 110;
    }
    else if (x1 < 40) {
      binc_min = 95;
      binc_max = 105;
    }
    else if (x1 < 50) {
      binc_min = 90;
      binc_max = 98;
    }
    else if (x1 < 70) {
      binc_min = 41;
      binc_max = 66;
    }
    else if (x1 < 100) {
      binc_min = 41;
      binc_max = 61;
    }
    else if (x1 > 139) {
      binc_min = 47;
      binc_max = 48;
    }

    if (x1 > 180) {
      binc_min = 38;
      binc_max = 40;
    }

    for (auto binc = binc_min; binc < binc_max; ++binc)
    {
      auto y = hnal_cut -> GetBinContent(binc);
      auto ref = hall_cut -> GetBinContent(binc);
      refy += ref*y;
      ref2 += ref*ref;
    }
    auto scale = refy/ref2;
    cout << bin1 << "(" << x1 << ") -> " << bin2 << "(" << x2 << ")  :  scale = " << scale << endl;
    graph -> SetPoint(graph->GetN(),(x1+x2)/2,scale);
    graph -> SetPointError(graph->GetN()-1,0,scale/sqrt(hall_cut->GetEntries()));

    auto line = new TLine();
    line -> SetLineColor(kViolet);
    line -> SetLineStyle(2);
    if (draw_all_xcut)
    {
      auto c1 = cc(Form("%s_cutx_%.2f_%.2f",hraw->GetName(),x1,x2));
      hraw_cut -> SetTitle(Form("x:%.2f->%.2f;TOF",x1,x2));
      hraw_cut -> GetXaxis() -> SetRangeUser(20,150);
      make(hraw_cut) -> Draw();
      make(hall_cut) -> Draw("same");
      make(hnal_cut) -> Draw("same");

      auto hscale = (TH1D *) hall_cut -> Clone();
      for (auto binc = 1; binc < 201; ++binc)
        hscale -> SetBinContent(binc,scale*hscale->GetBinContent(binc));
      hscale -> SetLineColor(kGreen);
      hscale -> Draw("same");

      auto hsub = (TH1D *) hnal_cut -> Clone();
      hsub -> Add(hscale,-1);
      hsub -> SetLineColor(kOrange-3);
      hsub -> Draw("same");

      auto max2 = max(hscale);
      auto x_min = hscale -> GetXaxis() -> GetBinCenter(binc_min);
      auto x_max = hscale -> GetXaxis() -> GetBinCenter(binc_max);
      line -> DrawLine(x_min,0,x_min,max2);
      line -> DrawLine(x_max,0,x_max,max2);

      hsub_all -> Add(hsub);

      auto l = new TLegend();
      l -> AddEntry(hraw_cut,"1. raw distribution (2.+3.)","l");
      l -> AddEntry(hall_cut,"2. vt_all=0 : gamma and neutron","l");
      l -> AddEntry(hnal_cut,"3. vt_all=1 : removed veto event","l");
      l -> AddEntry(hscale,Form("4. scaled from 2) by %.2f",scale),"l");
      l -> AddEntry(hsub,"5. removed all fitted neutron (3.-4.)","l");
      l -> AddEntry(line,"Fit range for scale factor","l");
      make(l) -> Draw();

      save(c1);
    }

    if (draw_n_distributions)
    {
      c_alls -> cd();
      auto hcloned = (TH1D *) hall_cut -> Clone();
      hcloned -> SetLineColor(colors[count]);
      lg -> AddEntry(hcloned,Form("x:%.2f->%.2f",x1,x2),"l");

      if (ignore_below_35) for (auto i = 1; i <= 35; ++i) hcloned -> SetBinContent(i,0);
      norm_max(hcloned) -> DrawCopy("same");
    }

    ++count;
  }

  if (draw_n_distributions) {
    c_alls -> cd();
    make(lg) -> Draw();
  }

  graph -> SetMarkerStyle(25);
  auto frame = graph -> GetHistogram();
  frame -> SetMinimum(0);
  frame -> SetTitle(";EDep;Scale factor");
  cc(); make(graph) -> Draw("alp");
  auto f1 = new TF1("f1","(x<170)*(pol4)+(x>=170)*[5]",0,200);
  f1 -> SetParameter(0,0.4);
  graph -> Fit(f1);

  if (draw_all_xcut) { cc(); make(hsub_all) -> Draw(); }
}
