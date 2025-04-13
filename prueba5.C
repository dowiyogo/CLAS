{
   THStack hs("hs","Convolution");

   TH1F *h1 = new TH1F("h1","Landau",500,0,20);
   TH1F *h2 = new TH1F("h2","Gauss",500,0,20);
   TH1F *h3 = new TH1F("h3","Convolution",500,0,20);
Int_t i;
 for (i=0;i<1000000;i++)
   {
      Double_t x = gRandom->Landau(4,0.1);
	h1->Fill(x);
	Double_t temp=gRandom->Gaus(3,0.5);
	h2->Fill(temp);
   }
Int_t i1, j,tmp;

//convolution process
	for (i1=0; i1<500; i1++)
	{
		tmp = 0.0;
		for (j=0; j<500; j++)
		{
			if((i1+1-j)>0 && (i1+1-j)<500)
				tmp = tmp + (h1->GetBinContent(i1 + 1-j)*h2->GetBinContent(j + 1));
		}
			h3->SetBinContent(i1 + 1,tmp);
	}

   h2->SetMarkerColor(kBlue);
   h2->SetMarkerStyle(21);
   hs.Add(h2);
   h1->SetMarkerColor(kRed);
   h1->SetMarkerStyle(21);
   hs.Add(h1);

   h3->SetMarkerColor(kGreen);
   h3->SetMarkerStyle(21);
   hs.Add(h3);
   TCanvas c1("c1","stacked hists",10,10,700,900);
   c1.Divide (1,2);
   c1.cd(1);

   hs.Draw("nostack,1p");
 mylegend = new TLegend(0.1,0.7,0.2,0.9);
 mylegend->AddEntry(h1,"Landau","1p");
   mylegend->AddEntry(h2,"Gauss","1p");
   mylegend->AddEntry(h3,"Convolution","1p");
mylegend->Draw();
   Double_t * source = new Double_t[500];
   Double_t * response = new Double_t[500];
c1.cd(2);
TH1F *deconv = new TH1F("deconv","Deconvolution",500,0,20);
   h3->Draw();
   TSpectrum *s = new TSpectrum();
 for (i = 0; i < 500; i++) source[i]=h3->GetBinContent(i + 1);
   for (i = 0; i < 500; i++) response[i]=h1->GetBinContent(i + 1);
   s->Deconvolution(source,response,500,5000,1,1);
  for (i = 0; i < 500; i++) deconv->SetBinContent(i + 1,source[i]);
	TF1 *func1 = new TF1("func1","gaus(0)",4,10); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func1->SetLineColor(kYellow);
	func1->SetParameters(20000,7,0.5);
	func1->SetParLimits(0,1000,40000);
	func1->SetParLimits(1,4,8);
	func1->SetParLimits(2,0.05,0.5);
	deconv->Fit("func1","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.

TCanvas* c2= new TCanvas("c1","Deconvolution",10,10,700,900);
   deconv->SetLineColor(kRed);


   deconv->Draw("L");



}

 

