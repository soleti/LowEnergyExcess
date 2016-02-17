import os
from ROOT import gStyle, gDirectory, TFile, TH1F, TCanvas, THStack, kBlack, kGreen, kWhite, kBlue, kRed, TLegend
from array import array
import math

def sigmaCalc(h_signal, h_background):
    chi2 = sum([h_signal.GetBinContent(i)**2/h_background.GetBinContent(i) for i in xrange(h_signal.GetNbinsX())])
    return math.sqrt(chi2)

scaled_2years = False
ob = False

filebase = os.environ['LARLITE_USERDEVDIR']+'/LowEnergyExcess/output/'

filenames = {
    'nue':'singleE_nue_selection_mc.root',
    'numu':'singleE_numu_selection_mc.root',
    'nc':'singleE_nc_selection_mc.root',
    'cosmic':'singleE_cosmic_selection_mc.root',
    'lee':'singleE_LEE_selection_mc.root'
}

treenames = { 
    'nue' : 'beamNuE',
    'cosmic' : 'cosmicShowers',
    'numu' : 'beamNuMu',
    'nc' : 'beamNC',
    'lee' : 'LEETree'
}

labels = { 
    'nue' : 'Beam intrinsic #nu_{e}',
    'cosmic' : 'Cosmic rays',#'CRY Cosmic, in-time',
    'numu' : 'Beam intrinsic #nu_{#mu}',
    'nc' : 'Beam intrinsic NC', 
    'lee' : 'Low energy excess'
}

colors = { 
    'nue' : kGreen-2,
    'numu' : kBlue-5,
    'nc' : kBlue-9,
    'cosmic' : kRed-3,
    'lee' : kWhite
}

#These weights are to scale to 6.6e20 POT
scaling_weights = { 
    'nue' : 6.6e20/(2.706e15*99600), #99600 is # of total BNB events looped over
    'cosmic' : 2.52, #(228,000 ms total exposure)/(6.4ms * 13100 evts generated) 
    #(this cosmic weight for if NOT using flash matching)
    'numu' : 6.6e20/(2.706e15*99600),
    'nc' : 6.6e20/(2.706e15*99600),
    'lee' : 1 #LEE weights to scale to 6.6e20 are fully contained in the output tree
}

xbins = array('d',[0.2, 0.3, 0.375, 0.475, 0.550, 0.675, 0.800, 0.950, 1.1, 1.25])

h_nue = TH1F("h_nue", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21, 0.2, 1.25)
h_cosmic = TH1F("h_cosmic", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21, 0.2, 1.25)
h_numu = TH1F("h_numu", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21, 0.2, 1.25)
h_nc = TH1F("h_nc", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21,  0.2, 1.25)
h_lee = TH1F("h_lee", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21, 0.2, 1.25)
h_cosmic_tag = TH1F("h_cosmic_tag", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21, 0.2, 1.25)
h_cosmic_tag_phaseA = TH1F("h_cosmic_tag", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21, 0.2, 1.25)

h_cosmic_ob = TH1F("h_cosmic_ob", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21, 0.2, 1.25)

h_bg = TH1F("h_bg", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21, 0.2, 1.25)
h_bg_notagger = TH1F("h_bg_notagger", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", 21, 0.2, 1.25)

h_total = THStack("h_total", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}")

h_pdg = TH1F("h_pdg","",60,-28,30)

histograms = {
    'nue' : h_nue,
    'numu' : h_numu,
    'nc' : h_nc,
    'cosmic' : h_cosmic,
    'lee' : h_lee
}

scalefactor = 1000. #from MeV to GeV

gStyle.SetOptStat(0)

legend = TLegend(0.53,0.61,0.88,0.85)
total_events = 0

for kind in filenames:
    f = TFile(filebase+filenames[kind])
    
    chain = gDirectory.Get(treenames[kind])
    entries = chain.GetEntriesFast()
    
    for entry in xrange(entries):
        ientry = chain.LoadTree(entry)
        nb = chain.GetEntry(entry)
        
        default_cut = chain._e_Edep > 50.
        tracklen_cut = chain._longestTrackLen < 100.
        fidvolcut = chain._x_vtx > 10 and chain._x_vtx < 246.35 and chain._y_vtx > -106.5 and chain._y_vtx < 106.5 and chain._z_vtx > 10 and chain._z_vtx < 1026.8;

        if default_cut and tracklen_cut :
            if (kind != 'cosmic'):
                histograms[kind].Fill(chain._e_nuReco/scalefactor,chain._weight)
            else:
                #if chain._in_tagger_phaseA:
                #    h_cosmic_tag_phaseA.Fill(chain._e_nuReco/scalefactor)
                #    continue
                #if chain._in_tagger:
                #    h_cosmic_tag.Fill(chain._e_nuReco/scalefactor)
                #    continue
                    
                if ob:
                    if chain._parentPDG == 22 and chain._in_ob:
                        h_cosmic_ob.Fill(chain._e_nuReco/scalefactor)
                        continue
                        
                histograms[kind].Fill(chain._e_nuReco/scalefactor)

        
    histograms[kind].Scale(scaling_weights[kind])    
    histograms[kind].SetFillColor(colors[kind])
    histograms[kind].SetLineColor(kBlack)
    if kind != 'fee':
        total_events += histograms[kind].Integral()
    print "{0:30} {1:7.2f} +- {2:5.2f}".format(labels[kind],histograms[kind].Integral(),math.sqrt(histograms[kind].Integral()))
    
    legend.AddEntry(histograms[kind],labels[kind],"f")
    f.Close()
    

h_cosmic_tag.Scale(scaling_weights['cosmic'])
h_cosmic_tag.SetLineColor(kBlack)
h_cosmic_tag.SetFillStyle(3002)
h_cosmic_tag.SetFillColor(colors['cosmic'])

h_cosmic_tag_phaseA.Scale(scaling_weights['cosmic'])
h_cosmic_tag_phaseA.SetLineColor(kBlack)
h_cosmic_tag_phaseA.SetFillStyle(3001)
h_cosmic_tag_phaseA.SetFillColor(colors['cosmic'])


h_cosmic_ob.Scale(scaling_weights['cosmic'])
h_cosmic_ob.SetLineColor(kBlack)
h_cosmic_ob.SetFillStyle(3001)
h_cosmic_ob.SetFillColor(colors['cosmic'])

# Tagger available only for 2 years
h_cosmic_tag_rescale = h_cosmic_tag.Clone()
h_cosmic_tag_rescale.SetName("h_cosmic_tag_rescale")
h_cosmic_tag_phaseA_rescale = h_cosmic_tag_phaseA.Clone()
h_cosmic_tag_phaseA_rescale.SetName("h_cosmic_tag_phaseA_rescale")
if scaled_2years:
    h_cosmic_tag_phaseA_rescale.Scale(4.1/6.6)
    h_cosmic_tag_rescale.Scale(2.1/6.6)
    for i in xrange(0, h_cosmic_tag.GetNbinsX()):
        h_cosmic.SetBinContent(i, h_cosmic.GetBinContent(i)+(h_cosmic_tag.GetBinContent(i)-h_cosmic_tag_rescale.GetBinContent(i)))
        h_cosmic.SetBinContent(i, h_cosmic.GetBinContent(i)+(h_cosmic_tag_phaseA.GetBinContent(i)-h_cosmic_tag_phaseA_rescale.GetBinContent(i)))
        

bg_histograms = [h_nue, h_numu, h_nc, h_cosmic, h_cosmic_tag_rescale, h_cosmic_tag_phaseA_rescale]

for h in bg_histograms:
    for i in xrange(0,h.GetNbinsX()):
        if h is not h_cosmic_tag_rescale and h is not h_cosmic_tag_phaseA_rescale:
            h_bg.SetBinContent(i,h_bg.GetBinContent(i)+h.GetBinContent(i))

        h_bg_notagger.SetBinContent(i,h_bg_notagger.GetBinContent(i)+h.GetBinContent(i))
        
#legend.AddEntry(h_cosmic_tag_phaseA,"Tagged cosmic rays - phase A","f")
legend.AddEntry(h_cosmic_tag,"Tagged cosmic rays - phase B","f")

if ob:
    legend.AddEntry(h_cosmic_ob,"Cosmic #gamma through the OB","f")

print "{0:30} {1:7.2f} +- {2:5.2f}".format("Cosmic rays - tagged",h_cosmic_tag.Integral(),math.sqrt(h_cosmic_tag.Integral()))
print "{0:30} {1:7.2f} +- {2:5.2f}".format("Cosmic gammas through the OB",h_cosmic_ob.Integral(),math.sqrt(h_cosmic_ob.Integral()))

print "Excess w/o tagger:", "{:.2f}".format(sigmaCalc(h_lee,h_bg_notagger)), "sigma"
print "Excess w/ tagger:", "{:.2f}".format(sigmaCalc(h_lee,h_bg)), "sigma"


h_total.Add(h_nue)
h_total.Add(h_numu)
h_total.Add(h_nc)
h_total.Add(h_cosmic)
h_total.Add(h_cosmic_ob)
h_total.Add(h_cosmic_tag_phaseA_rescale)

h_total.Add(h_cosmic_tag_rescale)


h_total.Add(h_lee)
h_lee.SetLineStyle(7)
legend.SetTextFont(42)
legend.SetShadowColor(0)
legend.SetBorderSize(0)
c = TCanvas("c")
h_total.SetMinimum(250)
h_total.Draw()
legend.Draw()
c.Update()


raw_input()