#include "style.h"
using namespace style;

void cluster_covariance()
{
  gstat(0);
  fstat(1);
  //auto file = new TFile("~/data/spirit/overview_cluster_yoffset.yoffset_cal2.develop.RC.1612.62c6e62.root");
  //file -> ls();

  //auto c1 = cc(); make(file->Get("dx")) -> Draw(); save(c1);
  //auto c2 = cc(); make(file->Get("dy")) -> Draw(); save(c2);
  //auto c3 = cc(); make(file->Get("dz")) -> Draw(); save(c3);

  auto file = new TFile("~/data/spirit/cluster/run2900_s0.cluster_cov.develop.RC.1612.62c6e62.yoffset_cal4.root");
  auto tree = (TTree *) file -> Get("cluster");
  tree -> Print();

  TCanvas *c1;
  TH1 *h1;
  TF1 *f1;

  //c1 = cc(); h1 = tp("dx",tree,"dx","isLayer", ";dx",200,-4,4); h1 -> Draw(); f1 = fitg(h1,0.5); cout << f1 -> GetParameter(2) << endl; f1 -> Draw("same");
  //c1 = cc(); h1 = tp("dy",tree,"dy","",        ";dy",200,-4,4); h1 -> Draw(); f1 = fitg(h1,1);   cout << f1 -> GetParameter(2) << endl; f1 -> Draw("same");
  //c1 = cc(); h1 = tp("dz",tree,"dz","!isLayer",";dz",200,-4,4); h1 -> Draw(); f1 = fitg(h1,1);   cout << f1 -> GetParameter(2) << endl; f1 -> Draw("same");

  //v(0);
  //cc();
  //h1 = tp("dx2",tree,"dx","isLayer&&n==2", ";dx",100,-4,4);  f1 = fitg(h1,0.5); h1 -> SetLineColor(kRed); h1 -> Draw();
  //h1 = tp("dx1",tree,"dx","isLayer&&n==1", ";dx",100,-4,4);  f1 = fitg(h1,0.5); h1 -> SetLineColor(kBlue); h1 -> Draw("same");
  //h1 = tp("dx3",tree,"dx","isLayer&&n==3", ";dx",100,-4,4);  f1 = fitg(h1,0.5); h1 -> SetLineColor(kGreen); h1 -> Draw("same");
  //h1 = tp("dx4",tree,"dx","isLayer&&n==4", ";dx",100,-4,4);  f1 = fitg(h1,0.5); h1 -> SetLineColor(kBlack); h1 -> Draw("same");

  Double_t s;
  h1 = tp("dx1",tree,"dx","isLayer&&n==1", ";dx",100,-4,4);  f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dy1",tree,"dy","n==1", ";dy",100,-4,4);           f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dz1",tree,"dz","!isLayer&&n==1", ";dz",100,-4,4); f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  cout << endl;

  h1 = tp("dx2",tree,"dx","isLayer&&n==2", ";dx",100,-4,4);  f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dy2",tree,"dy","n==2", ";dy",100,-4,4);           f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dz2",tree,"dz","!isLayer&&n==2", ";dz",100,-4,4); f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  cout << endl;

  h1 = tp("dx3",tree,"dx","isLayer&&n==3", ";dx",100,-4,4);  f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dy3",tree,"dy","n==3", ";dy",100,-4,4);           f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dz3",tree,"dz","!isLayer&&n==3", ";dz",100,-4,4); f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  cout << endl;

  h1 = tp("dx4",tree,"dx","isLayer&&n==4", ";dx",100,-4,4);  f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dy4",tree,"dy","n==4", ";dy",100,-4,4);           f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dz4",tree,"dz","!isLayer&&n==4", ";dz",100,-4,4); f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  cout << endl;

  h1 = tp("dx5",tree,"dx","isLayer&&n >5", ";dx",100,-4,4);  f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dy5",tree,"dy","n >5", ";dy",100,-4,4);           f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
  h1 = tp("dz5",tree,"dz","!isLayer&&n >5", ";dz",100,-4,4); f1 = fitg(h1,0.5); s = f1 -> GetParameter(2); cout << setw(14) << s << setw(14) << s*s/100. << endl;
}
