{
//gSystem->Load("/home/rene/roounfold/libRooUnfold.so");
TStopwatch T;
T.Start();
Int_t i;
TFile* simul = TFile::Open("/home/rene/respaldo/arratia/Elosscode/CFFTree_Fe1.root","READ");

TNtuple *Th = (TNtuple *)gROOT->FindObject("tree_thrown");
TNtuple *Ac = (TNtuple *)gROOT->FindObject("tree_accept");
RooUnfoldResponse response(100,0,2.5);
TCanvas* c1= new TCanvas("c1","Pion Energy Thrown",10,10,700,900);
	Th->Draw("Zh*Nu>>thrown()","Zh*Nu<2.0&&Xf>0.1"); //  del TNtuple como un Histograma
	TH1F* hist_true;
	hist_true = (TH1F *)gROOT->FindObject("thrown"); // Busco el objeto y asigno un puntero para el histograma recien dibujado


//TFile* data = TFile::Open("/home/rene/respaldo/arratia/Elosscode/D_10M_Standard_Ntuple.root","READ");
TFile* data = TFile::Open("/home/rene/respaldo/arratia/Elosscode/Fe_data.root","READ");
TNtuple *Da = (TNtuple *)gROOT->FindObject("ntuple_data");
TCanvas* c2= new TCanvas("c2","Pion Energy Fe Target",10,10,700,900);
	Da->Draw("Zh*Nu>>Fe_data()","Zh*Nu<2.0&&Xf>0.1"); //  del TNtuple como un Histograma
		TH1F* hist_data;
	hist_data = (TH1F *)gROOT->FindObject("Fe_data");

  
  TTreeReader datareader("ntuple_data",data);
  TTreeReader thrownreader("tree_thrown",simul);
  TTreeReader acceptreader("tree_accept",simul);

  TTreeReaderValue<Int_t> eventsize_thrown(thrownreader,"eventsize");
  TTreeReaderValue<Int_t> eventsize_accept(acceptreader,"eventsize");
  
  // energy = zh nu
  TTreeReaderArray<Float_t> zh_data(datareader,"Zh");
  TTreeReaderArray<Float_t> zh_thrown(thrownreader,"Zh");
  TTreeReaderArray<Float_t> zh_accept(acceptreader,"Zh");

  TTreeReaderArray<Float_t> nu_data(datareader,"Nu");
  TTreeReaderArray<Float_t> nu_thrown(thrownreader,"Nu");
  TTreeReaderArray<Float_t> nu_accept(acceptreader,"Nu");

  TTreeReaderArray<Float_t> xf_data(datareader,"Xf");
  TTreeReaderArray<Float_t> xf_thrown(thrownreader,"Xf");
  TTreeReaderArray<Float_t> xf_accept(acceptreader,"Xf");

  // only positive pions
  TTreeReaderArray<Float_t> pid_thrown(thrownreader,"pid");
  TTreeReaderArray<Float_t> pid_accept(acceptreader,"pid");

  // check
  TTreeReaderArray<Float_t> phipq_thrown(thrownreader,"PhiPQ");
  TTreeReaderArray<Float_t> phipq_accept(acceptreader,"PhiPQ");


  // training loop
  TH1F *response_fill = new TH1F("response_fill","Accepted training data",100,0,2.5);
  TH1F *response_miss = new TH1F("response_miss","Rejected training data",100,0,2.5);
  while ( thrownreader.Next() ) {
    acceptreader.Next();


    for ( int i = 0; i < *eventsize_thrown; i++ ) {
      for ( int j = 0; j < *eventsize_accept; j++ ) {
	if ( pid_thrown[i] != 8 || pid_accept[j] != 211 ) {
	  continue;
	}
	
	Float_t thrown_energy = (zh_thrown[i])*(nu_thrown[i]);
	Float_t accept_energy = (zh_accept[j])*(nu_accept[j]);
	
	if ( pid_thrown[i] == 8 && xf_thrown[i] > 0.1 && thrown_energy < 2.0 ) {
	  if ( xf_accept[j] > 0.1 && accept_energy < 2.0 && abs(phipq_thrown[i] - phipq_accept[j]) < 4.0 ) {
		 response.Fill(accept_energy,thrown_energy);
	    response_fill->Fill(accept_energy);
	  } else {
	    response_miss->Fill(thrown_energy);
		response.Miss(thrown_energy);
	  }
	}
      }

    }
  }
 
TCanvas* c3= new TCanvas("c3","Pion Energy Accept Fill",10,10,700,900);
response_fill->Draw();
TCanvas* c4= new TCanvas("c4","Pion Energy Accept Miss",10,10,700,900);
response_miss->Draw();

//RooUnfoldSvd unfold (&response, hist_data, 5);
RooUnfoldBayes unfold (&response, hist_data, 3);
//RooUnfoldBinByBin unfold (&response, hist_data);
TH1D* hReco= (TH1D*) unfold.Hreco();
unfold.PrintTable (cout, hist_true);


TCanvas* c5= new TCanvas("c5","Pion Energy Deconvolution",10,10,700,900);

hReco->Draw();

T.Stop();
T.Print();
}

 

