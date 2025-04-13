{
//gSystem->Load("/home/rene/roounfold/libRooUnfold.so");
TStopwatch T;
T.Start();

RooUnfoldResponse responseD(100,0,2.5);
TFile* simulD= TFile::Open("/home/rene/respaldo/conv/CFFTree_D2.root","READ");

//TNtuple *ThD ;
//simulD->GetObject("tree_thrown",ThD );

TNtuple *ThD = (TNtuple *)gROOT->FindObject("tree_thrown");


//TNtuple *AcD ;
//simulD->GetObject("tree_accept",AcD);

TCanvas* c1D= new TCanvas("c1D","Pion Energy Thrown D",10,10,700,900);
cout<<"hola"<<endl;
	ThD->Draw("Zh*Nu>>thrownD()","Zh*Nu<2.0&&Xf>0.1"); //  del TNtuple como un Histograma
	TH1F* hist_trueD;
cout<<"chao"<<endl;	
	hist_trueD = (TH1F *)gROOT->FindObject("thrownD"); // Busco el objeto y asigno un puntero para el histograma recien dibujado

TFile* dataD = TFile::Open("/home/rene/respaldo/arratia/Elosscode/D_10M_Standard_Ntuple.root","READ");	
//TNtuple *DaD;
TCanvas* c2D= new TCanvas("c2D","Pion Energy D Target",10,10,700,900);
TH1F* hist_dataD;
//dataD->GetObject("ntuple_data",DaD );
TNtuple *DaD = (TNtuple *)gROOT->FindObject("ntuple_data");
DaD->Draw("Zh*Nu>>D_data()","Zh*Nu<2.0&&Xf>0.1"); //  del TNtuple como un Histograma
hist_dataD = (TH1F *)gROOT->FindObject("D_data");

// Create a TTreeReader named "MyTree" from the given TDirectory.
// The TTreeReader gives access to the TTree to the TTreeReaderValue and
// TTreeReaderArray objects. It knows the current entry number and knows
// how to iterate through the TTree.
   
  TTreeReader datareaderD("ntuple_data",dataD);
  TTreeReader thrownreaderD("tree_thrown",simulD);
  TTreeReader acceptreaderD("tree_accept",simulD);

  TTreeReaderValue<Int_t> eventsize_thrownD(thrownreaderD,"eventsize");
  TTreeReaderValue<Int_t> eventsize_acceptD(acceptreaderD,"eventsize");
  
  // energy = zh nu
  TTreeReaderArray<Float_t> zh_dataD(datareaderD,"Zh");
  TTreeReaderArray<Float_t> zh_thrownD(thrownreaderD,"Zh");
  TTreeReaderArray<Float_t> zh_acceptD(acceptreaderD,"Zh");

  TTreeReaderArray<Float_t> nu_dataD(datareaderD,"Nu");
  TTreeReaderArray<Float_t> nu_thrownD(thrownreaderD,"Nu");
  TTreeReaderArray<Float_t> nu_acceptD(acceptreaderD,"Nu");

  TTreeReaderArray<Float_t> xf_dataD(datareaderD,"Xf");
  TTreeReaderArray<Float_t> xf_thrownD(thrownreaderD,"Xf");
  TTreeReaderArray<Float_t> xf_acceptD(acceptreaderD,"Xf");

  // only positive pions
  TTreeReaderArray<Float_t> pid_thrownD(thrownreaderD,"pid");
  TTreeReaderArray<Float_t> pid_acceptD(acceptreaderD,"pid");

  // check
  TTreeReaderArray<Float_t> phipq_thrownD(thrownreaderD,"PhiPQ");
  TTreeReaderArray<Float_t> phipq_acceptD(acceptreaderD,"PhiPQ");


  // training loop
  TH1F *response_fillD = new TH1F("response_fillD","Accepted D training data",100,0,2.5);
  TH1F *response_missD = new TH1F("response_missD","Rejected D training data",100,0,2.5);
  while ( thrownreaderD.Next() ) {
    acceptreaderD.Next();


    for ( int i = 0; i < *eventsize_thrownD; i++ ) {
      for ( int j = 0; j < *eventsize_acceptD; j++ ) {
	if ( pid_thrownD[i] != 8 || pid_acceptD[j] != 211 ) {
	  continue;
	}
	
	Float_t thrown_energyD = (zh_thrownD[i])*(nu_thrownD[i]);
	Float_t accept_energyD = (zh_acceptD[j])*(nu_acceptD[j]);
	
	if ( pid_thrownD[i] == 8 && thrown_energyD < 2.0 ) {
	  if (  accept_energyD < 2.0 && abs(phipq_thrownD[i] - phipq_acceptD[j]) < 4.0 ) {
		 responseD.Fill(accept_energyD,thrown_energyD);
	    response_fillD->Fill(accept_energyD);
	  } else {
	    response_missD->Fill(thrown_energyD);
		responseD.Miss(thrown_energyD);
	  }
	}
      }

    }
  }
 
 TCanvas* c3D= new TCanvas("c3D","Pion Energy D Accept Fill",10,10,700,900);
response_fillD->Draw();
TCanvas* c4D= new TCanvas("c4D","Pion Energy D Accept Miss",10,10,700,900);
response_missD->Draw();

 //RooUnfoldSvd unfold (&response, hist_data, 5);
RooUnfoldBayes unfoldD (&responseD, hist_dataD, 3);
//RooUnfoldBinByBin unfold (&response, hist_data);
TH1D* hRecoD= (TH1D*) unfoldD.Hreco();
unfoldD.PrintTable (cout, hist_trueD);

TCanvas* c5D= new TCanvas("c5D","Pion Energy D Deconvolution",10,10,700,900);

hRecoD->Draw();

TCanvas* c6D= new TCanvas("c6D","Pion Energy D Deconvolution Normalized",10,10,700,900);

hRecoD->DrawNormalized();
hist_dataD->DrawNormalized("same");

//simulD->Close();
//dataD->Close();
 /*+++++++++++++++++++++++++++FIERRO+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

 TFile* simulFe = TFile::Open("/home/rene/respaldo/arratia/Elosscode/CFFTree_Fe1.root","READ");
 
//TNtuple *ThFe ;
//simulFe->GetObject("tree_thrown",ThFe );
TNtuple *ThFe = (TNtuple *)gROOT->FindObject("tree_thrown");
//TNtuple *AcFe ;
//simulFe->GetObject("tree_accept",AcFe);

RooUnfoldResponse responseFe(100,0,2.5);

TCanvas* c1Fe= new TCanvas("c1Fe","Pion Energy Thrown Fe",10,10,700,900);

ThFe->Draw("Zh*Nu>>thrownFe()","Zh*Nu<2.0&&Xf>0.1"); //  del TNtuple como un Histograma

TH1F* hist_trueFe;

hist_trueFe = (TH1F *)gROOT->FindObject("thrownFe"); // Busco el objeto y asigno un puntero para el histograma recien dibujado

TFile* dataFe= TFile::Open("/home/rene/respaldo/arratia/Elosscode/Fe_data.root","READ");
//TNtuple *DaFe;
TCanvas* c2Fe= new TCanvas("c2Fe","Pion Energy Fe Target",10,10,700,900);
TH1F* hist_dataFe;
//dataFe->GetObject("ntuple_data",DaFe );
TNtuple *DaFe = (TNtuple *)gROOT->FindObject("ntuple_data");
DaFe->Draw("Zh*Nu>>Fe_data()","Zh*Nu<2.0&&Xf>0.1&&TargType==1"); //  del TNtuple como un Histograma
hist_dataFe = (TH1F *)gROOT->FindObject("Fe_data");
 
 TTreeReader datareaderFe("ntuple_data",dataFe);
  TTreeReader thrownreaderFe("tree_thrown",simulFe);
  TTreeReader acceptreaderFe("tree_accept",simulFe);

  TTreeReaderValue<Int_t> eventsize_thrownFe(thrownreaderFe,"eventsize");
  TTreeReaderValue<Int_t> eventsize_acceptFe(acceptreaderFe,"eventsize");
  
  // energy = zh nu
  TTreeReaderArray<Float_t> zh_dataFe(datareaderFe,"Zh");
  TTreeReaderArray<Float_t> zh_thrownFe(thrownreaderFe,"Zh");
  TTreeReaderArray<Float_t> zh_acceptFe(acceptreaderFe,"Zh");

  TTreeReaderArray<Float_t> nu_dataFe(datareaderFe,"Nu");
  TTreeReaderArray<Float_t> nu_thrownFe(thrownreaderFe,"Nu");
  TTreeReaderArray<Float_t> nu_acceptFe(acceptreaderFe,"Nu");

  TTreeReaderArray<Float_t> xf_dataFe(datareaderFe,"Xf");
  TTreeReaderArray<Float_t> xf_thrownFe(thrownreaderFe,"Xf");
  TTreeReaderArray<Float_t> xf_acceptFe(acceptreaderFe,"Xf");

  // only positive pions
  TTreeReaderArray<Float_t> pid_thrownFe(thrownreaderFe,"pid");
  TTreeReaderArray<Float_t> pid_acceptFe(acceptreaderFe,"pid");

  // check
  TTreeReaderArray<Float_t> phipq_thrownFe(thrownreaderFe,"PhiPQ");
  TTreeReaderArray<Float_t> phipq_acceptFe(acceptreaderFe,"PhiPQ");


  // training loop
  TH1F *response_fillFe = new TH1F("response_fillFe","Accepted Fe training data",100,0,2.5);
  TH1F *response_missFe = new TH1F("response_missFe","Rejected Fe training data",100,0,2.5);
  while ( thrownreaderFe.Next() ) {
    acceptreaderFe.Next();


    for ( int i = 0; i < *eventsize_thrownFe; i++ ) {
      for ( int j = 0; j < *eventsize_acceptFe; j++ ) {
	if ( pid_thrownFe[i] != 8 || pid_acceptFe[j] != 211 ) {
	  continue;
	}
	
	Float_t thrown_energyFe = (zh_thrownFe[i])*(nu_thrownFe[i]);
	Float_t accept_energyFe = (zh_acceptFe[j])*(nu_acceptFe[j]);
	
	if ( pid_thrownFe[i] == 8 &&  thrown_energyFe < 2.0 ) {
	  if ( accept_energyFe < 2.0 && abs(phipq_thrownFe[i] - phipq_acceptFe[j]) < 4.0 ) {
		 responseFe.Fill(accept_energyFe,thrown_energyFe);
	    response_fillFe->Fill(accept_energyFe);
	  } else {
	    response_missFe->Fill(thrown_energyFe);
		responseFe.Miss(thrown_energyFe);
	  }
	}
      }

    }
  }
 
TCanvas* c3Fe= new TCanvas("c3Fe","Pion Energy Fe Accept Fill",10,10,700,900);
response_fillFe->Draw();
TCanvas* c4Fe= new TCanvas("c4Fe","Pion Energy Fe Accept Miss",10,10,700,900);
response_missFe->Draw();

//RooUnfoldSvd unfold (&response, hist_data, 5);
RooUnfoldBayes unfoldFe (&responseFe, hist_dataFe, 3);
//RooUnfoldBinByBin unfold (&response, hist_data);
TH1D* hRecoFe= (TH1D*) unfoldFe.Hreco();
unfoldFe.PrintTable (cout, hist_trueFe);

TCanvas* c5Fe= new TCanvas("c5Fe","Pion Energy Fe Deconvolution",10,10,700,900);

hRecoFe->Draw();

TCanvas* c6Fe= new TCanvas("c6Fe","Pion Energy Fe Deconvolution Normalized",10,10,700,900);

hRecoFe->DrawNormalized();
hist_dataFe->DrawNormalized("same");


TCanvas* c7FeD= new TCanvas("c7FeD","Pion Energy D and Fe Deconvolution Normalized",10,10,700,900);
hRecoFe->DrawNormalized();
hRecoD->SetLineColor(kRed);
hRecoD->DrawNormalized("same");

TCanvas* c8D= new TCanvas("c8D","Response Matrix D",10,10,700,900);
responseD.HresponseNoOverflow()->DrawNormalized("colz");
TCanvas* c8Fe= new TCanvas("c8Fe","Response Matrix Fe",10,10,700,900);
TH2D* HrespFe=responseFe.HresponseNoOverflow();
//responseFe.HresponseNoOverflow()->Draw("colz");
HrespFe->DrawNormalized("colz");
Double_t scaleFe = 1/HrespFe->Integral();
HrespFe->Scale(scaleFe);
TH2D*  HrespDif= responseD.HresponseNoOverflow();
Double_t scaleD = 1/HrespDif->Integral();
HrespDif->Scale(scaleD);
HrespDif->Add(HrespFe,-1);

TCanvas* c8FeD= new TCanvas("c8FeD","Response Matrix D minus Fe",10,10,700,900);
HrespDif->Draw("colz");


//ThD->Draw("Zh*Nu>>thrownD()","Zh*Nu<2.0");

//simulFe->Close();
//dataFe->Close();
T.Stop();
T.Print();
}

 

