{
gSystem->Load("/home/rene/roounfold/libRooUnfold.so");
Int_t i;
TCanvas* c3= new TCanvas("c3","Pion Energy Thrown",10,10,700,900);
TFile* simul = TFile::Open("/home/rene/respaldo/arratia/Elosscode/Pb1_simul.root","READ");
	ntuple_thrown->Draw("Zh*Nu>>thrown()","Xf>0.1"); //  del TNtuple como un Histograma
	TH1F* h4;
	h4 = (TH1F *)gROOT->FindObject("thrown"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h4->Scale(1/h4->Integral());
TCanvas* c4= new TCanvas("c4","Pion Energy Accept",10,10,700,900);
	ntuple_accept->Draw("Zh*Nu>>accept()","Xf>0.1"); //  del TNtuple como un Histograma
	TH1F* h5;
	h5 = (TH1F *)gROOT->FindObject("accept"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h5->Scale(1/h5->Integral());


Int_t Nbin= h5->GetNbinsX();

   RooUnfoldResponse response(100,0,5);
   TSpectrum *sp = new TSpectrum();
  Double_t * source1 = new Double_t[Nbin];
   Double_t * response1 = new Double_t[Nbin];
 for (i = 0; i < Nbin; i++) 
	{
	source1[i]=h4->GetBinContent(i + 1); 
	response1[i]=h5->GetBinContent(i + 1); 
	response.Fill(h4->GetBinContent(i + 1),h5->GetBinContent(i + 1,1)) ;
	}
   sp->Deconvolution(source1,response1,Nbin,10,1,1);

TCanvas* c5= new TCanvas("c5","Deconvolution",10,10,700,900);

TH1F *deconv2 = new TH1F("deconv2","Montecarlo Deconvolution",Nbin,0,5);
for (i = 0; i < Nbin; i++) deconv2->SetBinContent(i + 1,source1[i]);
 deconv2->SetLineColor(kRed);

   deconv2->Draw();
TCanvas* c6= new TCanvas("c6","Pion Energy from Data",10,10,700,900);
TFile* data = TFile::Open("/home/rene/respaldo/arratia/Elosscode/Pb_data.root","READ");
ntuple_data->Draw("Zh*Nu>>data()","Xf>0.1"); //  del TNtuple como un Histograma
	TH1F* h6;

h6 = (TH1F *)gROOT->FindObject("data"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	h6->Scale(1/h6->Integral());
RooUnfoldBayes   unfold (&response, h6, 4);
TH1D* hReco= (TH1D*) unfold.Hreco();
unfold.PrintTable (cout, h4);

//Nbin= h6->GetNbinsX();

   
   TSpectrum *sp2 = new TSpectrum();
  Double_t * source2 = new Double_t[Nbin];
   Double_t * response2 = new Double_t[Nbin];
 for (i = 0; i < Nbin; i++) source2[i]=h6->GetBinContent(i + 1);
 for (i = 0; i < Nbin; i++) response2[i]=source1[i];
   sp2->Deconvolution(source2,response2,Nbin,20,1,1);
 //  sp2->Deconvolution(response2,source2,Nbin,1000,1,1);

TCanvas* c7= new TCanvas("c7","Deconvolution",10,10,700,900);

TH1F *deconv3 = new TH1F("deconv3","Data Deconvolution",Nbin,0,5);
for (i = 0; i < Nbin; i++) deconv3->SetBinContent(i + 1,source2[i]);
//for (i = 0; i < Nbin; i++) deconv3->SetBinContent(i + 1,response2[i]);
 deconv3->SetLineColor(kBlue);

   deconv3->Draw();


TCanvas* c8= new TCanvas("c8","Deconvolution",10,10,700,900);

hReco->Draw();

Int_t i1, j,tmp;

//convolution process
/*	for (i1=0; i1<500; i1++)
	{
		tmp = 0.0;
		for (j=0; j<500; j++)
		{
			if((i1+1-j)>0 && (i1+1-j)<500)
				tmp = tmp + (h1->GetBinContent(i1 + 1-j)*h2->GetBinContent(j + 1));
		}
			h3->SetBinContent(i1 + 1,tmp);
	}
*/

}

 

