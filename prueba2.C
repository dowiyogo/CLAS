{
   THStack hs("hs","test stacked histograms");
TF1 *fb1 = new TF1("fb1","TMath::Gaus(x,3,0.5,0)",0,10);

   TH1F *h1 = new TH1F("h1","test hstack",100,0,10);
   h1->FillRandom("fb1",20000);
   h1->SetFillColor(kRed);
   hs.Add(h1);
TF1 *fb2 = new TF1("fb2","TMath::Landau(x,[0],[1],0)",0,10);
fb2->SetParameters(4,1);
   TH1F *h2 = new TH1F("h2","test hstack",100,0,10);
   h2->FillRandom("fb2",20000);
   h2->SetFillColor(kBlue);
   hs.Add(h2);
   TH1F *h3 = new TH1F("h3","test hstack",100,0,10);
Int_t i, j, i1;
Double_t tmp;
//convolution process
	for (i=0; i<100; i++)
	{
		i1 = i;
		tmp = 0.0;
		for (j=0; j<100; j++)
		{
			if(i1>=0 && i1<100)
				tmp = tmp + (h1->GetBinContent(i1 + 1)*h2->GetBinContent(j + 1));
 
			i1 = i1-1;
			h3->SetBinContent(i + 1,tmp);

		}
	}

   h3->SetFillColor(kGreen);
   hs.Add(h3);
   TCanvas c1("c1","stacked hists",10,10,700,900);
   c1.Divide (1,2);
   c1.cd(1);
   hs.Draw();
   Int_t i2;
   Double_t * source = new Double_t[100];
   Double_t * response = new Double_t[100];
c1.cd(2);
TH1F *dec = new TH1F("dec","",100,0,10);
   h3->Draw();
   TSpectrum *s = new TSpectrum();
 for (i2 = 0; i2 < 100; i2++) source[i2]=h3->GetBinContent(i2 + 1);
   for (i2 = 0; i2 < 100; i2++) response[i2]=h2->GetBinContent(i2 + 1);
   s->Deconvolution(source,response,100,10000,1,1);
  for (i2 = 0; i2 < 100; i2++) dec->SetBinContent(i2 + 1,source[i2]);
   dec->SetLineColor(kRed);
   dec->Draw("SAME L");
   c1.cd(2);
}

 

