#include "TROOT.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TMultiGraph.h"
#include "TTree.h"
#include "TH1D.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TH1F.h"
#include "TH1.h"

#include "TH2.h"
#include "TGraph.h"
#include "TMath.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

#include <stdio.h>
#include <stdlib.h>
using namespace std;
Color_t COLOR[6] = { kBlack, kRed+1,  kBlue+1, kGreen+3 , kMagenta , kOrange + 2 };

const int nbins_nu = 9;  
double nu_min;
double nu_max;
//Double_t xbins_nu[nbins_nu+1] = {2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.2};
const double limit_energy = 2.5;
int Nu_bin;

double Calculate_Modified_Xf(Float_t Shift, Float_t Nu, Float_t P, Float_t Pt, Float_t Q2, Float_t W, Float_t Zh)
{
  //cout << "Shift " << Shift << endl;
  //double xf = ((Nu + 0.9385)*(TMath::Sqrt(P*P-Pt*Pt) - TMath::Sqrt(Q2+Nu*Nu)*Zh*Nu/(Nu+0.9385))/W)/((TMath::Sqrt(TMath::Power(W*W-0.9392*0.9392+0.1395*0.1395,2)-4.*0  .1395*0.1395*W*W)/2./W));
  double xf = ((Nu + 0.9385)*(TMath::Sqrt((P+Shift)*(P+Shift)-(P+Shift)*(Pt/P)*(P+Shift)*(Pt/P))-TMath::Sqrt(Q2+Nu*Nu)*(Zh+Shift/Nu)*Nu/(Nu+0.9385))/W)/((TMath::Sqrt(TMath::Power(W*W-0.9392*0.9392+0.1395*0.1395,2)-4.*0.1395*0.1395*W*W)/2./W));
  //cout<< "Xf " << xf << endl;
  return xf;
}

int main(int argc, char *argv[]){
 
  TString Nuclei_Type = argv[1];
  Nu_bin = atoi(argv[2]);
  nu_min = atof(argv[3]); 
  nu_max = atof(argv[4]);
  
  double limit_xf = atof(argv[5]);
  int nbins_E = atoi(argv[6]);
  double step_E = atoi(argv[7])/1000.0;

  if(Nuclei_Type =="C" || Nuclei_Type == "Fe" || Nuclei_Type == "Pb"){
    cout << "The Nuclei type studied is " << Nuclei_Type << endl;                                                                                                     
  }   
  else std::cout << "Please choose as a first argument either C, Fe or Pb " << std::endl;
 
  cout << "Nu interval studied : " << nu_min << " - " << nu_max << endl;

  ostringstream nubin_label;
  nubin_label << nu_min  <<" < #nu < " << nu_max << " GeV ";

  //Reading Tree and Ntuple
  TFile *file;
  if(Nuclei_Type =="Pb") file = new TFile("DATA/Pb_data.root");
  else if(Nuclei_Type =="Fe") file = new TFile("DATA/Fe_data.root");
  else if(Nuclei_Type =="C") file = new TFile("DATA/C_data.root");
  else std::cout<< "Problem, please choose either Pb, Fe or C " << std::endl;
  if(!file) return;
  cout<< "The cut on Xf is " << limit_xf << endl;

  
  TFile* fout = new TFile(Form("OUTPUT/fout_"+Nuclei_Type+"_nubin%d.root",Nu_bin),"RECREATE");

  TTree* tree = (TTree*)file->Get("ntuple_data");
  
  //Reading Branches with appropiate variables. 
  Float_t TargType;
  Float_t Q2;
  Float_t Nu;
  Float_t Xb;
  Float_t W;
  Float_t SectorEl;
  Float_t ThetaPQ;
  Float_t PhiPQ;
  Float_t Zh;
  Float_t Pt;
  Float_t W2p;
  Float_t Xf;
  Float_t T;
  Float_t P;
  Float_t T4;
  Float_t deltaZ;
  Float_t NmbPion;
  tree->SetBranchAddress("TargType",&TargType);
  tree->SetBranchAddress("Q2",&Q2);
  tree->SetBranchAddress("Nu",&Nu);
  tree->SetBranchAddress("Xb",&Xb);
  tree->SetBranchAddress("W",&W);
  tree->SetBranchAddress("SectorEl",&SectorEl);
  tree->SetBranchAddress("ThetaPQ",&ThetaPQ);
  tree->SetBranchAddress("PhiPQ",&PhiPQ);
  tree->SetBranchAddress("Zh",&Zh);
  tree->SetBranchAddress("Pt",&Pt);
  tree->SetBranchAddress("W2p",&W2p);
  tree->SetBranchAddress("Xf",&Xf);
  tree->SetBranchAddress("T",&T);
  tree->SetBranchAddress("P",&P);
  tree->SetBranchAddress("T4",&T4);
  tree->SetBranchAddress("deltaZ",&deltaZ);
  tree->SetBranchAddress("NmbPion",&NmbPion);

  Int_t nentries = tree->GetEntries();
  double energy_shift = 0;
  ////////////////////////////////////////////////////////////
  ////////FILLING, THE HISTOGRAMS.                     
  ///////////////////////////////////////////////////////////                                                                                                         
  TGraph* g_p0_Chi2 = new TGraph();
  TGraph* g_p0_Kolmo = new TGraph();

 /////////////////////////////////////////////////////////
  //Starting the loop over the Energy shift. 


  //Histograms with Energy distribution. 
  TH1F *D = new TH1F("D","D",100,0.,2.8);
  TH2F *Nuclei = new TH2F("Nuclei","Nuclei",100,0.0,2.8,10,0.0,100.0);
  D->Sumw2();
  //std::cout<<"Create histograms" << std::endl;  
  std::map<int,TH1F*> histograms;

  for(int i = 0; i<= nbins_E; i++){
    //std::cout<< " i " << i << std::endl;
    histograms[i] = new TH1F(Form("Nuclei_bin%d",i),Form("Nuclei_bin%d",i),100,0.0,2.8);
    //histograms[i]->Print();
    histograms[i]->Sumw2();
  }  
  ///////////////////////////////////////////////////////////////////////
  //Filling Histograms //////////////////////////////////////////////////   
  ///////////////////////////////////////////////////////////////////////

  std::cout<<"Entering the loop over entries "<< nentries <<std::endl;

  for(Int_t j=1; j<= nentries; j++){
    if(j%5000000==0) std::cout<< "Processing event number " << j/1000000.0 << "M "<< std::endl;
    tree->GetEntry(j);
    //Apply Cuts bin in Nu
    if(Nu> nu_max || Nu < nu_min) continue;
    //Apply cuts for Deuterium, fill with Energy = Zh*Nu
    if(TargType==1 && Xf> limit_xf && Zh*Nu < limit_energy){
       D->Fill(Zh*Nu);
     }
    //Apply cuts for Nuclei, fill with Energy = Zh*Nu + energy shift. 

    if(TargType!=2) continue;
    for(Int_t i_shift = 0; i_shift <= nbins_E; i_shift++){
     energy_shift = step_E*i_shift; //In GeV // 5 MeV steps.                                                                                                         
     double Xf_Nuclei = Calculate_Modified_Xf( energy_shift, Nu, P, Pt,  Q2,  W, Zh);
     if(Xf_Nuclei> limit_xf && Zh*Nu + energy_shift < limit_energy ){
       histograms[i_shift]->Fill(Zh*Nu+energy_shift);
     }
    }//END LOOP OVER ENTRIES
  }
  std::cout<< "END LOOP OVER ENTRIES" << std::endl;

  //Saving the Nu and dE bins used in analysis 
  TGraph* g_Ebins = new TGraph();
  TGraph* g_Nubins = new TGraph();
  TGraph* g_Xfcut = new TGraph();
  std::cout<< "Setting to the plot " << nu_min << " " << nu_max << std::endl;
  g_Nubins->SetPoint(0,0,nu_min);
  g_Nubins->SetPoint(1,1,nu_max);
  g_Xfcut->SetPoint(0,0,limit_xf);

  for(Int_t i_shift = 0; i_shift <= nbins_E; i_shift++){
    energy_shift = step_E*i_shift; //In GeV // 5 MeV steps.                                                                                                            
    g_Ebins->SetPoint(i_shift,i_shift,energy_shift);
  }



  fout->cd();
  D->Write();
  //Nuclei->Write();
  for(int i = 0; i<= nbins_E; i++){
    histograms[i]->Write();
  }
  g_Ebins->Write("g_Ebins");
  g_Nubins->Write("g_Nubins");
  g_Xfcut->Write("g_Xfcut");
  //fout->Print();
  std::cout<<" ABOUT TO CLOSE " << std::endl;
  fout->Close();
  
  std::cout<< " BYE BYE " << std::endl;
  return 1;
}
