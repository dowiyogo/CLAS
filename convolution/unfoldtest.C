{
//gSystem->Load("/home/rene/roounfold/libRooUnfold.so");
TStopwatch T;
T.Start();
Int_t i;
TFile* simul = TFile::Open("/home/rene/respaldo/conv/CFF_Pb.root","READ");

TNtuple *Th = (TNtuple *)gROOT->FindObject("ntuple_thrown");
TNtuple *Ac = (TNtuple *)gROOT->FindObject("ntuple_accept");
RooUnfoldResponse response(100,0,5);
	Th->Draw("Zh*Nu>>thrown()"); //  del TNtuple como un Histograma
	TH1F* hist_true;
	hist_true = (TH1F *)gROOT->FindObject("thrown"); // Busco el objeto y asigno un puntero para el histograma recien dibujado

TCanvas* canvas1= new TCanvas("canvas1","Deconvolution",10,10,700,900);


TFile* data = TFile::Open("/home/rene/respaldo/arratia/Elosscode/Pb_data.root","READ");
TNtuple *Da = (TNtuple *)gROOT->FindObject("ntuple_data");
	Da->Draw("Zh*Nu>>accept()"); //  del TNtuple como un Histograma
	TH1F* hist_data;
	hist_data = (TH1F *)gROOT->FindObject("accept");

Int_t Entries= Th->GetEntries();

std::cout<<"Entries : "<<Entries<<std::endl;

for(i=0;i<Entries;i++)
{
Th->GetEvent(i);
Ac->GetEvent(i);
if(Th->GetLeaf("pid")->GetValue()==8&&Th->GetLeaf("Xf")->GetValue()>0.1)
{
	 if(Ac->GetLeaf("pid")->GetValue()==211&&abs(Ac->GetLeaf("ThetaPQ")->GetValue()-Th->GetLeaf("ThetaPQ")->GetValue())<10&&abs(Ac->GetLeaf("PhiPQ")->GetValue()-Th->GetLeaf("PhiPQ")->GetValue())<10&&Ac->GetLeaf("Xf")->GetValue()>0.1)
	 response.Fill(Ac->GetLeaf("Zh")->GetValue()*Ac->GetLeaf("Nu")->GetValue(), Th->GetLeaf("Zh")->GetValue()*Th->GetLeaf("Nu")->GetValue()); 
	 else if (Ac->GetLeaf("Xf")->GetValue()>0.1)
	 response.Miss(Th->GetLeaf("Zh")->GetValue()*Th->GetLeaf("Nu")->GetValue());
}
}


RooUnfoldSvd unfold (&response, hist_data, 5);
//RooUnfoldBayes unfold (&response, hist_data, 100);
//RooUnfoldBinByBin unfold (&response, hist_data);
TH1D* hReco= (TH1D*) unfold.Hreco();
unfold.PrintTable (cout, hist_true);




hReco->Draw();

T.Stop();
T.Print();
}

 

