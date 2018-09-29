#include "style.h"
using namespace style;

void tan2()
{
  fsave(1);
  cutt();
  cutr();

  //auto f0 = new TF1("f1","TMath::Tan(x)*TMath::Tan(x)",0,1.2);
  //auto f1 = new TF1("f1","TMath::Tan(x)*TMath::Tan(x)",0,0.5);
  //auto f2 = new TF1("f2","TMath::Tan(x)*TMath::Tan(x)",0.7,1.2);

  auto f0 = new TF1("f1","TMath::Tan(x*TMath::DegToRad())*TMath::Tan(x*TMath::DegToRad())",TMath::RadToDeg()*0,  TMath::RadToDeg()*1.2);
  auto f1 = new TF1("f1","TMath::Tan(x*TMath::DegToRad())*TMath::Tan(x*TMath::DegToRad())",TMath::RadToDeg()*0,  TMath::RadToDeg()*0.5);
  auto f2 = new TF1("f2","TMath::Tan(x*TMath::DegToRad())*TMath::Tan(x*TMath::DegToRad())",TMath::RadToDeg()*0.7,TMath::RadToDeg()*1.2);

  for (auto f : {f1,f2}) {
    make(f);
    f->SetLineWidth(3);
    auto hist = f -> GetHistogram();
    hist -> SetTitle(";#alpha (Degrees);tan^{2}#alpha");
    //hist -> SetTitle(";#alpha (Rad.);tan^{2}#alpha");
    auto cvs = c(f->GetName(),340,500);
    make(hist) -> Draw();
    //f -> Draw("same");
    cvs -> SetGrid(); 
    save(cvs);
  }

  return;
  for (auto alpha = 0.; alpha<TMath::Pi()/2; alpha+=TMath::Pi()/2/30)
  {
    auto tt = TMath::Tan(alpha)*TMath::Tan(alpha);
    if (tt > 9)
      break;
    cout << setw(15) << alpha << setw(15) << alpha*TMath::RadToDeg() << setw(15) << tt << endl;
  }
}
