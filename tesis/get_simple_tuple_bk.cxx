#include "Riostream.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TClasTool.h"
#include "TIdentificator.h"
#include "TMath.h"


int main(int argc, char **argv)
{
  //gROOT->Reset();

  bool simul_key = 0;
  
  TClasTool *input = new TClasTool();
  input->InitDSTReader("ROOTDSTR");
  
  if(argc == 1) {    
    char File[200];
    //system("ls -l *.root > dataFiles.txt");
    ifstream in("dataFiles.txt", ios::in);
    if (!in) {
        cerr << "File Not Opened!" << endl;
        exit(1);
    }
    while (in >> File) {
        input->Add(File);
    }
    in.close();
    //system("rm dataFiles.txt");
  } else {
    simul_key = 1;
    char File[200];
    system("ls -1 *.root > simulFiles.txt");
    ifstream in("simulFiles.txt", ios::in);
    if (!in) {
        cerr << "File Not Opened!" << endl;
        exit(1);
    }
    while (in >> File) {
        input->Add(File);
    }
    in.close();
    system("rm simulFiles.txt");
  }
  const char* NtupleName;
  const char* VarList = "TargType:Q2:Nu:Xb:W:SectorEl:ThetaPQ:PhiPQ:Zh:Pt:W2p:Xf:T:P:T4:deltaZ:E:Ee:Pe:Ect:Sct:Ecr:Scr:evnt:Px:Py:Pz:Xe:Ye:Ze:Xec:Yec:Zec:ECX:ECY:ECZ:Pex:Pey:Pez";
  Int_t Nvar = 39;
  Float_t *vars = new Float_t[Nvar];
  TVector3 *vert;
  TIdentificator *t = new TIdentificator(input);
  
  Long_t nEntries = (Long_t) input->GetEntries();

  TFile *output;
  if(simul_key == 0) {
    NtupleName = "ntuple_data";
    output = new TFile("prune_data.root", "RECREATE", "Data of particles");
  } else { 
    NtupleName = "ntuple_accept";
    output = new TFile("prune_simul.root", "RECREATE", "Data of particles");
  }

  TNtuple *ntuple = new TNtuple(NtupleName,"Pion pluses",VarList);
  TNtuple *ntuple_thrown = 0;
  if(simul_key == 1) ntuple_thrown = new TNtuple("ntuple_thrown","Pion pluses",VarList);
  
  input->Next();
  
  for (Int_t k = 0; k < nEntries; k++) 
  {
    Int_t nRows = input->GetNRows("EVNT");
    if(nRows>0 && (t->GetCategorization(0)) == "electron" && t -> Q2() > 1. && t -> W() > 2. && t -> Nu() / 5.015 < 0.85)
//    if(nRows>0 && (t->GetCategorization(0)) == "electron")  
    {
      Int_t NmbPion = 0;
      for (Int_t i = 1; i < nRows; i++) 
      {
      	TString category = t->GetCategorization(i);
      	if (category == "gamma") 
        {
	        vars[0] = t -> ElecVertTarg();
	        vars[1] = t -> Q2();
	        vars[2] = t -> Nu();
	        vars[3] = t -> Xb();
	        vars[4] = t -> W();
	        vars[5] = t -> Sector(0);
	        vars[6] = t -> ThetaPQ(i);
	        vars[7] = t -> PhiPQ(i);
	        vars[8] = t -> Zh(i);
	        vars[9] = TMath::Sqrt(t -> Pt2(i));
	        vars[10] = t -> Mx2(i);
	        vars[11] = t -> Xf(i);
	        vars[12] = t -> T(i);
	        vars[13] = t -> Momentum(i);
	        vars[14] = t -> TimeCorr4(0.139570,i);
	        vars[15] = (t -> Z(i)) - (t -> Z(0));
	        vars[16] = TMath::Max(t->Etot(i),t->Ein(i)+t->Eout(i));;
          vars[17] = TMath::Max(t->Etot(0),t->Ein(0)+t->Eout(0));
          vars[18] = t->Momentum(0);
          vars[19] = t->TimeEC(0);
          vars[20] = t->TimeSC(0);
          vars[21] = t->PathEC(0);
          vars[22] = t->PathSC(0);
          vars[23] = k;
          vars[24] = t->Px(i);
          vars[25] = t->Py(i);
          vars[26] = t->Pz(i);
          vars[27] = t->X(0);
          vars[28] = t->Y(0);
          vars[29] = t->Z(0);
          vert = t->GetCorrectedVert();
          vars[30] = vert->X(); 
          vars[31] = vert->Y(); 
          vars[32] = vert->Z();
          vars[33] = t->XEC(0); 
          vars[34] = t->YEC(0); 
          vars[35] = t->ZEC(0);
          vars[33] = t->Px(0); 
          vars[34] = t->Py(0); 
          vars[35] = t->Pz(0);
	        ntuple->Fill(vars);
    	  }
      }
    }

    if(simul_key == 1 && t -> Id(0,1)==3 /*&& t -> Q2(1) > 1. && t -> W(1) > 2. && t -> Nu(1) / 5.015 < 0.85*/) 
    {
      Int_t NmbPion = 0;
      for(Int_t i=1; i < input->GetNRows("GSIM"); i++) 
      {
      	if(t -> Id(i,1)==8) 
        {
	        vars[0] = t -> ElecVertTarg(1);
	        vars[1] = t -> Q2(1);
	        vars[2] = t -> Nu(1);
	        vars[3] = t -> Xb(1);
	        vars[4] = t -> W(1);
	        vars[5] = t -> Sector(0,1);
	        vars[6] = t -> ThetaPQ(i,1);
	        vars[7] = t -> PhiPQ(i,1);
	        vars[8] = t -> Zh(i,1);
	        vars[9] = TMath::Sqrt(t -> Pt2(i,1));
	        vars[10] = t -> Mx2(i,1);
	        vars[11] = t -> Xf(i,1);
	        vars[12] = t -> T(i,1);
	        vars[13] = t -> Momentum(i,1);
	        vars[14] = 0.;
	        vars[15] = (t -> Z(i,1)) - (t -> Z(0,1));
	        vars[16] = ++NmbPion ;
	        ntuple_thrown->Fill(vars);
    	  }
      }
    }
    
    input->Next();
  }
  
  output->Write();
  output->Close();
  cout << "Done." << endl;
  return 0;
}
