#include "style.h"
using namespace style;

void draw_check()
{
  gstat(1);
  fstat(1);
  zcolor(1);
  fsave(1);
  //TString vname = "final_0";
  //TString vname = "fit_0.8";
  //TString vname = "fit_1";
  TString vname = "n1";
  //TString vname = "n2";
  //TString vname = "n3";
  //TString vname = "n4";
  version(vname);

  bool drawSum = true;
  bool draw2D = false;
  bool drawDetail = false;

  auto draw =[vname, drawSum, draw2D, drawDetail](TFile *file, TString var1, Double_t v1, Double_t v2, Double_t dv)
  {
    TH1D *histSum = nullptr;

    for (auto var2=v1; var2<=v2; var2+= dv)
    {
      TString name0 = Form("d%sdl_%.2f",var1.Data(),var2);
      auto hist = (TH2D *) file -> Get(name0);
      if (hist==nullptr)
        continue;

      auto graph = (TGraphErrors *) file -> Get(Form("d%sdl_%.2f_fit",var1.Data(),var2));
      graph -> SetFillStyle(0);
      graph -> SetMarkerStyle(20);

      if (draw2D) {
        auto cvs = cc(name0);
        hist -> Draw("colz");
        graph -> Draw("same 2 p");
        cvs -> SetLogz();
      }

      if (drawSum) {
        if (histSum == nullptr)
          histSum = hist -> ProjectionX();
        else
          histSum -> Add(hist -> ProjectionX());
      }

      if (drawDetail)
        for (auto i = 0; i < 10; ++i) {
          auto name = Form("d%sdl_%.2f_%d",var1.Data(),var2,i);
          auto histi = (TH2D *) file -> Get(name);
          if (histi==nullptr)
            continue;

          cc(name);
          histi -> Draw();
          auto fit_detail = fitg(histi,0.8);
          fit_detail -> Draw("same");
        }
    }

    if (drawSum) {
      histSum -> SetNameTitle(var1+"-projection",vname+";"+var1);
      cc(var1);
      make(histSum) -> Draw();
      auto f1 = fitg(histSum,0.5);
      f1 -> Draw("same");
      auto par1 = f1 -> GetParameter(2);
      auto par2 = f1 -> GetParError(2);
      cout << endl;
      cout << par1 << " - 2*" << par2 << " = " << par1 - 2*par2 << endl;
      cout << par1 << " + 2*" << par2 << " = " << par1 + 2*par2 << endl;
      //auto p = new TPaveText();
      //p -> AddText(Form("Amplitude = %.f",f1->GetParameter(0)));
      //p -> AddText(Form("mean = %.f",     f1->GetParameter(1)));
      //p -> AddText(Form("#sigma = %.f",   f1->GetParameter(2)));
      //make(p) -> Draw("same");
    }
  };

  auto filey = new TFile(Form("data/histy.%s.root",vname.Data()));
  auto filex = new TFile(Form("data/histx.%s.root",vname.Data()));
  auto filez = new TFile(Form("data/histz.%s.root",vname.Data()));

  draw(filex,"x", 0,.45,.05);
  draw(filey,"y", 0,.45,.05);
  draw(filez,"z", 2,6,1);
}
