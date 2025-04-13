

void unf()
{
  TFile *datafile = new TFile("/home/luciano/Physics/CLAS/data/Fe_data.root");
  TFile *simufile = new TFile("/home/luciano/Physics/CLAS/data/CFFTree_Fe1.root");

  TTreeReader datareader("ntuple_data",datafile);
  TTreeReader thrownreader("tree_thrown",simufile);
  TTreeReader acceptreader("tree_accept",simufile);

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
	    response_fill->Fill(accept_energy);
	  } else {
	    response_miss->Fill(thrown_energy);
	  }
	}
      }

    }
  }
  response_fill->Draw();
  new TCanvas();
  response_miss->Draw();
}
