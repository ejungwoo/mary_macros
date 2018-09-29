#include "style.h"
using namespace style;

void subtracted()
{
  gstat(1);
  zcolor(1);
  fsave(1);
  version("nsub2");

  //auto fscale = new TF1("fscale","pol4",0,500);
  //fscale -> SetParameters(0.280059, 0.0068539, 0.00026414, -3.58293e-06, 1.25202e-08);
  //auto fscale = new TF1("fscale","pol5",0,500);
  auto fscale = new TF1("f1","(x<170)*(pol4)+(x>=170)*[5]",0,200);
  fscale -> SetParameters( 3.93116e-01, -8.41271e-03, 7.15946e-04, -8.10660e-06, 2.68121e-08, 2.34707e+00);

  auto file = new TFile("data/histograms_edep_tof.root");
  
  auto hraw = (TH2D *) file -> Get("h_edep_tof");
  auto hsub = (TH2D *) hraw -> Clone();
  auto hadd = (TH2D *) hraw -> Clone();

  auto hall = (TH2D *) file -> Get("h_edep_tof_all_is0");
  auto hnal = (TH2D *) file -> Get("h_edep_tof_all_is1");

  auto c_all = cc(hall->GetName()); hall -> SetTitle("veto_all==0 -> No charged particle;edep;TOF");         inv(hall) -> Draw("colz"); gPad -> SetLogz(); //save(c_all);
  auto c_nal = cc(hnal->GetName()); hnal -> SetTitle("veto_all==1 -> At least 1 charged particle;edep;TOF"); inv(hnal) -> Draw("colz"); gPad -> SetLogz(); //save(c_nal);

  bool doDrawDetail = false;

  auto nbins = 250;
  auto dbin = 1;
  auto binmin = 1;
  auto binmax = 250;

  for (auto xbin = binmin; xbin < binmax; xbin+=dbin)
  {
    auto bin1 = xbin;
    auto bin2 = bin1 + dbin-1;

    auto x1 = hall->GetXaxis()->GetBinLowEdge(bin1);
    auto x2 = hall->GetXaxis()->GetBinUpEdge(bin2);

    TGraph box;

    auto hraw_cut = cutx(hraw,bin1,bin2,&box);
    hraw_cut -> SetLineColor(kGray);

    auto hall_cut = make(cutx(hall,bin1,bin2,&box));
    hall_cut -> SetLineColor(kPink);
    c_all -> cd();

    auto boxnal = new TGraph();
    auto hnal_cut = make(cutx(hnal,bin1,bin2,&box));
    hnal_cut -> SetLineColor(kBlue);
    c_nal -> cd();

    auto scale = fscale->Eval((x1+x2)/2);

    {
      hraw_cut -> SetTitle(Form("x:%.2f->%.2f;TOF",x1,x2));
      hraw_cut -> GetXaxis() -> SetRangeUser(20,150);

      auto hscale = (TH1D *) hall_cut -> Clone();
      for (auto binc = 1; binc < 201; ++binc)
        hscale -> SetBinContent(binc,scale*hscale->GetBinContent(binc));
      hscale -> SetLineColor(kGreen);

      auto hsub_cut = (TH1D *) hnal_cut -> Clone();
      hsub_cut -> Add(hscale,-1);
      hsub_cut -> SetLineColor(kOrange-3);

      auto hadd_cut = (TH1D *) hall_cut -> Clone();
      hadd_cut -> Add(hscale,1);

      for (auto ybin = 1; ybin <= 200; ybin++)
        hsub -> SetBinContent(xbin,ybin,hsub_cut->GetBinContent(ybin));

      for (auto ybin = 1; ybin <= 200; ybin++)
        hadd -> SetBinContent(xbin,ybin,hadd_cut->GetBinContent(ybin));

      if (doDrawDetail)
      {
        auto c1 = cc(Form("%s_cutx_%.2f_%.2f",hraw->GetName(),x1,x2));
        make(hraw_cut) -> Draw();
        make(hall_cut) -> Draw("same");
        make(hnal_cut) -> Draw("same");
        hscale -> Draw("same");
        hsub_cut -> Draw("same");

        auto l = new TLegend();
        l -> AddEntry(hall_cut,"2. vt_all=0 : gamma and neutron","l");
        l -> AddEntry(hnal_cut,"3. vt_all=1 : removed veto event","l");
        l -> AddEntry(hscale,Form("4. scaled from 2) by %.2f",scale),"l");
        l -> AddEntry(hsub_cut,"5. removed all fitted neutron (3.-4.)","l");
        make(l) -> Draw();
        //save(c1);
      }
    }
  }

  hsub -> SetName("h_neutron_subtracted");
  hsub -> SetTitle("Neutron subtracted;edep;TOF");

  auto c2 = cc(hsub->GetName());
  auto hsub_inv=inv(hsub);
  hsub_inv -> Draw("colz");
  c2 -> SetLogz();
  save(c2);
  write(hsub_inv);

  hadd -> SetName("h_neutron_added");
  hadd -> SetTitle("Neutron added;edep;TOF");

  auto c3 = cc(hsub->GetName());
  auto hadd_inv=inv(hadd);
  hadd_inv -> Draw("colz");
  c3 -> SetLogz();
  save(c3);
  write(hadd_inv);
}
