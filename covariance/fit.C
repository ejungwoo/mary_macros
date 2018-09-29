#include "style.h"
using namespace style;

void fit()
{
  fsave(1);
  zcolor(1);

  axisd();
  cutr(0.12);
  gstat(0);
  fstat(1);

  auto zmin = 0;
  auto zmax = 1.5;

  //TString vname = "final_0";
  TString vname = "fit_0.8";
  //TString vname = "fit_1";
  //TString vname = "n2"; zmax = 1.5;
  //TString vname = "n3"; zmax = 1.5;
  //TString vname = "n4"; zmax = 5.0;
  version(vname);


  for (auto var : {TString("y"), TString("x"), TString("z")})
  {
    const char *vc = var.Data();
    auto file = new TFile(Form("data/graph_summary_%s.%s.root",vc,vname.Data()));
    auto graph = (TGraph2D *) file -> Get(Form("graph_summary_%s",vc));
    if (graph->GetN() < 3) {
      cout << "[" << var << "] Number of points in graph is less than 3!!!" << endl;
      continue;
    }

    auto graphx = new TGraph();
    auto graphy = new TGraph();

    auto n = graph -> GetN();
    Double_t x, y, z;
    Double_t *xarray = graph -> GetX();
    Double_t *yarray = graph -> GetY();
    Double_t *zarray = graph -> GetZ();

    Double_t xmax=0., xmin=10000.;
    Double_t ymax=0., ymin=10000.;

    for (auto i=0; i<n; ++i) {
      graphx -> SetPoint(i,xarray[i],zarray[i]);
      graphy -> SetPoint(i,yarray[i],zarray[i]);
      if (xarray[i] > xmax) xmax = xarray[i];
      if (xarray[i] < xmin) xmin = xarray[i];
      if (yarray[i] > ymax) ymax = yarray[i];
      if (yarray[i] < ymin) ymin = yarray[i];
    }

    auto fx = new TF1("fx","[0]+[1]*x",xmin,xmax);
    auto fy = new TF1("fy","[0]+[1]*x",ymin,ymax);
    if (0) {
      cc(); auto hx = new TH1D("hx","Drift length", 10,xmin,xmax); hx-> SetMinimum(0); hx -> Draw(); graphx -> Draw("same*");
      cc(); auto hy = new TH1D("hy","tan^{2}#angle",10,ymin,ymax); hy-> SetMinimum(0); hy -> Draw(); graphy -> Draw("same*");
    }
    graphx -> Fit(fx);
    graphy -> Fit(fy);
    auto f2 = new TF2(Form("f2_%s",vc),"[0]+[1]*(x-[3])*(x-[3])+[2]*(y-[4])*(y-[4])",xmin,xmax,ymin,ymax);
    f2 -> SetParameters((fx->GetParameter(0)+fy->GetParameter(0))/2.,fx->GetParameter(1),fy->GetParameter(1),0,1);

    //auto f2 = new TF2(Form("f2_%s",vc),"[0]+[1]*x+[2]*y",xmin,xmax,ymin,ymax);
    //f2 -> SetParameters((fx->GetParameter(0)+fy->GetParameter(0))/2.,fx->GetParameter(1),fy->GetParameter(1));
    //f2 -> SetParNames(Form("#sigma_{0,%s}",vc), Form("#sigma_{#angle,%s}",vc), Form("#sigma_{#L,%s}",vc));
    graph -> Fit(f2);

    auto p0 = f2->GetParameter(0);
    auto pL = f2->GetParameter(1);
    auto pA = f2->GetParameter(2);

    //auto hg = graph -> GetHistogram();
    //hg -> SetMaximum(zmax);
    //hg -> SetMinimum(zmin);
    //make(hg);
    //if (var=="y") hg -> SetTitle(Form("#sigma^{2}_{y} = %.2f + %.2f #times tan^{2}#lambda + %.5f #times L;Drift length (mm);tan^{2}#lambda;#sigma^{2}_{y} (mm)",p0,pA,pL));
    //if (var=="x") hg -> SetTitle(Form("#sigma^{2}_{x} = %.2f + %.2f #times tan^{2}#chi + %.5f #times L;Drift length (mm);tan^{2}#chi;#sigma^{2}_{x} (mm)",p0,pA,pL));
    //if (var=="z") hg -> SetTitle(Form("#sigma^{2}_{z} = %.2f + %.4f #times tan^{2}#chi + %.5f #times L;Drift length (mm);tan^{2}#chi;#sigma^{2}_{z} (mm)",p0,pA,pL));

    auto hf = (TH1 *) f2 -> GetHistogram(); 
    hf -> SetMaximum(zmax);
    hf -> SetMinimum(zmin);
    make(hf);
    if (var=="y") hf -> SetTitle(Form("[%s] #sigma^{2}_{y} = %.2f + %.2f #times tan^{2}#lambda + %.5f #times L;Drift length (mm);tan^{2}#lambda;#sigma^{2}_{y} (mm)",vname.Data(),p0,pA,pL));
    if (var=="x") hf -> SetTitle(Form("[%s] #sigma^{2}_{x} = %.2f + %.2f #times tan^{2}#chi + %.5f #times L;Drift length (mm);tan^{2}#chi;#sigma^{2}_{x} (mm)",  vname.Data(),p0,pA,pL));
    if (var=="z") hf -> SetTitle(Form("[%s] #sigma^{2}_{z} = %.2f + %.4f #times tan^{2}#chi + %.5f #times L;Drift length (mm);tan^{2}#chi;#sigma^{2}_{z} (mm)",  vname.Data(),p0,pA,pL));

    graph -> SetMarkerStyle(20);

    auto c1 = c(Form("fit_cov_%s",vc));

    //f2 -> SetLineColor(kOrange+1);
    //f2 -> Draw("surf");
    f2 -> SetMinimum(zmin);
    f2 -> SetMaximum(zmax);
    graph -> SetMinimum(zmin);
    graph -> SetMaximum(zmax);
    f2 -> SetLineWidth(0);
    f2 -> Draw("surf1z");
    graph -> Draw("same surf");
    graph -> Draw("same p");

    //if (var=="z") c1 -> SetPhi(210);

    save(c1);
  }
}
