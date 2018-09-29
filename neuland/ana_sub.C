#include "functions.h"
#include "style.h"
using namespace style;

void ana_sub()
{
  zcolor(0);

  auto file = new TFile("/Users/ejungwoo/np/data/h_neutron_subtracted_Inv.nsub2.root");
  file -> ls();
  auto hist = (TH2D *) file -> Get("h_neutron_subtracted_Inv");

  auto fit_proton = new TF1("fit_proton", fdedx_p, 0., 5000., 2, 1);
  make(fit_proton);

  for (const char *cut_name : {"cut_proton"}) {
    auto cut = cutg(Form("data/%s.root",cut_name),cut_name,"x","y");
    auto histc = (TH2D *) cutg(hist,cut);
    histc -> SetMinimum(0.1);
    histc -> SetMaximum(1000);

    //auto xbin1 = 50;
    //auto xbin2 = 65;
    //auto xbin1 = 70;
    //auto xbin2 = 100;
    auto xbin1 = 30;
    auto xbin2 = 48;
    auto dxbin = 3;
    bool draw_data_point = false;

    auto graph = new TGraphErrors();
    for (auto xbin=xbin1; xbin<xbin2; xbin+=dxbin) {
      auto bin1 = xbin;
      auto bin2 = xbin + dxbin - 1;
      auto x1 = histc -> GetXaxis() -> GetBinLowEdge(bin1);
      auto x2 = histc -> GetXaxis() -> GetBinUpEdge(bin2);
      auto hp = histc -> ProjectionY(TString(histc->GetName())+Form("_%d(%.2f)_%d(%.2f)",bin1,x1,bin2,x2),bin1,bin2);
      auto fp = fitg(hp,0.8);
      auto mean = fp -> GetParameter(1);
      auto sigma = fp -> GetParameter(2);
      graph -> SetPoint(graph->GetN(), (x1+x2)/2., mean);
      graph -> SetPointError(graph->GetN()-1, (x2-x1)/2., sigma);

      if (draw_data_point) {
        cc();
        hp -> Draw();
        fp -> Draw("same");
        hp -> GetXaxis() -> SetRangeUser(mean-5*sigma,mean+5*sigma);
      }
    }

    auto cvs2 = cc2();
    histc -> Draw("colz");
    histc -> GetXaxis() -> SetRangeUser(30,110);
    graph -> Draw("samep");

    //fit_proton -> SetParameters(-5.85199e-02, 6.21632e+02);
    graph -> Fit(fit_proton,"N");
    fit_proton -> Draw("same");

    auto c1 = fit_proton -> GetParameter(0);
    auto c2 = fit_proton -> GetParameter(1);
    
    cout << c1 << " " << c2 << endl;
  }

  auto cvs = cc2();
  hist -> Draw("colz");
  hist -> SetMinimum(0.1);
  hist -> SetMaximum(1000);

  fit_proton -> DrawCopy("same");
}
