{
   THStack hs("hs","Convolution");

   TH1F *h1 = new TH1F("h1","Landau",500,-10,10);
   TH1F *h2 = new TH1F("h2","Gauss",500,-10,10);
   TH1F *h3 = new TH1F("h3","Convolution",500,-10,10);
Int_t i;
 for (i=0;i<1000000;i++)
   {
      Double_t x = gRandom->Landau(0,0.1);
	h1->Fill(x);
	Double_t temp=gRandom->Gaus(0,0.5);
	h2->Fill(temp);
	Double_t noise=gRandom->Gaus(0,1);
	if (i%10==0){x=x+temp+noise;} 
	else {x += temp;}
      h3->Fill(x);//probability density function of the addition of two variables is the convolution of 2 dens. functions
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
TH1F *deconv = new TH1F("deconv","Deconvolution",500,-10,10);
   h3->Draw();
   TSpectrum *s = new TSpectrum();
 for (i = 0; i < 500; i++) source[i]=h3->GetBinContent(i + 1);
   for (i = 0; i < 500; i++) response[i]=h2->GetBinContent(i + 1);
   s->Deconvolution(source,response,500,5000,1,1);
  for (i = 0; i < 500; i++) deconv->SetBinContent(i + 1,source[i]);
	TF1 *func1 = new TF1("func1","gaus(0)",4,10); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func1->SetLineColor(kYellow);
	func1->SetParameters(20000,7,0.5);
	func1->SetParLimits(0,1000,40000);
	func1->SetParLimits(1,4,8);
	func1->SetParLimits(2,0.05,0.5);
	//deconv->Fit("func1","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.

TCanvas* c2= new TCanvas("c1","Deconvolution",10,10,700,900);
   deconv->SetLineColor(kRed);


   deconv->Draw("L");



}

 

