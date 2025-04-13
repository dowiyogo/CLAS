{
gROOT->Reset();

//establecer opción para no mostrar en el histograma los datos estadísticos asociados (media, SD, número de entradas, etc)
gStyle->SetOptStat(00000);

//definir un nuevo tipo de variable para usarlas en la creación automática de los títulos
typedef char FLS[12];
FLS base_func[3]={"Delta","Gauss","Exponential"};
FLS res_func[2]={"Gauss","Landau"};

//crear 3 histogramas para la función base, la de resolución y la convolución de ambas
base=new TH1F("base","base",100,-5,5);
base->SetLineStyle(2); //establecer estilo de línea punteada
res=new TH1F("res","res",100,-5,5);
res->SetLineColor(1); //hacer el histograma de color negro
res->SetLineStyle(2);
conv=new TH1F("conv","conv",100,-5,5);
conv->SetLineColor(2); //color rojo en la línea del histograma

//ciclos para crear los histogramas de una sola vez
for (Int_t i=0;i<3;i++){ //recorrerá las diferentes funciones base
	for (Int_t j=0;j<2;j++){ //recorrerá las funciones de resolución
		base->Reset();//reseteo de los 3 histogramas para que al cambiar de ciclo no se agreguen datos sobre un histograma lleno
		res->Reset();
		conv->Reset();
		for (Int_t k=0;k<10000000;k++){ //la cantidad de eventos que queremos generar
			Double_t x, y, z;
			if (i==0) x=0; //caso de distribución delta de Dirac
			else if (i==1) x=gRandom->Gaus(0,1); //generación número aleatorio distribución gaussiana
			else if (i==2) x=gRandom->Exp(5); //número aleatorio de función exponencial
			if (j==0) y=gRandom->Gaus(0,1); //número aleatorio de distribución gaussiana pero para la función de resolución
			else if (j==1) y=gRandom->Landau(0,1); //número aleatorio con distribución Landau
			z=x+y; //convolución... función de distribución base más la función de resolución
			base->Fill(x); //llenado de histograma evento por evento
			res->Fill(y);
			conv->Fill(z);
		}
		THStack * st=new THStack("Convolution",Form("%s+%s convolution",base_func[i],res_func[j]));//pila de histogramas para evitar cierto tipo de dificultades al dibujar
		st->Add(base);//agregar a la pila creada los histogramas creados más arriba
		st->Add(res);
		st->Add(conv);

		TLegend * l=new TLegend(0.75,0.75,0.9,0.9); //creación de la leyenda que se verá junto con los histogramas
		l->AddEntry(base,base_func[i]); //agregar los histogramas a la pila
		l->AddEntry(res,res_func[j]);
		l->AddEntry(conv,"Convolution");

		TCanvas * c1=new TCanvas("c1","c1",1920,1080); //crear el lienzo donde se dibujará

		st->Draw("nostack"); //dibujar la pila con cada histograma por separado y no combinarlos en 1
		l->Draw(); //dibujar la leyenda sobre el histograma

		if (i==0 && j==0) c1->SaveAs("convolution_hw.pdf("); //en el caso de que sea el primer ciclo se hace creación de un archivo pdf al que luego se le agregarán más páginas (a eso se refiere el paréntesis abierto en el final del nombre)
		else if (i==2 && j==1) c1->SaveAs("convolution_hw.pdf)"); //en caso de que sea el último ciclo cerrar el archivo pdf
		else c1->SaveAs("convolution_hw.pdf"); //en cualquier otro caso agregar una página al archivo ya abierto
	}
}

}
