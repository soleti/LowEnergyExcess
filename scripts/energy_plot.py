import os
from ROOT import gStyle, gDirectory, TFile, TH1F, TCanvas, THStack, kBlack, kGreen, kBlue, TLegend, kAzure, kOrange, kMagenta
from array import array
import math

filebase = os.environ['LARLITE_USERDEVDIR']+'/LowEnergyExcess/output/singleE_cosmic_selection_mc.root'


particles = { 
    'mu' : 13,
    'antimu' : -13,
    'e' : 11,
    'pos' : -11, 
    'photon' : 22
}

colors = { 
    'mu' : kGreen+4,
    'antimu' : kGreen+2,
    'pos' : kBlue+1,
    'e' : kAzure+1,
    'photon' : kOrange
}

labels = { 
    'mu' : "#mu^{-}",
    'antimu' : "#mu^{+}",
    'e' : "e^{-}",
    'pos' : "e^{+}", 
    'photon' : "#gamma"
}


h_energy_mu = TH1F("h_energy_mu","",20,0,80)
h_energy_e = TH1F("h_energy_e","",20,0,80)
h_energy_antimu = TH1F("h_energy_antimu","",20,0,80)
h_energy_pos = TH1F("h_energy_pos","",20,0,80)
h_energy_gamma = TH1F("h_energy_gamma","",20,0,80)
h_tagger_mu = TH1F("h_tagger_mu","",20,0,80)
h_tagger_antimu = TH1F("h_tagger_antimu","",20,0,80)
h_tagger_e = TH1F("h_tagger_e","",20,0,80)
h_tagger_pos = TH1F("h_tagger_pos","",20,0,80)
h_tagger_gamma = TH1F("h_tagger_gamma","",20,0,80)

h_total = THStack("h_total", ";E [GeV/c^{2}]; N. Events / 2 GeV/c^{2}")

histograms = { 
    'mu' : h_energy_mu,
    'antimu' : h_energy_antimu,
    'e' : h_energy_e,
    'pos' : h_energy_pos, 
    'photon' : h_energy_gamma
}

histograms_tagger = { 
    'mu' : h_tagger_mu,
    'antimu' : h_tagger_antimu,
    'e' : h_tagger_e,
    'pos' : h_tagger_pos, 
    'photon' : h_tagger_gamma
}

scalefactor = 1000. #from MeV to GeV

gStyle.SetOptStat(0)

legend = TLegend(0.53,0.61,0.88,0.85)

f = TFile(filebase)
    
chain = gDirectory.Get("cosmicShowers")
entries = chain.GetEntriesFast()

for particle in particles:

    for entry in xrange(entries):
        ientry = chain.LoadTree(entry)
        nb = chain.GetEntry(entry)
    
        default_cut = chain._e_Edep > 50.
        tracklen_cut = chain._longestTrackLen < 100.
        fidvolcut = chain._x_vtx > 10 and chain._x_vtx < 246.35 and chain._y_vtx > -106.5 and chain._y_vtx < 106.5 and chain._z_vtx > 10 and chain._z_vtx < 1026.8;
        nu_cut = chain._e_nuReco > 250

        if default_cut and tracklen_cut and chain._parentPDG == particles[particle] and nu_cut and fidvolcut:                           
            if not chain._in_tagger:
                histograms[particle].Fill(chain._parent_e/scalefactor)
                if chain._parentPDG == 22:
                    print chain._in_ob
                
            else:
                histograms_tagger[particle].Fill(chain._parent_e/scalefactor)
                    
    histograms[particle].Scale(2.52)
    histograms[particle].SetFillColor(colors[particle])
    histograms[particle].SetLineColor(kBlack)
    print labels[particle]+str(histograms[particle].Integral())
    histograms_tagger[particle].SetFillStyle(3002)
    histograms_tagger[particle].Scale(2.52)
    histograms_tagger[particle].SetFillColor(colors[particle])
    histograms_tagger[particle].SetLineColor(kBlack)
    legend.AddEntry(histograms[particle],labels[particle]+"- not tagged","f")
    legend.AddEntry(histograms_tagger[particle],labels[particle],"f")
        

f.Close()


h_total.Add(h_energy_gamma)
h_total.Add(h_energy_antimu)


h_total.Add(h_energy_mu)


h_total.Add(h_energy_e)

h_total.Add(h_energy_pos)

h_total.Add(h_tagger_gamma)
h_total.Add(h_tagger_antimu)
h_total.Add(h_tagger_mu)
h_total.Add(h_tagger_e)


h_total.Add(h_tagger_pos)


legend.SetTextFont(42)
legend.SetShadowColor(0)
legend.SetBorderSize(0)


c = TCanvas("c")
h_total.Draw()
legend.Draw()
c.Update()

raw_input()