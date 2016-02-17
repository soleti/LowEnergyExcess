import os
from ROOT import gStyle, gDirectory, TFile, TH1F, TCanvas, THStack, kBlack, kGreen, kWhite, kBlue, kRed, TLegend
from array import array


files = [os.environ['LARLITE_USERDEVDIR']+'/LowEnergyExcess/output/singleE_cosmic_selection_mc_notag.root', os.environ['LARLITE_USERDEVDIR']+'/LowEnergyExcess/output/singleE_cosmic_selection_mc.root']

h_cosmic_tag = TH1F("h_cosmic_tag", ";Reconstructed neutrino energy [GeV/c^{2}]; N. Events / 0.25 GeV^{2}", 11, 0.25, 3)
h_cosmic_notag = TH1F("h_cosmic_notag", ";Reconstructed neutrino energy [GeV/c^{2}]; N. Events / 0.25 GeV^{2}", 11, 0.25, 3)

histos =[h_cosmic_notag, h_cosmic_tag]

scalefactor = 1000. #from MeV to GeV

for i, file in enumerate(files):
    f = TFile(file)
    
    chain = gDirectory.Get("cosmicShowers")
    entries = chain.GetEntriesFast()
    
    for entry in xrange(entries):
        ientry = chain.LoadTree(entry)
        nb = chain.GetEntry(entry)
        
        default_cut = chain._e_Edep > 50.
        tracklen_cut = chain._longestTrackLen < 100.
        
        if default_cut and tracklen_cut:
            histos[i].Fill(chain._e_nuReco/scalefactor)
        
    f.Close()
    
c1 = TCanvas("c1")
h_cosmic_notag.SetFillStyle(3002)
h_cosmic_tag.SetFillStyle(3002)
h_cosmic_notag.SetFillColor(kBlue)
h_cosmic_tag.SetFillColor(kRed)
h_cosmic_tag.SetLineColor(kRed+1)

h_cosmic_notag.Draw()
h_cosmic_tag.Draw("same")
raw_input()