
  if (idx == 0)
  {
    TString rootname = "histy";
    auto dtltl = 0.05;
    auto graphy = new TGraph2D();
    graphy -> SetName("graphy");
    graphy -> GetHistogram() -> SetTitle(";Dirft length (mm); tan^{2}#lambda");

    for (auto tltl = 0.; tltl < 0.45; tltl+=dtltl)
    {
      TString hname = Form("dydl_%.2f",tltl);
      TString fname = Form("dydl_%.2f_fit",tltl);
      TCut cut = Form("tltl>%.2f&&tltl<%.2f",tltl,tltl+dtltl);
      cut = cut+globalcut;
      TString title = Form("tltl=%.2f~%.2f;dy;Drift length (mm)",tltl,tltl+dtltl);

      auto tltl_rep = (tltl+tltl+dtltl)/2;
      auto hist = tp(hname,tree,"l:dy",cut,title,200,-4,4,200,100,450);
      if (hist -> GetEntries() < 30000)
        continue;

      write(rootname,hist);

      auto array = fitgsy_list(hist,7,0.5);
      auto n = array->GetEntries();
      auto graph_fit = new TGraphErrors();
      graph_fit -> SetName(fname);

      for (auto i = 0; i < n; ++i)
      {
        auto data = (style::hdata *) array -> At(i);
        auto sigma = data -> dx;
        if (sigma < 0.01)
          continue;
        auto driftLength = data->y;

        if (tltl>0.01&&driftLength>190&&driftLength<260) continue;
        if (tltl>0.14&&driftLength>140&&driftLength<180) continue;

        auto h2 = data->hist;
        if (h2->Integral(76,125) > 10000)
        {
          graphy -> SetPoint(graphy->GetN(),driftLength,tltl_rep,sigma*sigma);
          write(rootname,h2);

          graph_fit->SetPoint(i,data->x,data->y);
          graph_fit->SetPointError(graph_fit->GetN(),data->dx,data->dy/2.);
        }
      }
      write(rootname,graph_fit);
    }
    write(graphy);
  }
  else if (idx == 1)
  {
    TString rootname = "histx";
    auto dtatax = 0.05;
    auto graphx = new TGraph2D();
    graphx -> SetName("graphx");
    graphx -> GetHistogram() -> SetTitle(";Dirft length (mm); tan^{2}#alpha");
    for (auto tata = 0.; tata < 0.45; tata+=dtatax)
    {
      TString hname = Form("dxdl_%.2f",tata);
      TString fname = Form("dxdl_%.2f_fit",tata);
      TCut cut = Form("isLayer&&tata>%.2f&&tata<%.2f",tata,tata+dtatax);
      cut = cut+globalcut;
      TString title = Form("tata=%.2f~%.2f;dx;Drift length (mm)",tata,tata+dtatax);

      auto tata_rep = (tata+tata+dtatax)/2;
      auto hist = tp(hname,tree,"l:dx",cut,title,200,-4,4,200,100,450);
      if (hist -> GetEntries() < 30000)
        continue;


      write(rootname,hist);

      auto array = fitgsy_list(hist,7,0.5);
      auto n = array->GetEntries();
      for (auto i = 0; i < n; ++i)
      {
        auto data = (style::hdata *) array -> At(i);
        auto sigma = data -> dx;
        if (sigma < 0.01)
          continue;
        auto driftLength = data->y;

        //cout << data->n << " " << i << " " << driftLength << " " << tata_rep << " " << sigma << endl;
        if (data->hist->Integral(76,125) > 10000) {
          graphx -> SetPoint(graphx->GetN(),driftLength,tata_rep,sigma*sigma);
          write(rootname,data->hist);
        }
      }
    }
    write(graphx);
  }
  else if (idx == 2)
  {
    TString rootname = "histz";
    auto dtataz = 1.;
    auto graphz = new TGraph2D();
    graphz -> SetName("graphz");
    graphz -> GetHistogram() -> SetTitle(";Dirft length (mm); tan^{2}#alpha");
    for (auto tata = 2.; tata < 7.; tata+=dtataz)
    {
      TString hname = Form("dzdl_%.2f",tata);
      TString fname = Form("dzdl_%.2f_fit",tata);
      TCut cut = Form("!isLayer&&tata>%.2f&&tata<%.2f",tata,tata+dtataz);
      cut = cut+globalcut;
      TString title = Form("tata=%.2f~%.2f;dz;Drift length (mm)",tata,tata+dtataz);

      auto tata_rep = (tata+tata+dtataz)/2;
      auto hist = tp(hname,tree,"l:dz",cut,title,200,-4,4,200,100,450);
      if (hist -> GetEntries() < 30000)
        continue;

      write(rootname,hist);
      auto ge = fitgsy(hist,7,0.5);
      ge -> SetName(fname);
      write(rootname,ge);

      auto array = fitgsy_list(hist,7,0.5);
      auto n = array->GetEntries();
      for (auto i = 0; i < n; ++i) {
        auto data = (style::hdata *) array -> At(i);
        auto sigma = data -> dx;
        if (sigma < 0.01)
          continue;
        auto driftLength = data->y;

        //cout << data->n << " " << i << " " << driftLength << " " << tata_rep << " " << sigma << endl;
        if (data->hist->Integral(76,125) > 10000) {
          graphz -> SetPoint(graphz->GetN(),driftLength,tata_rep,sigma*sigma);
          write(rootname,data->hist);
        }
      }
    }
    write(graphz);
  }

  else
  {
     TCanvas *cvs;

     for (auto numHits = 1; numHits <= 4; ++numHits)
     {
       TString title = Form("number of hits = %d",numHits);
       cvs = cc(Form("dydl_n%d",numHits));
       auto h_dydl = tp(Form("dydl_n%d",numHits),tree,"l:dy",Form("n==%d",numHits),title+";dy;Drift length (mm)",200,-4,4,200,100,450); make(h_dydl) -> Draw("colz");
       auto g_dydl = fitgsy(h_dydl,50,0.5); make(g_dydl) -> Draw("same");
       gPad -> SetLogz();
       save(cvs);

       cvs = cc(Form("dytl_n%d",numHits));
       auto h_dytl = tp(Form("dytl_n%d",numHits),tree,"tltl:dy",Form("n==%d",numHits),title+";dy;tan^{2}#lambda",200,-4,4,200,0,0.5); make(h_dytl) -> Draw("colz");
       auto g_dytl = fitgsy(h_dytl,50,0.5); make(g_dytl) -> Draw("same");
       gPad -> SetLogz();
       save(cvs);

       cvs = cc(Form("dxdl_n%d",numHits));
       auto h_dxdl = tp(Form("dxdl_n%d",numHits),tree,"l:dx",Form("isLayer&&n==%d",numHits),title+";dx;Drift length (mm)",200,-4,4,200,100,450); make(h_dxdl) -> Draw("colz");
       auto g_dxdl = fitgsy(h_dxdl,50,0.5); make(g_dxdl) -> Draw("same");
       gPad -> SetLogz();
       save(cvs);

       cvs = cc(Form("dxta_n%d",numHits));
       auto h_dxta = tp(Form("dxta_n%d",numHits),tree,"tata:dx",Form("isLayer&&n==%d",numHits),title+";dx;tan^{2}#alpha",200,-4,4,200,0,0.5); make(h_dxta) -> Draw("colz");
       auto g_dxta = fitgsy(h_dxta,50,0.5); make(g_dxta) -> Draw("same");
       gPad -> SetLogz();
       save(cvs);

       cvs = cc(Form("dzdl_n%d",numHits));
       auto h_dzdl = tp(Form("dzdl_n%d",numHits),tree,"l:dz",Form("!isLayer&&n==%d",numHits),title+";dz;Drift length (mm)",200,-4,4,200,100,450); make(h_dzdl) -> Draw("colz");
       auto g_dzdl = fitgsy(h_dzdl,50,0.5); make(g_dzdl) -> Draw("same");
       gPad -> SetLogz();
       save(cvs);

       cvs = cc(Form("dzta_n%d",numHits));
       auto h_dzta = tp(Form("dzta_n%d",numHits),tree,"tata:dz",Form("!isLayer&&n==%d",numHits),title+";dz;tan^{2}#alpha",200,-4,4,200,0,10); make(h_dzta) -> Draw("colz");
       auto g_dzta = fitgsy(h_dzta,50,0.5); make(g_dzta) -> Draw("same");
       gPad -> SetLogz();
       save(cvs);
    }
  }
