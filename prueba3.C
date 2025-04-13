{
   THStack hs("hs","test stacked histograms");
TF1 *fb1 = new TF1("fb1","TMath::Gaus(x,3,0.5,0)",0,10);
fb1->SetNpx(10000);
   TH1F *h1 = new TH1F("h1","test hstack",100,0,10);
   h1->FillRandom("fb1",20000);
   h1->SetFillColor(kRed);
   hs.Add(h1);
TF1 *fb2 = new TF1("fb2","TMath::Landau(x,[0],[1],0)",0,10);
fb2->SetParameters(4,1);
fb2->SetNpx(10000);
   TH1F *h2 = new TH1F("h2","test hstack",100,0,10);
   h2->FillRandom("fb2",20000);
   h2->SetFillColor(kBlue);
   hs.Add(h2);
   TH1F *h3 = new TH1F("h3","test hstack",100,0,10);
TF1Convolution *f_conv = new TF1Convolution(fb1, fb2,0,10,false);
f_conv->SetRange(0,10);
TF1 *fc = new TF1("fc",*f_conv,0,10,f_conv->GetNpar());
fc->SetNpx(10000);
fc->SetParameters(2,1);
h3->FillRandom("fc",20000);
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
   s->Deconvolution(source,response,100,1000,1,1);
  for (i = 0; i < 100; i++) dec->SetBinContent(i + 1,source[i]);
   dec->SetLineColor(kRed);
   dec->Draw("SAME L");
   c1.cd(2);
}

 

