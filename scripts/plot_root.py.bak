#!/usr/bin/env python

import os
from ROOT import gStyle, gDirectory, TFile, TH1F, TCanvas, THStack, kBlack, kGreen, kWhite, kBlue, kRed, TLegend, kOrange, kPink, kGray
from array import array
import math

def sigmaCalc(h_signal, h_background):
    chi2 = sum([h_signal.GetBinContent(i)**2/h_background.GetBinContent(i) for i in xrange(0,h_signal.GetNbinsX())])
    return math.sqrt(chi2)

scaled_2years = True
ob = False
sbn = 1
BGWstart = 3.6
BGWend   = 5.2
filebase = '/Users/soleti/larlite/UserDev/LowEnergyExcess/output/'

filenames = {
    'nue':'singleE_nue_selection_mc.root',
    'numu':'singleE_numu_selection_mc.root',
    'nc':'singleE_nc_selection_mc.root',
    'bite':'bite_tagged.root',
    #'bite':'singleE_dirt_selection_mc.root',
#    'cosmicoutoftime':'cosmic_out.root',
    'cosmic':'tagger.root',
    'lee':'singleE_LEE_selection_mc.root',
    'ob_cosmic':'ob_tagged.root'
}


treenames = { 'nue' : 'beamNuE',
    'cosmic' : 'cosmicShowers',
    'cosmicoutoftime':'cosmicOutOfTime',
    'numu' : 'beamNuMu',
    'nc' : 'beamNC',
    'bite': 'dirt',
    'lee' : 'LEETree',
    'ob_cosmic':'cosmicShowers'
}
            
labels = { 'nue' : 'Beam Intrinsic #nu_{e}',
    'cosmicoutoftime':'Cosmics Out Of Time, #nu In Time',
    'cosmic' : 'Cosmics In Time',#'CRY Cosmic, in-time',
    'numu' : 'Beam Intrinsic #nu_{#mu}',
    'nc' : 'Beam Intrinsic NC', 
    'bite': 'B.I.T.E. (In Cryostat)',
    'lee' : 'Scaled Signal',
    'ob_cosmic':'Cosmics In Time'
}

colors = { 'nue' : kGreen-2,
    'numu' : kBlue-5,
    'nc' : kBlue-9,
    'cosmic' : kRed-3,
    'cosmicoutoftime' : kPink,
    'bite' : kGray,
    'lee' : kWhite,
    'ob_cosmic' : kRed-3
}



#These weights are to scale to 6.6e20 POT
scaling_weights = { 
'nue' : 6.6e20/(3.1845e17*19920),
 #(211,000 ms total exposure)/(7.2ms * 36600 evts generated)
  'cosmic' : 211000/(7.25*5346), #4749 with OB 5346 without
  #'cosmic' : (1.056e6)/(36600),
  'numu' : 6.6e20/((1.203e15)*20240),
  'nc' : 6.6e20/(1.203e15*20240),
  'bite' : 6.6e20/(1.203e15*9330), #9330
  'cosmicoutoftime': 6.6e20/(1.203e15*9960),
  'lee' : 1,
  'ob_cosmic' : 211000/(7.25*4749) #4749 with OB 5346 without
}

xbins = array('d',[0.2, 0.3, 0.375, 0.475, 0.550, 0.675, 0.800, 0.950, 1.1, 2])

start_energy = 0.1
bins = 19

h_nue = TH1F("h_nue", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_cosmic = TH1F("h_cosmic", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_cosmic_out = TH1F("h_cosmic_out", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)

h_numu = TH1F("h_numu", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_nc = TH1F("h_nc", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins,  start_energy, 2)
h_lee = TH1F("h_lee", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_bite_tag = TH1F("h_bite_tag", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_bite_tag_phaseA = TH1F("h_bite_tag_phaseA", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_cosmic_tag = TH1F("h_cosmic_tag", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_cosmic_tag_phaseA = TH1F("h_cosmic_tag_phaseA", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_bite = TH1F("h_bite", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_cosmic_ob_tag = TH1F("h_cosmic_ob_tag", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_cosmic_ob_tag_phaseA = TH1F("h_cosmic_ob_tag_phaseA", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_cosmic_ob = TH1F("h_cosmic_ob", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)

h_bg = TH1F("h_bg", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)
h_bg_notagger = TH1F("h_bg_notagger", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}", bins, start_energy, 2)

h_total = THStack("h_total", ";E_{#nu}^{QE} [GeV/c^{2}]; N. Events / 0.05 GeV/c^{2}")

h_pdg = TH1F("h_pdg","",60,-28,30)

histograms = {
    'nue' : h_nue,
    'numu' : h_numu,
    'nc' : h_nc,
    'cosmic' : h_cosmic,
    'cosmicoutoftime' : h_cosmic_out,
    'bite' : h_bite,
    'lee' : h_lee,
    'ob_cosmic' : h_cosmic_ob,
}

scalefactor = 1000. #from MeV to GeV

gStyle.SetOptStat(0)

legend = TLegend(0.43,0.41,0.78,0.85)
total_events = 0
tag_bite = True
tag_cosmic = True

for kind in filenames:
    f = TFile(filebase+filenames[kind])
    
    chain = gDirectory.Get(treenames[kind])
    entries = chain.GetEntriesFast()
    
    for entry in xrange(entries):
        ientry = chain.LoadTree(entry)
        nb = chain.GetEntry(entry)
        
        default_cut = chain._e_Edep > 50.
        fidvolcut = chain._x_vtx > 10 and chain._x_vtx < 246.35 and chain._y_vtx > -106.5 and chain._y_vtx < 106.5 and chain._z_vtx > 10 and chain._z_vtx < 1026.8;
        BGWcut = chain._flash_time > BGWstart and chain._flash_time < BGWend


        if kind == 'cosmic' or kind == 'ob_cosmic':
            chain._flash_time = (BGWstart+BGWend)/2.
        
        
        if default_cut and BGWcut:
            
            if kind == 'bite' and tag_bite:
                if chain.crt_phaseA:
                    h_bite_tag_phaseA.Fill(chain._e_nuReco/scalefactor, chain._weight)
                    continue
                if chain.crt_phaseB:
                    h_bite_tag.Fill(chain._e_nuReco/scalefactor, chain._weight)
                    continue

                histograms[kind].Fill(chain._e_nuReco/scalefactor, chain._weight)
                continue
        
            if kind == 'cosmicoutoftime':

                if chain._in_tagger_phaseA:
                    h_cosmic_tag_phaseA.Fill(chain._e_nuReco/scalefactor, chain._weight)
                    continue
                if chain._in_tagger:
                    h_cosmic_tag.Fill(chain._e_nuReco/scalefactor, chain._weight)
                    continue

                if  (chain._mc_time < 3100 or chain._mc_time > 4700) and chain._mc_origin == 2:
                    histograms[kind].Fill(chain._e_nuReco/scalefactor, chain._weight)

                continue
                
            if kind == 'cosmic' and tag_cosmic:

                if chain.crt_phaseA:
                    h_cosmic_tag_phaseA.Fill(chain._e_nuReco/scalefactor)
                    continue
                if chain.crt_phaseB:
                    h_cosmic_tag.Fill(chain._e_nuReco/scalefactor)
                    continue

                histograms[kind].Fill(chain._e_nuReco/scalefactor)
                continue
                
                
            if kind == 'ob_cosmic' and tag_cosmic:
                if chain.crt_phaseA:
                    h_cosmic_ob_tag_phaseA.Fill(chain._e_nuReco/scalefactor)
                    continue
                if chain.crt_phaseB:
                    h_cosmic_ob_tag.Fill(chain._e_nuReco/scalefactor)
                    continue

                histograms[kind].Fill(chain._e_nuReco/scalefactor)
                continue
                
            histograms[kind].Fill(chain._e_nuReco/scalefactor, chain._weight)
            

            #if kind != 'cosmic':
            #    histograms[kind].Fill(chain._e_nuReco/scalefactor,chain._weight)
            #else:
            #    

            #    histograms[kind].Fill(chain._e_nuReco/scalefactor)

            

        
    histograms[kind].Scale(scaling_weights[kind])    
    histograms[kind].SetFillColor(colors[kind])
    histograms[kind].SetLineColor(kBlack)
    if kind != 'fee':
        total_events += histograms[kind].Integral()

    if not 'cosmic' in kind and not 'bite' in kind and not 'lee' in kind:
        legend.AddEntry(histograms[kind],labels[kind]+" (%d events)"%histograms[kind].Integral(),"f")
    f.Close()
    
h_cosmic_tag.Scale(scaling_weights['cosmic'])
h_cosmic_tag.SetLineColor(kBlack)
h_cosmic_tag.SetFillStyle(3002)
h_cosmic_tag.SetFillColor(colors['cosmic'])

h_cosmic_tag_phaseA.Scale(scaling_weights['cosmic'])
h_cosmic_tag_phaseA.SetLineColor(kBlack)
h_cosmic_tag_phaseA.SetFillStyle(3001)
h_cosmic_tag_phaseA.SetFillColor(colors['cosmic'])

h_cosmic_ob_tag.Scale(scaling_weights['cosmic'])
h_cosmic_ob_tag.SetLineColor(kBlack)
h_cosmic_ob_tag.SetFillStyle(3002)
h_cosmic_ob_tag.SetFillColor(colors['cosmic'])

h_cosmic_ob_tag_phaseA.Scale(scaling_weights['cosmic'])
h_cosmic_ob_tag_phaseA.SetLineColor(kBlack)
h_cosmic_ob_tag_phaseA.SetFillStyle(3001)
h_cosmic_ob_tag_phaseA.SetFillColor(colors['cosmic'])

h_bite_tag.Scale(scaling_weights['bite'])
h_bite_tag.SetLineColor(kBlack)
h_bite_tag.SetFillStyle(3002)
h_bite_tag.SetFillColor(colors['bite'])

h_bite_tag_phaseA.Scale(scaling_weights['bite'])
h_bite_tag_phaseA.SetLineColor(kBlack)
h_bite_tag_phaseA.SetFillStyle(3001)
h_bite_tag_phaseA.SetFillColor(colors['bite'])



# Tagger available only for 2 years
h_cosmic_tag_rescale = h_cosmic_tag.Clone()
h_cosmic_tag_rescale.SetName("h_cosmic_tag_rescale")

h_cosmic_ob_tag_rescale = h_cosmic_ob_tag.Clone()
h_cosmic_ob_tag_rescale.SetName("h_cosmic_ob_tag_rescale")

h_cosmic_ob_tag_phaseA_rescale = h_cosmic_ob_tag_phaseA.Clone()
h_cosmic_ob_tag_phaseA_rescale.SetName("h_cosmic_ob_tag_phaseA_rescale")

h_cosmic_tag_phaseA_rescale = h_cosmic_tag_phaseA.Clone()
h_cosmic_tag_phaseA_rescale.SetName("h_cosmic_tag_phaseA_rescale")

h_bite_tag_rescale = h_bite_tag.Clone()
h_bite_tag_rescale.SetName("h_bite_tag_rescale")

h_bite_tag_phaseA_rescale = h_bite_tag_phaseA.Clone()
h_bite_tag_phaseA_rescale.SetName("h_bite_tag_phaseA_rescale")

if scaled_2years:
    if sbn == 2:
        h_cosmic_tag_phaseA_rescale.Scale(10.7/13.2)
        h_cosmic_tag_rescale.Scale(8.7/13.2)
        h_bite_tag_phaseA_rescale.Scale(10.7/13.2)
        h_bite_tag_rescale.Scale(8.7/13.2)
    else:
        h_bite_tag_phaseA_rescale.Scale(4.1/6.6)
        h_bite_tag_rescale.Scale(2.1/6.6)
        if ob:
            h_cosmic.Scale(2.5/6.6)
            h_cosmic_ob.Scale(4.1/6.6)
            h_cosmic_ob_tag_phaseA_rescale.Scale(4.1/6.6)
            h_cosmic_ob_tag_rescale.Scale(2.1/6.6)
        else:
            h_cosmic_tag_phaseA_rescale.Scale(4.1/6.6)
            h_cosmic_tag_rescale.Scale(2.1/6.6)
            
    
    for i in xrange(0, h_cosmic_tag.GetNbinsX()):
        h_bite.SetBinContent(i, h_bite.GetBinContent(i)+(h_bite_tag.GetBinContent(i)-h_bite_tag_rescale.GetBinContent(i)))
        h_bite.SetBinContent(i, h_bite.GetBinContent(i)+(h_bite_tag_phaseA.GetBinContent(i)-h_bite_tag_phaseA_rescale.GetBinContent(i)))
        
        if ob:
            h_cosmic_ob.SetBinContent(i, h_cosmic_ob.GetBinContent(i)+(h_cosmic_ob_tag.GetBinContent(i)-h_cosmic_ob_tag_rescale.GetBinContent(i)))
            h_cosmic_ob.SetBinContent(i, h_cosmic_ob.GetBinContent(i)+(h_cosmic_ob_tag_phaseA.GetBinContent(i)-h_cosmic_ob_tag_phaseA_rescale.GetBinContent(i)))
            h_cosmic_ob.SetBinContent(i, h_cosmic_ob.GetBinContent(i)+h_cosmic.GetBinContent(i))        
        
        h_cosmic.SetBinContent(i, h_cosmic.GetBinContent(i)+(h_cosmic_tag.GetBinContent(i)-h_cosmic_tag_rescale.GetBinContent(i)))
        h_cosmic.SetBinContent(i, h_cosmic.GetBinContent(i)+(h_cosmic_tag_phaseA.GetBinContent(i)-h_cosmic_tag_phaseA_rescale.GetBinContent(i)))
        



bg_histograms = [h_nue, h_numu, h_nc, h_cosmic, h_bite, h_cosmic_out, h_cosmic_tag_rescale, h_cosmic_tag_phaseA_rescale, h_bite_tag_rescale, h_bite_tag_phaseA_rescale]

for h in bg_histograms:
    for i in xrange(0,h.GetNbinsX()):
        if h is not h_cosmic_tag_rescale and h is not h_cosmic_tag_phaseA_rescale and h is not h_bite_tag_rescale and h is not h_bite_tag_phaseA_rescale:
            h_bg.SetBinContent(i,h_bg.GetBinContent(i)+h.GetBinContent(i))

        h_bg_notagger.SetBinContent(i,h_bg_notagger.GetBinContent(i)+h.GetBinContent(i))

if ob:
    legend.AddEntry(h_cosmic,"Cosmic rays (%d events)" % h_cosmic_ob.Integral(),"f")
else:
    legend.AddEntry(h_cosmic,"Cosmic rays (%d events)" % h_cosmic.Integral(),"f")


if tag_cosmic:
    if ob:
        legend.AddEntry(h_cosmic_ob_tag_phaseA,"Tagged cosmic rays - phase A (%d events)" % h_cosmic_ob_tag_phaseA_rescale.Integral(),"f")
        legend.AddEntry(h_cosmic_ob_tag,"Tagged cosmic rays - phase B (%d events)" % h_cosmic_ob_tag_rescale.Integral(),"f")
    else:
        legend.AddEntry(h_cosmic_tag_phaseA,"Tagged cosmic rays - phase A (%d events)" % h_cosmic_tag_phaseA_rescale.Integral(),"f")
        legend.AddEntry(h_cosmic_tag,"Tagged cosmic rays - phase B (%d events)" % h_cosmic_tag_rescale.Integral(),"f")

legend.AddEntry(h_bite,"B.I.T.E. (%d events)" % h_bite.Integral(),"f")

if tag_bite:
    legend.AddEntry(h_bite_tag_phaseA,"Tagged B.I.T.E. - phase A (%d events)" % h_bite_tag_phaseA_rescale.Integral(),"f")
    legend.AddEntry(h_bite_tag,"Tagged B.I.T.E. - phase B (%d events)" % h_bite_tag_rescale.Integral(),"f")
    
legend.AddEntry(h_lee,"Low Energy Excess (%d events)" % h_lee.Integral(),"f")


print "{0:30} {1:7.2f} +- {2:5.2f}".format("Cosmic rays",h_cosmic.Integral(),math.sqrt(h_cosmic.Integral()))
print "{0:30} {1:7.2f} +- {2:5.2f}".format("Cosmic rays - tagged",h_cosmic_tag_rescale.Integral()+h_cosmic_tag_phaseA_rescale.Integral(),math.sqrt(h_cosmic_tag.Integral()))

print "Excess w/o tagger:", "{:.2f}".format(sigmaCalc(h_lee,h_bg_notagger)), "sigma"
print "Excess w/ tagger:", "{:.2f}".format(sigmaCalc(h_lee,h_bg)), "sigma"


h_total.Add(h_nue)
h_total.Add(h_numu)
h_total.Add(h_nc)
h_total.Add(h_bite)

h_total.Add(h_bite_tag_phaseA_rescale)
h_total.Add(h_bite_tag_rescale)

if ob:
    h_total.Add(h_cosmic_ob)
    h_total.Add(h_cosmic_ob_tag_phaseA_rescale)
    h_total.Add(h_cosmic_ob_tag_rescale)
else:
    h_total.Add(h_cosmic)
    h_total.Add(h_cosmic_tag_phaseA_rescale)
    h_total.Add(h_cosmic_tag_rescale)

h_total.Add(h_cosmic_out)




h_total.Add(h_lee)
h_lee.SetLineStyle(7)
legend.SetTextFont(42)
legend.SetShadowColor(0)
legend.SetBorderSize(0)
c = TCanvas("c")
h_total.Draw()

legend.Draw()
c.Update()


raw_input()