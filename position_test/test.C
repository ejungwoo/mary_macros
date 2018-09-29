void test()
{
  gRandom -> SetSeed(time(0));
  auto hist_mean = new TH1D("hist_mean","mean",500,-10,10);
  auto hist = new TH1D("hist","",10,-10,10);

  auto gaus_mean = 0.3;
  auto gaus_sigma = 1.1;

  for (auto trial = 0; trial < 1000; ++trial)
  {
    hist -> Clear();
    for (auto entry = 0; entry < 1000000; ++entry) {
      auto val = gRandom -> Gaus(gaus_mean,gaus_sigma);
      hist -> Fill(val);
    }
    auto mean = hist -> GetMean();
    auto r = mean - gaus_mean;
    if (trial/100==0)
      cout << mean << " " << r << endl;

    hist_mean -> Fill(r*1000);

    //new TCanvas(); hist -> DrawCopy();
  }
  new TCanvas(); hist_mean -> Draw();
}
