#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TCut.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TTree.h"
#include "TNtuple.h"
#include "math.h"
#include "TMath.h"
#include "TLegendEntry.h"
#include "TGraph.h"

using namespace std;

Double_t Q2_min;
Double_t Q2_max;
Double_t Nu_min;
Double_t Nu_max;
Double_t Phi_min;
Double_t Phi_max;
Double_t Pt2_min;
Double_t Pt2_max;
Double_t E_min;
Double_t E_max;

Double_t delta_Q2;
Double_t delta_Nu;
Double_t delta_Phi;
Double_t delta_Pt2;

Int_t N_Q2;
Int_t N_Nu;
Int_t N_Phi;
Int_t N_Pt2;
Int_t nbins;

TString f_location;
TString fd_ext;
TString fs_ext;
TString material;

Int_t Nu_bin;



string toString(int i)
{
	stringstream ss;
	ss.str("");
	ss<<i;
	return ss.str();
}



int main(int argc, char **argv)
{
  const Double_t Q2_min = 1.;
  const Double_t Q2_max = 4.;
  const Int_t N_Q2= 6.;//6
  
  const Double_t Phi_min = -180.;
  const Double_t Phi_max = 180.;
  const Int_t N_Phi = 12.; //12
  
  const Double_t Pt2_min = 0.;
  const Double_t Pt2_max = 1.5;
  const Int_t N_Pt2 = 6.;//6
    

  const Double_t E_min = 0.;	
  const Double_t E_max = 2.5;  

  const Int_t nbins = 20;      // number of energy bins
  
  const Int_t nSimuFiles = 4;

  TString f_location = "/user/h/hamilton/ThesisProj/data/"; // The location of data and simulation files with ntuples inside
  TString fd_ext = "_data.root";
  TString fs_ext = "_simul.root";

  material = (TString) argv[1];

  Nu_bin = (Int_t) std::stoi(argv[2]);

  Double_t Nu_min = 3.2 + Nu_bin*0.1;
  Double_t Nu_max = Nu_min + 0.1;
  
  delta_Q2 = (Q2_max-Q2_min)/N_Q2;
  delta_Phi = (Phi_max-Phi_min)/N_Phi;
  delta_Pt2 = (Pt2_max-Pt2_min)/N_Pt2;
 
  
/*---------------------------CUTS-----------------------------------------*/

TCut Target_cut1 = "TargType==1"; //cut for Deuterium

TCut Q2_cut_S = Form("Q2>%f && Q2<%f", Q2_min, Q2_max);                 
TCut Nu_cut_S = Form("Nu>%f && Nu<%f", Nu_min, Nu_max);
TCut Phi_cut_S = Form("PhiPQ>%f && PhiPQ<%f", Phi_min, Phi_max);
TCut Pt2_cut_S = Form("Pt*Pt>%f && Pt*Pt<%f", Pt2_min, Pt2_max);
TCut xf_cut = "Xf>0.1";

TCut cuts_simul=Q2_cut_S&&Nu_cut_S&&Pt2_cut_S&&Phi_cut_S&&Target_cut1&&xf_cut;
 
TCut Q2_cut, Nu_cut, Phi_cut, Pt2_cut, cuts_loop;                           

//-----------------------OBTAINING THE NTUPLES-------------------------//

TChain *data = new TChain("ntuple_data");
data->Add(f_location + material + fd_ext);

TChain *reconstructed = new TChain("ntuple_accept");
for(Int_t q = 0; q < nSimuFiles; q++)
  reconstructed->Add(f_location + "D" + toString(q+1) + fs_ext);

TChain *thrown = new TChain("ntuple_thrown");
for(Int_t w = 0; w < nSimuFiles; w++)
  thrown->Add(f_location + "D" + toString(w+1) + fs_ext);

data->SetBranchStatus("*",0);
reconstructed->SetBranchStatus("*",0);
thrown->SetBranchStatus("*",0);

 data->SetBranchStatus("Q2",1);
 data->SetBranchStatus("Nu",1);
 data->SetBranchStatus("PhiPQ",1);
 data->SetBranchStatus("Pt",1);
 data->SetBranchStatus("Xf",1);
 data->SetBranchStatus("Zh",1);
 data->SetBranchStatus("TargType",1);

 reconstructed->SetBranchStatus("Q2",1);
 reconstructed->SetBranchStatus("Nu",1);
 reconstructed->SetBranchStatus("PhiPQ",1);
 reconstructed->SetBranchStatus("Pt",1);
 reconstructed->SetBranchStatus("Xf",1);
 reconstructed->SetBranchStatus("Zh",1);
 reconstructed->SetBranchStatus("TargType",1);

 thrown->SetBranchStatus("Q2",1);
 thrown->SetBranchStatus("Nu",1);
 thrown->SetBranchStatus("PhiPQ",1);
 thrown->SetBranchStatus("Pt",1);
 thrown->SetBranchStatus("Xf",1);
 thrown->SetBranchStatus("Zh",1);
 thrown->SetBranchStatus("TargType",1);


reconstructed->Draw(">>list_acc",cuts_simul,"goff");
reconstructed->SetEventList((TEventList*)gDirectory->Get("list_acc"));

thrown->Draw(">>list_thr",cuts_simul,"goff");
thrown->SetEventList((TEventList*)gDirectory->Get("list_thr"));


//----------------------CREATING THE FILE THAT CONTAINS ALL--------------------//

 TFile *plots = new TFile(Form("output/Difffout_"+material+"_nubin%d.root",Nu_bin),"RECREATE");

//----------------------CREATING THE ENERGY HISTOGRAM (CORRECTED)---------------------//

 
 TH1F *D = new TH1F("D", "D", nbins, E_min, E_max);     // Deuterium Histrograms

 //----------------------LOOPING TO FILL THE HISTOGRAMS-------------------------//
 for (int j = 0; j < N_Q2; j++)
 {
   
    	  Q2_cut = Form("Q2>%f && Q2<%f", Q2_min + j*delta_Q2 , Q2_min + (j+1)*delta_Q2);
       	  for (int k = 0; k < N_Phi; k++)
       	  {
	
       	        Phi_cut = Form("PhiPQ>%f && PhiPQ<%f", Phi_min + k*delta_Phi , Phi_min + (k+1)*delta_Phi);
       		for (int l = 0; l < N_Pt2; l++)		
       		{
		  
       			Pt2_cut = Form("Pt*Pt>%f && Pt*Pt<%f", Pt2_min + l*delta_Pt2 , Pt2_min + (l+1)*delta_Pt2);

			
	       		// DEUTERIUM HISTOGRAMS
				
		       	cuts_loop=Q2_cut&&Nu_cut&&Phi_cut&&Pt2_cut&&xf_cut&&Target_cut1;
				
	       		TH1F *data_histo = new TH1F(Form("data_histo%d%d%d%d", Nu_bin, j, k, l),"",nbins,E_min,E_max);
       			TH1F *thrown_histo = new TH1F(Form("thrown_histo%d%d%d%d", Nu_bin, j, k, l),"",nbins,E_min,E_max);
       			TH1F *reconstructed_histo = new TH1F(Form("reconstructed_histo%d%d%d%d", Nu_bin, j, k, l),"",nbins,E_min,E_max);

				
       			data->Draw("Zh*Nu>>data_histo",cuts_loop,"goff");
       				data_histo->Sumw2();
			     
       			thrown->Draw("Zh*Nu>>thrown_histo",cuts_loop,"goff");
			        thrown_histo->Sumw2();
			       
			reconstructed->Draw("Zh*Nu>>reconstructed_histo",cuts_loop,"goff");
		       		reconstructed_histo->Sumw2();
				

	       		TH1F *acceptance_histo = new TH1F(Form("acceptance_histo%d%d%d%d", Nu_bin, j, k, l),"",nbins,E_min,E_max);
       				acceptance_histo->Divide(reconstructed_histo,thrown_histo,1,1,"B");

       			TH1F *acceptance_correction_histo = new TH1F(Form("acceptance_correction_histo%d%d%d%d", Nu_bin, j, k, l),"",nbins,E_min,E_max);
       				acceptance_correction_histo->Divide(data_histo,acceptance_histo,1,1);

			// Filling the Deuterium histogram
       			D->Add(acceptance_correction_histo, 1);

			delete acceptance_correction_histo;
			delete acceptance_histo;
			delete data_histo;
			delete thrown_histo;
			delete reconstructed_histo;
		
		}
       	}
}


//----------------SAVING THE DATA CORR HISTO IN FILE---------------//

D->Write();
 

//----------------WRITE FINAL HISTOS-----------------------//

TH1F *data_histo_final = new TH1F("data_histo_final","Data",nbins,E_min,E_max);
TH1F *reconstructed_histo_final = new TH1F("reconstructed_histo_final","Reconstruction",nbins,E_min,E_max);
TH1F *thrown_histo_final = new TH1F("thrown_histo_final","Thrown",nbins,E_min,E_max);

data->Draw("Zh*Nu>>data_histo_final",cuts_simul,"goff");
data_histo_final->Write();

reconstructed->Draw("Zh*Nu>>reconstructed_histo_final",cuts_simul,"goff");
reconstructed_histo_final->Write();

thrown->Draw("Zh*Nu>>thrown_histo_final",cuts_simul,"goff");
thrown_histo_final->Write();

 
//----------------DELETING FINAL HISTOS------------------//

delete data_histo_final;
delete reconstructed_histo_final;
delete thrown_histo_final;

//-----------------CLOSING THE FILE-----------------------//
plots->Close();

return 0;
}
