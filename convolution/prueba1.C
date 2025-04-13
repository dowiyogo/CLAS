{
   THStack hs("hs","test stacked histograms");
TF1 *fb1 = new TF1("fb1","TMath::Gaus(x,3,0.5,0)",0,10);

   TH1F *h1 = new TH1F("h1","test hstack",100,0,10);
   h1->FillRandom("fb1",1000000);
   h1->SetFillColor(kRed);
   hs.Add(h1);
TF1 *fb2 = new TF1("fb2","TMath::Landau(x,[0],[1],0)",0,10);
fb2->SetParameters(4,0.1);
   TH1F *h2 = new TH1F("h2","test hstack",100,0,10);
   h2->FillRandom("fb2",1000000);
   h2->SetFillColor(kBlue);
   hs.Add(h2);
   TH1F *h3 = new TH1F("h3","test hstack",100,0,10);
   h3->Add(h1);
   h3->Add(h2);
   h3->SetFillColor(kGreen);
   hs.Add(h3);
   TCanvas c1("c1","stacked hists",10,10,700,900);
   c1.Divide (1,2);
   c1.cd(1);
   hs.Draw();
   Int_t i;
   Double_t * source = new Double_t[100];
   Double_t * response = new Double_t[100];
c1.cd(2);
TH1F *dec = new TH1F("dec","",100,0,10);
   h3->Draw();
   TSpectrum *s = new TSpectrum();
 for (i = 0; i < 100; i++) source[i]=h3->GetBinContent(i + 1);
   for (i = 0; i < 100; i++) response[i]=h2->GetBinContent(i + 1);
   s->Deconvolution(source,response,100,100000,1,1);
  for (i = 0; i < 100; i++) dec->SetBinContent(i + 1,source[i]);
   dec->SetLineColor(kRed);
   dec->Draw("SAME L");
   c1.cd(2);
}
