#!/usr/bin/env python

from ROOT import TGraph, kRed, kBlue, kOrange, TCanvas, TLegend, kAzure
from array import array

stages = array('d',[1,2,3,4])
lee = array('d',[6.54,6.79,6.81,6.95])
sbn = array('d',[9.25,9.74,9.81,10.06])
lee_noalgo = array('d',[4.81,4.97,5.04,5.29])
sbn_noalgo = array('d',[6.81,7.11,7.32,8.00])

first_lee = lee[0]
first_sbn = sbn[0]
first_noalgo = lee_noalgo[0]
first_sbn_noalgo = sbn_noalgo[0]

for i in xrange(len(lee)):
    lee[i] = (lee[i]-first_lee)/first_lee
    sbn[i] = (sbn[i]-first_sbn)/first_sbn
    lee_noalgo[i] = (lee_noalgo[i]-first_noalgo)/first_noalgo
    sbn_noalgo[i] = (sbn_noalgo[i]-first_sbn_noalgo)/first_sbn_noalgo


for i in xrange(len(lee)):
    lee[i] *= 100 
    sbn[i] *= 100
    lee_noalgo[i] *= 100
    sbn_noalgo[i] *= 100


labels = ["No upgrades","CRT phase A", "CRT phase B", "CRT + OB"]
c = TCanvas("c")

g_lee = TGraph(len(lee),stages,lee)
g_sbn = TGraph(len(sbn),stages,sbn)
g_noalgo = TGraph(len(lee_noalgo),stages,lee_noalgo)
g_sbn_noalgo = TGraph(len(sbn_noalgo),stages,sbn_noalgo)

g_lee.Draw("APL")
g_lee.SetMarkerStyle(20)
g_lee.SetLineColor(kRed+1)
g_lee.SetLineWidth(2)
g_lee.GetYaxis().SetTitle("Significance increase [%]")

xax = g_lee.GetXaxis()
i=1
while i <= xax.GetXmax():
    bin_index = xax.FindBin(i)
    xax.SetBinLabel(bin_index,labels[i-1])
    i+=1
xax.LabelsOption("h")


g_sbn.Draw("PL")
g_sbn.SetMarkerStyle(21)
g_sbn.SetLineColor(kOrange+1)
g_sbn.SetLineWidth(2)

#g_noalgo.Draw("PL")
g_noalgo.SetMarkerStyle(4)
g_noalgo.SetLineColor(kBlue+1)
g_noalgo.SetLineWidth(2)
g_noalgo.SetLineStyle(2)


#g_sbn_noalgo.Draw("PL")
g_sbn_noalgo.SetMarkerStyle(25)
g_sbn_noalgo.SetLineStyle(2)

g_sbn_noalgo.SetLineColor(kAzure-9)
g_sbn_noalgo.SetLineWidth(2)


l = TLegend(0.6,0.6,0.9,0.9)
l.AddEntry(g_lee,"3 years", "lp")
#l.AddEntry(g_noalgo,"3 years - no algos", "lp")
l.AddEntry(g_sbn,"6 years", "lp")
#l.AddEntry(g_sbn_noalgo,"6 years - no algos", "lp")

l.SetTextFont(43)
l.SetBorderSize(0)
l.SetShadowColor(0)
l.Draw()
c.Update()

raw_input()