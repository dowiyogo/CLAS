// reading a text file
{
  gROOT->Reset();
#include <fstream>
#include <string>
  TF1* fc = new TF1("fc", "TMath::CauchyDist(x, [0], [1])", -5, 5);
  fc->SetParameters(0, 1);
  fc->Draw();

  TFile* file_final = new TFile("cesium137.root","RECREATE");
  TNtuple* datos= new TNtuple("datos", "datos", "Dato");
  TH1I* histo= new TH1I("histo","histograma",1200,0,1200);
  histo->SetXTitle("Canales");
  histo->SetYTitle("Cuentas");
  histo->SetAxisRange(0,1200,"X");
  double Dato= 0;// inicializar valores
  
  ifstream lisfile ("cesium137");
  while(! lisfile.eof()) {
	lisfile>>Dato;
	datos->Fill(Dato);//revisar!!!!
	histo->Fill(Dato);
  }
//  histo->Draw("*");
  file_final->Write();

  
  //myfile.close();
  lisfile.close();
  file_final->Close(); 
  delete file_final;
  return 0;
}

float *conv(float *A, float *B, int lenA, int lenB, int *lenC)
{
	int nconv;
	int i, j, i1;
	float tmp;
	float *C;
 
	//allocated convolution array	
	nconv = lenA+lenB-1;
	C = (float*) calloc(nconv, sizeof(float));
 
	//convolution process
	for (i=0; i<nconv; i++)
	{
		i1 = i;
		tmp = 0.0;
		for (j=0; j<lenB; j++)
		{
			if(i1>=0 && i1<lenA)
				tmp = tmp + (A[i1]*B[j]);
 
			i1 = i1-1;
			C[i] = tmp;
		}
	}
 
	//get length of convolution array	
	(*lenC) = nconv;
 
	//return convolution array
	return(C);
}

