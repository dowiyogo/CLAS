{
//gSystem->Load("/home/rene/roounfold/libRooUnfold.so");
TStopwatch T;
T.Start();
Int_t i;
TFile* simul = TFile::Open("/home/rene/respaldo/conv/CFFTree_D2.root","READ");

TNtuple *Th = (TNtuple *)gROOT->FindObject("tree_thrown");
TNtuple *Ac = (TNtuple *)gROOT->FindObject("tree_accept");
RooUnfoldResponse response(100,0,2.5);
TCanvas* c1= new TCanvas("c1","Pion Energy Thrown",10,10,700,900);
	Th->Draw("Zh*Nu>>thrown()","Zh*Nu<2.0&&Xf>0.1"); //  del TNtuple como un Histograma
	TH1F* hist_true;
	hist_true = (TH1F *)gROOT->FindObject("thrown"); // Busco el objeto y asigno un puntero para el histograma recien dibujado


//TFile* data = TFile::Open("/home/rene/respaldo/arratia/Elosscode/D_10M_Standard_Ntuple.root","READ");
TFile* data = TFile::Open("/home/rene/respaldo/arratia/Elosscode/Pb_data.root","READ");
TNtuple *Da = (TNtuple *)gROOT->FindObject("ntuple_data");
TCanvas* c2= new TCanvas("c2","Pion Energy Pb Target",10,10,700,900);
	Da->Draw("Zh*Nu>>Pb_data()","Zh*Nu<2.0&&Xf>0.1"); //  del TNtuple como un Histograma
		TH1F* hist_data;
	hist_data = (TH1F *)gROOT->FindObject("Pb_data");

TH1F* response_fill= new TH1F("response_fill", "Accepted training data",100,0,2.5);
TH1F* response_miss= new TH1F("response_miss","Rejected training data",100,0,2.5);
Int_t Entries= Th->GetEntries();

std::cout<<"Entries : "<<Entries<<std::endl;

for(i=0;i<Entries;i++)
{
Th->GetEvent(i);
Ac->GetEvent(i);
if(Th->GetLeaf("pid")->GetValue()==8&&Th->GetLeaf("Xf")->GetValue()>0.1&&Ac->GetLeaf("Zh")->GetValue()*Ac->GetLeaf("Nu")->GetValue()<2.0&&Th->GetLeaf("Zh")->GetValue()*Th->GetLeaf("Nu")->GetValue()<2.0)
{
	 if(Ac->GetLeaf("pid")->GetValue()==211&&abs(Ac->GetLeaf("ThetaPQ")->GetValue()-Th->GetLeaf("ThetaPQ")->GetValue())<10&&abs(Ac->GetLeaf("PhiPQ")->GetValue()-Th->GetLeaf("PhiPQ")->GetValue())<10&&Ac->GetLeaf("Xf")->GetValue()>0.1)
	 {response.Fill(Ac->GetLeaf("Zh")->GetValue()*Ac->GetLeaf("Nu")->GetValue(), Th->GetLeaf("Zh")->GetValue()*Th->GetLeaf("Nu")->GetValue()); 
	response_fill->Fill(Ac->GetLeaf("Zh")->GetValue()*Ac->GetLeaf("Nu")->GetValue(), Th->GetLeaf("Zh")->GetValue()*Th->GetLeaf("Nu")->GetValue());
	}
	 else if (Ac->GetLeaf("Xf")->GetValue()>0.1)
	 {response.Miss(Th->GetLeaf("Zh")->GetValue()*Th->GetLeaf("Nu")->GetValue());
		response_miss->Fill(Th->GetLeaf("Zh")->GetValue()*Th->GetLeaf("Nu")->GetValue());
		}
}
}
TCanvas* c3= new TCanvas("c3","Pion Energy Accept Fill",10,10,700,900);
response_fill->Draw();
TCanvas* c4= new TCanvas("c4","Pion Energy Accept Miss",10,10,700,900);
response_miss->Draw();
//RooUnfoldSvd unfold (&response, hist_data, 5);
RooUnfoldBayes unfold (&response, hist_data, 4);
//RooUnfoldBinByBin unfold (&response, hist_data);
TH1D* hReco= (TH1D*) unfold.Hreco();
unfold.PrintTable (cout, hist_true);


TCanvas* c5= new TCanvas("c5","Pion Energy Deconvolution",10,10,700,900);

hReco->Draw();

T.Stop();
T.Print();
}

 

