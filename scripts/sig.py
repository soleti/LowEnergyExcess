#!/usr/bin/env python3.4

from ROOT import TGraph, kRed, kBlue, kOrange, TCanvas, TLegend, kAzure
from array import array

stages = array('d',[1,2,3,4,5,6,7,8,9])
lee = array('d',[6.54,6.79,6.81,6.95,0,0,0,0,0])
sbn = array('d',[0,0,0,0,0,9.25,9.74,9.81,10.06])
first_lee = lee[0]

labels = ["No upgrades","CRT phase A", "CRT phase B", "CRT + OB", "", "No upgrades","CRT phase A", "CRT phase B", "CRT + OB"]



no = TGraph(9,stages,array('d',[6.54,0,0,0,0,0,0,0,0]))
phaseA = TGraph(9,stages,array('d',[0,6.79,0,0,0,0,0,0]))
phaseB = TGraph(9,stages,array('d',[0,0,6.81,0,0,0,0,0]))
ob = TGraph(9,stages,array('d',[0,0,0,6.95,0,0,0,0]))

no_sbn = TGraph(9,stages,array('d',[0,0,0,0,0,9.25,0,0,0]))
phaseA_sbn = TGraph(9,stages,array('d',[0,0,0,0,0,0,9.74,0,0]))
phaseB_sbn = TGraph(9,stages,array('d',[0,0,0,0,0,0,0,9.81,0]))
ob_sbn = TGraph(9,stages,array('d',[0,0,0,0,0,0,0,0,10.06]))

xax = no.GetXaxis()
i=1
while i <= xax.GetXmax():
    bin_index = xax.FindBin(i)
    xax.SetBinLabel(bin_index,labels[i-1])
    i+=1
xax.LabelsOption("h")

for i in range(4):
    lee[i] = (lee[i]-first_lee)/first_lee

for i in range(4):
    lee[i] *= 100 

for i in range(len(sbn)):
    sbn[i] = (sbn[i]-9.25)/9.25*100

c = TCanvas("c")

g_lee = TGraph(4,stages,lee)
g_sbn = TGraph(9,stages,sbn)

g_lee.SetLineWidth(3)
g_lee.SetLineColor(kBlue+1)
g_sbn.SetLineWidth(3)
g_sbn.SetLineColor(kRed+1)

no.SetFillColorAlpha(kBlue+1,0.5)
phaseA.SetFillColorAlpha(kBlue+2,0.5)
phaseB.SetFillColorAlpha(kBlue+3,0.5)
ob.SetFillColorAlpha(kBlue+4,0.5)

no_sbn.SetFillColorAlpha(kRed+1,0.5)
phaseA_sbn.SetFillColorAlpha(kRed+2,0.5)
phaseB_sbn.SetFillColorAlpha(kRed+3,0.5)
ob_sbn.SetFillColorAlpha(kRed+4,0.5)

c = TCanvas("c")
no.GetYaxis().SetTitle("Significance [#sigma]")
no.Draw("ABY+")
phaseA.Draw("B")
phaseB.Draw("B")
ob.Draw("B")

no_sbn.Draw("B")
phaseA_sbn.Draw("B")
phaseB_sbn.Draw("B")
ob_sbn.Draw("B")

g_lee.Draw("PL")
g_sbn.Draw("PL")
c.Update()
input()