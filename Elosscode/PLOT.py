import ROOT
from Legend import Legend
from ROOT import TFile
from ROOT import TH2F
from ROOT import TH1F
from ROOT import TGraph
from ROOT import TMath
from ROOT import TMultiGraph
from ROOT import TLatex
from ROOT import TText

from AtlasCommonUtils import *
import optparse,os

SetAtlasStyle()

op = optparse.OptionParser(usage="%prog [opts] ")
op.add_option("-r","--rebin",dest="rebin",
              default=5,
              help="rebin= 2 would merge every 2 bins in histogram")
op.add_option("-E","--Ecut",dest="Ecut",
              default=2.0,
              help="rebin= 2 would merge every 2 bins in histogram")

op.add_option("-p","--pvalue",dest="pvalue",
              default=0.05,
              help="P value corresponding to choosen CL interval. E.g 0.05 for 95% CL")

(p_ops, p_args) = op.parse_args()

rebin = int(p_ops.rebin)
p0cut = float(p_ops.pvalue)
Ecut = float(p_ops.Ecut)

def CutTail(histo):
    bincut = histo.FindBin(Ecut)
    for i in range(bincut,histo.GetNbinsX()):
        histo.SetBinContent(i,0)


def GetKolmogorovp0(hD,hNuclei):
    #hD.Rebin(2)
    #hNuclei.Rebin(2)
    CutTail(hD)
    CutTail(hNuclei)
    hD.Scale(1.0/hD.Integral())
    hNuclei.Scale(1.0/hNuclei.Integral())
    p0_kolmo = hD.KolmogorovTest(hNuclei,"D");
    return p0_kolmo

def GetAllowedRange(g,pvalue):
    p0_cut = -1*ROOT.TMath.Log10(pvalue)
    dE = ROOT.Double()
    p0 = ROOT.Double()
    allowed = {}
    for i in range(g.GetN()):
        g.GetPoint(i,dE,p0)
        if p0 < p0_cut :
            #print dE, p0, p0_cut , ' yes ' 
            allowed[str(float(dE))] = float(p0)
    print allowed
    return allowed
  

def GetProbPlot(files):
    Deuterium = files.Get("D")
    Deuterium.Rebin(rebin)
    #print Deuterium.Print()
    canvas = ROOT.TCanvas()
    hNuclei = {}
    gp0 = ROOT.TGraph()
    g_Ebins = files.Get("g_Ebins")
    #g_Ebins.Print()
    n = g_Ebins.GetN()
    x = ROOT.Double()
    y = ROOT.Double()
    it = 0
    for i in range(n):
        hNuclei[str(i)] = files.Get("Nuclei_bin"+str(i))
        hNuclei[str(i)].Rebin(rebin)
        #print hNuclei[str(i)]
        p0 = GetKolmogorovp0(Deuterium,hNuclei[str(i)])
        g_Ebins.GetPoint(i,x,y)
        print 'Ebins ' , x,y
        if p0>0.001:
            gp0.SetPoint(it,1000*y, -1*ROOT.TMath.Log10(p0))
            it += 1

    return gp0



def AnalyzeNuInterval(nu_bin =0):
   
    files = {}
    files["Pb"] = ROOT.TFile("OUTPUT/fout_Pb_nubin%s.root" %(nu_bin))
    files["Fe"] = ROOT.TFile("OUTPUT/fout_Fe_nubin%s.root" %(nu_bin))
    files["C"] = ROOT.TFile("OUTPUT/fout_C_nubin%s.root" %(nu_bin))

    g_prob = {}
    g_prob["Pb"] = GetProbPlot(files["Pb"])
    g_prob["Fe"] = GetProbPlot(files["Fe"])
    g_prob["C"]  = GetProbPlot(files["C"])

    g_Nubins = files["Pb"].Get("g_Nubins")
    print 'Printing nubins' 
    numin = ROOT.Double()
    numax = ROOT.Double()
    temp = ROOT.Double()
    g_Nubins.GetPoint(0, temp, numin)
    g_Nubins.GetPoint(1, temp, numax)
    print ' Nu interval ' , numin, ' -- ' , numax
    g_Xfcut = files["Pb"].Get("g_Xfcut")
    xflimit = ROOT.Double()
    g_Xfcut.GetPoint(0,temp,xflimit)

    g_prob["Pb"].SetLineColor(2)
    g_prob["Fe"].SetLineColor(4)
    g_prob["C"].SetLineColor(5)

    for key in g_prob.keys():
        g_prob[key].SetLineWidth(3)

    canvas = ROOT.TCanvas()
    multi = ROOT.TMultiGraph()
    multi.Add(g_prob["Pb"],"L")
    multi.Add(g_prob["Fe"],"L")
    multi.Add(g_prob["C"],"L")

    multi.Draw("AL")
    multi.SetMaximum(4)
    multi.GetYaxis().SetNdivisions(2)
    multi.GetXaxis().SetTitle("dE [MeV]")
    multi.GetYaxis().SetTitle("-Log(p_{0})")
    
    c1 = ROOT.TF1("c1","-TMath::Log10(0.05)",-20,120);
    c2 = ROOT.TF1("c2","-TMath::Log10(0.1)",-20,120);
    c3 = ROOT.TF1("c3","-TMath::Log10(0.32)",-20,120);

    c1.SetLineColor(1)
    c2.SetLineColor(1)
    c3.SetLineColor(1)

    c1.SetLineStyle(2)
    c2.SetLineStyle(2)
    c3.SetLineStyle(2)


    c1.Draw("same")
    c2.Draw("same")
    c3.Draw("same")
    legend = Legend("")
    legend.Add(g_prob["C"], "Carbon", "L")
    legend.Add(g_prob["Fe"], "Iron", "L")
    legend.Add(g_prob["Pb"], "Lead", "L")
    latex = TLatex()
    latex.SetNDC()
    latex.DrawLatex(0.2, 0.88, "#font[42]{CLAS Internal}")
    latex.DrawLatex(0.04, 0.42, "#font[42]{95% CL}")
    latex.DrawLatex(0.04, 0.35, "#font[42]{90% CL}")
    latex.DrawLatex(0.04, 0.23, "#font[42]{68% CL}")

    latex.DrawLatex(0.2, 0.82, "#font[42]{%s< #nu <%s , X_{F}> %s, E_{#pi^{+}}< %s GeV}" %(numin,numax,xflimit, Ecut))
    legend.Draw(.75,.95)
    #multi.GetXaxis().SetRangeUser(-100,100)
    canvas.SaveAs("NuBIN%s_Ecut%s.pdf" %(nu_bin,int(100*Ecut)))

    allowed = {}
    allowed["Pb"] = GetAllowedRange(g_prob["Pb"],p0cut)
    allowed["Fe"] = GetAllowedRange(g_prob["Fe"],p0cut)
    allowed["C"] = GetAllowedRange(g_prob["C"],p0cut)

    upE = 0
    doE = 0
    print ' Printing the dictionary with allowed values of p0' 

    minE = {}

    for ikey in allowed.keys():
        print ikey
        if  len(allowed[ikey].keys())>0:
            minE[ikey] = float(min(allowed[ikey], key=allowed[ikey].get))    
        else:
            minE[ikey] = -999
        print minE[ikey]
    print ' minima ' , minE["Pb"] , minE["Fe"], minE["C"]

    numean = (numin + numax)/2.0
    spectrum_Nuclei_Shifted = {}
    spectrum_D = {}
    spectrum_Nuclei = {}
    ratios = {}
    ratios_Shifted = {}
    canvas.Clear()
  
    for key in minE.keys():
        if minE[key] < 0:
            continue 
        bin = int(minE[key]/1.0)
        spectrum_Nuclei_Shifted[key] = files[key].Get("Nuclei_bin"+str(bin))
        spectrum_Nuclei[key] = files[key].Get("Nuclei_bin"+str(0))
        spectrum_D[key] = files[key].Get("D")
        CutTail(spectrum_Nuclei_Shifted[key])
        CutTail(spectrum_D[key])
        #spectrum_Nuclei_Shifted[key].Rebin(rebin)
        #spectrum_Nuclei[key] .Rebin(rebin)
        #spectrum_D[key].Rebin(rebin)

        spectrum_Nuclei_Shifted[key].Scale(1.0/spectrum_Nuclei_Shifted[key].Integral())
        spectrum_Nuclei[key].Scale(1.0/spectrum_Nuclei[key].Integral())
        spectrum_D[key].Scale(1.0/spectrum_D[key].Integral())
 
       
        spectrum_Nuclei_Shifted[key].SetLineColor(4)
        spectrum_Nuclei_Shifted[key].SetMarkerColor(4)
        spectrum_Nuclei_Shifted[key].SetMarkerSize(1)
        spectrum_Nuclei_Shifted[key].SetMarkerStyle(22)
        spectrum_Nuclei_Shifted[key].SetLineStyle(2)

        spectrum_Nuclei[key].SetLineColor(2)
        spectrum_Nuclei[key].SetMarkerColor(2)
        spectrum_Nuclei[key].SetMarkerSize(1)
        spectrum_Nuclei[key].SetMarkerStyle(23)

        legend = Legend("")
        legend.Add(spectrum_D[key], "Deuterium", "L")
        legend.Add(spectrum_Nuclei[key], key  , "L")
        legend.Add(spectrum_Nuclei_Shifted[key], key + " shifted" , "L")


        spectrum_D[key].Draw("hist")
        spectrum_Nuclei_Shifted[key].Draw("Phistsame")
        spectrum_Nuclei[key].Draw("histsame")

        legend.Draw(.6,.88)
        canvas.SaveAs("Spectrum_"+str(key)+"_Nubin_"+str(nu_bin)+"_Ecut%s.pdf" %(int(100*Ecut)))
        canvas.Clear()

        spectrum_Nuclei[key].Divide(spectrum_D[key])
        spectrum_Nuclei_Shifted[key].Divide(spectrum_D[key])
        
        spectrum_Nuclei[key].Draw("P")
        spectrum_Nuclei_Shifted[key].Draw("Psame")
        #canvas.SaveAs("Ratios_"+str(key)+"_Nubin_"+str(nu_bin)+".pdf")


    return minE,numean,xflimit


#MAIN FUNCTION
g_EvsNu = {}
g_EvsNu["Pb"] = ROOT.TGraph()
g_EvsNu["Fe"] = ROOT.TGraph()
g_EvsNu["C"]  = ROOT.TGraph()

g_EvsNu["Pb"].SetMarkerColor(2)
g_EvsNu["Fe"].SetMarkerColor(4)
g_EvsNu["C"].SetMarkerColor(5)



minE = {}
numean = {}

for nubin in range(9):
    minE[str(nubin)], numean[str(nubin)],xflimit = AnalyzeNuInterval(nubin)

print  minE

for nukey in minE.keys():
    for nucleikey in minE[nukey].keys():
        if minE[nukey][nucleikey]>0 :
            g_EvsNu[nucleikey].SetPoint(g_EvsNu[nucleikey].GetN(), numean[nukey], minE[nukey][nucleikey])


g_EvsNu["Pb"].Print()
 

multi_EvsNu = ROOT.TMultiGraph()

for key in g_EvsNu.keys():
    multi_EvsNu.Add(g_EvsNu[key])

canvas = ROOT.TCanvas()
multi_EvsNu.Draw("AP")

multi_EvsNu.SetMinimum(0)
multi_EvsNu.SetMaximum(130)
multi_EvsNu.SetTitle("; #nu [GeV] ; dE [MeV]")
multi_EvsNu.GetXaxis().SetNdivisions(5)
multi_EvsNu.Draw("AP")


latex = TLatex()
latex.SetNDC()
latex.DrawLatex(0.2, 0.88, "#font[42]{CLAS Internal}")
latex.DrawLatex(0.2, 0.82, "#font[42]{X_{F}> %s , E_{#pi^{+}}< %s GeV}" %(xflimit,Ecut))


canvas.SaveAs("EvsNU%s_Ecut%s.pdf" %(rebin,int(100*Ecut)))
