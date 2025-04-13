{
gSystem->Load("/home/rene/roounfold/libRooUnfold.so");
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
   s->Deconvolution(source,response,500,4,1,1);
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

TCanvas* c3= new TCanvas("c2","Deconvolution",10,10,700,900);
TFile* simul = TFile::Open("/home/rene/respaldo/arratia/Elosscode/Pb1_simul.root","READ");
	ntuple_thrown->Draw("Zh*Nu>>thrown()","Xf>0.1"); //  del TNtuple como un Histograma
	TH1F* h4;
	h4 = (TH1F *)gROOT->FindObject("thrown"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h4->Scale(1/h4->Integral());
TCanvas* c4= new TCanvas("c3","Deconvolution",10,10,700,900);
	ntuple_accept->Draw("Zh*Nu>>accept()","Xf>0.1"); //  del TNtuple como un Histograma
	TH1F* h5;
	h5 = (TH1F *)gROOT->FindObject("accept"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h5->Scale(1/h5->Integral());


Int_t Nbin= h5->GetNbinsX();

   
   TSpectrum *sp = new TSpectrum();
  Double_t * source1 = new Double_t[Nbin];
   Double_t * response1 = new Double_t[Nbin];
 for (i = 0; i < Nbin; i++) source1[i]=h4->GetBinContent(i + 1);
   for (i = 0; i < Nbin; i++) response1[i]=h5->GetBinContent(i + 1);
   sp->Deconvolution(source1,response1,Nbin,10,1,1);

TCanvas* c5= new TCanvas("c1","Deconvolution",10,10,700,900);

TH1F *deconv2 = new TH1F("deconv2","Montecarlo Deconvolution",Nbin,0,5);
for (i = 0; i < Nbin; i++) deconv2->SetBinContent(i + 1,source1[i]);
 deconv2->SetLineColor(kRed);

   deconv2->Draw();
TCanvas* c6= new TCanvas("c1","Deconvolution",10,10,700,900);
TFile* data = TFile::Open("/home/rene/respaldo/arratia/Elosscode/Pb_data.root","READ");
ntuple_data->Draw("Zh*Nu>>data()","Xf>0.1"); //  del TNtuple como un Histograma
	TH1F* h6;
h6 = (TH1F *)gROOT->FindObject("data"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h6->Scale(1/h6->Integral());

//Nbin= h6->GetNbinsX();

   
   TSpectrum *sp2 = new TSpectrum();
  Double_t * source2 = new Double_t[Nbin];
   Double_t * response2 = new Double_t[Nbin];
 for (i = 0; i < Nbin; i++) source2[i]=h6->GetBinContent(i + 1);
 for (i = 0; i < Nbin; i++) response2[i]=source1[i];
   sp2->Deconvolution(source2,response2,Nbin,20,1,1);
 //  sp2->Deconvolution(response2,source2,Nbin,1000,1,1);

TCanvas* c7= new TCanvas("c1","Deconvolution",10,10,700,900);

TH1F *deconv3 = new TH1F("deconv3","Data Deconvolution",Nbin,0,5);
for (i = 0; i < Nbin; i++) deconv3->SetBinContent(i + 1,source2[i]);
//for (i = 0; i < Nbin; i++) deconv3->SetBinContent(i + 1,response2[i]);
 deconv3->SetLineColor(kBlue);

   deconv3->Draw();
}

 

