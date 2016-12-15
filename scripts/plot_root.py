#!/usr/bin/env python

import os
from ROOT import kWhite,TObject, TPad, gStyle, gDirectory, TFile, TH1F, TCanvas, THStack, kBlack, kGreen, kWhite, kBlue, kRed, TLegend, kOrange, kPink, kGray, TLine, TArrow, TPaveText
from array import array
import math

def sigmaCalc(h_signal, h_background):
    try:
        chi2 = sum([h_signal.GetBinContent(i)**2/h_background.GetBinContent(i) for i in range(2,h_signal.GetNbinsX())])
        return math.sqrt(chi2)
    except:
        print 'Error'
        return 0

algo = True
scaled_2years = False
ob = False
sbn = 2e20/6.6e20
BGWstart = 3.6
BGWend   = 5.2
tag_bite = True
tag_cosmic = True

print("SBN", sbn)
print("Overburden", ob)
print("Cosmic algo", algo)
print("Tag BITE", tag_bite)
print("Tag cosmic", tag_cosmic)

filebase = '/Users/soleti/larlite/UserDev/LowEnergyExcess/output/'

if algo:
    filenames = {
        'nue':'singleE_nue_selection_mc.root',
        'numu':'numu_all.root',#'singleE_numu_selection_mc.root',
        'nc':'nc_all.root',#'singleE_nc_selection_mc.root',
        'bite':'bite.root',
        'cosmicoutoftime':'outoftime_all.root',
        'cosmic':'cosmic.root',
        'lee':'singleE_LEE_selection_mc.root',
        'ob_cosmic':'ob_cosmic.root',
        'ob_cosmicoutoftime':'ob_cosmicoutoftime.root'
    }
else:
    filenames = {
        'nue':'singleE_nue_selection_mc.root',
        'numu':'numu_all.root',#'singleE_numu_selection_mc.root',
        'nc':'nc_all.root',#'singleE_nc_selection_mc.root',
        'bite':'bite.root',
        'cosmicoutoftime':'outoftime_noalgo.root',
        'cosmic':'cosmic_noalgo.root',
        'lee':'singleE_LEE_selection_mc.root',
        'ob_cosmic':'ob_cosmic_noalgo.root',
        'ob_cosmicoutoftime':'ob_cosmicoutoftime_noalgo.root'
    }


treenames = { 'nue' : 'beamNuE',
    'cosmic' : 'cosmicShowers',
    'cosmicoutoftime':'cosmicOutOfTime',
    'numu' : 'beamNuMu',
    'nc' : 'beamNC',
    'bite': 'dirt',
    'lee' : 'LEETree',
    'ob_cosmic':'cosmicShowers',
    'ob_cosmicoutoftime':'cosmicOutOfTime'
    
}
            
labels = { 'nue' : 'Beam Intrinsic #nu_{e}',
    'cosmicoutoftime':'Cosmics Out Of Time',
    'cosmic' : 'Cosmics In Time',#'CRY Cosmic, in-time',
    'numu' : 'Beam Intrinsic #nu_{#mu}',
    'nc' : 'Beam Intrinsic NC', 
    'bite': 'B.I.T.E. (In Cryostat)',
    'lee' : 'Scaled Signal',
    'ob_cosmic':'Cosmics In Time',
    'ob_cosmicoutoftime':'Cosmics Out Of Time, #nu In Time',
    
}

colors = { 'nue' : kGreen-2,
    'numu' : kBlue-5,
    'nc' : kBlue-9,
    'cosmic' : kRed-3,
    'cosmicoutoftime' : kPink,
    'bite' : kGray,
    'lee' : kWhite,
    'ob_cosmic' : kRed-3,
    'ob_cosmicoutoftime' : kPink
    
}



#These weights are to scale to 6.6e20 POT
if algo:
    scaling_weights = { 
        'nue' : sbn*6.6e20/(3.1845e17*19920),
        #(211,000 ms total exposure)/(7.2ms * 36600 evts generated)
        'cosmic' : sbn*211000/(7.25*55584), #55584 full sample, 38803 noalgo
        #'cosmic' : (1.056e6)/(36600),
        'numu' : sbn*6.6e20/((1.203e15)*93004), 
        'nc' : sbn*6.6e20/(1.203e15*93004), 
        'bite' : sbn*6.6e20/(1.203e15*93004), 
        'cosmicoutoftime': sbn*6.6e20/(1.203e15*167089), #167089
        'lee' : sbn*1,
        'ob_cosmic' : sbn*211000/(7.25*49492), #49492 full sample #36782 no algo
        'ob_cosmicoutoftime': sbn*6.6e20/(1.203e15*167089*6436/9857), #167089
        
    }
else:
    scaling_weights = { 
      'nue' : sbn*6.6e20/(3.1845e17*19920),
     #(211,000 ms total exposure)/(7.2ms * 36600 evts generated)
      'cosmic' : sbn*211000/(7.25*38803), #55584 full sample, 38803 noalgo
      #'cosmic' : (1.056e6)/(36600),
      'numu' : sbn*6.6e20/((1.203e15)*93004), 
      'nc' : sbn*6.6e20/(1.203e15*93004), 
      'bite' : sbn*6.6e20/(1.203e15*93004), 
      'cosmicoutoftime': sbn*6.6e20/(1.203e15*167089*3583/9857), #167089
      'lee' : sbn*1,
      'ob_cosmic' : sbn*211000/(7.25*36782), #49492 full sample #36782 no algo
      'ob_cosmicoutoftime': sbn*6.6e20/(1.203e15*167089*6436/9857), #167089
      
    }

xbins = array('d',[0.05,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2])

start_energy = 0.05
bins = len(xbins)-1

h_nue = TH1F("h_nue", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_cosmic = TH1F("h_cosmic", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_cosmic_out = TH1F("h_cosmic_out", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_cosmic_out_ob = TH1F("h_cosmic_out_ob", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)

h_numu = TH1F("h_numu", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_nc = TH1F("h_nc", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins,  xbins)
h_lee = TH1F("h_lee", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_bite_tag = TH1F("h_bite_tag", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_bite_tag_phaseA = TH1F("h_bite_tag_phaseA", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_cosmic_tag = TH1F("h_cosmic_tag", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_cosmic_tag_phaseA = TH1F("h_cosmic_tag_phaseA", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_bite = TH1F("h_bite", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_cosmic_ob_tag = TH1F("h_cosmic_ob_tag", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_cosmic_ob_tag_phaseA = TH1F("h_cosmic_ob_tag_phaseA", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_cosmic_ob = TH1F("h_cosmic_ob", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)

h_bg = TH1F("h_bg", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)
h_bg_notagger = TH1F("h_bg_notagger", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}", bins, xbins)

h_total = THStack("h_total", ";Reconstructed #nu energy [GeV/c^{2}]; N. Events / 0.1 GeV/c^{2}")

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
    'ob_cosmicoutoftime' : h_cosmic_out_ob
}

scalefactor = 1000. #from MeV to GeV

gStyle.SetOptStat(0)

legend = TLegend(0.37,0.32,0.71,0.71)

total_events = 0

for kind in filenames:
    f = TFile(filebase+filenames[kind])
    
    chain = gDirectory.Get(treenames[kind])
    entries = chain.GetEntriesFast()
    
    for entry in range(entries):
        ientry = chain.LoadTree(entry)
        nb = chain.GetEntry(entry)
        
        default_cut = chain._e_Edep > 60.
        fidvolcut = chain._x_vtx > 10 and chain._x_vtx < 246 and chain._y_vtx > -106.5 and chain._y_vtx < 106.5 and chain._z_vtx > 10 and chain._z_vtx < 1026.8;
        BGWcut = chain._flash_time > BGWstart and chain._flash_time < BGWend


        if kind == 'cosmic' or kind == 'ob_cosmic':
            chain._flash_time = (BGWstart+BGWend)/2.

        if default_cut and fidvolcut and BGWcut:

            if kind == 'bite' and tag_bite:
                if chain.crt_phaseA:
                    h_bite_tag_phaseA.Fill(chain._e_nuReco_better/scalefactor, chain._weight)
                    continue
                if chain.crt_phaseB:
                    h_bite_tag.Fill(chain._e_nuReco_better/scalefactor, chain._weight)
                    continue

                histograms[kind].Fill(chain._e_nuReco_better/scalefactor, chain._weight) 
                continue
        
            if kind == 'cosmicoutoftime' or kind == 'ob_cosmicoutoftime':
                
                if  (chain._mc_time < 3100 or chain._mc_time > 4700) and chain._mc_origin == 0:
                    histograms[kind].Fill(chain._e_nuReco_better/scalefactor, chain._weight)

                continue
                
            if kind == 'cosmic' and tag_cosmic:
                if chain.crt_phaseA:
                    h_cosmic_tag_phaseA.Fill(chain._e_nuReco_better/scalefactor)
                    continue
                if chain.crt_phaseB:
                    h_cosmic.Fill(chain._e_nuReco_better/scalefactor)
                    continue

                histograms[kind].Fill(chain._e_nuReco_better/scalefactor)
                continue
                
                
            if kind == 'ob_cosmic' and tag_cosmic:
                if chain.crt_phaseA:
                    h_cosmic_ob_tag_phaseA.Fill(chain._e_nuReco_better/scalefactor)
                    continue
                if chain.crt_phaseB:
                    h_cosmic_ob_tag.Fill(chain._e_nuReco_better/scalefactor)
                    continue

                histograms[kind].Fill(chain._e_nuReco_better/scalefactor)
                continue
                
                
            histograms[kind].Fill(chain._e_nuReco_better/scalefactor, chain._weight)

        
    histograms[kind].Scale(scaling_weights[kind])    
    histograms[kind].SetFillColor(colors[kind])
    histograms[kind].SetLineColor(kBlack)
    if kind != 'fee':
        total_events += histograms[kind].Integral()

    if 'cosmic' not in kind and 'bite' != kind and 'lee' != kind:
        legend.AddEntry(histograms[kind],labels[kind]+" (%d events, scale factor %.1f)" % (histograms[kind].Integral(2,bins),scaling_weights[kind]),"f")
        
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

        h_bite_tag_phaseA_rescale.Scale(10.7/13.2)
        h_bite_tag_rescale.Scale(8.7/13.2)
        if ob:
            h_cosmic.Scale(2.5/13.2)
            h_cosmic_tag_phaseA.Scale(2.5/13.2)
            h_cosmic_tag.Scale(2.1/13.2)
            h_cosmic_ob.Scale(10.7/13.2)
            h_cosmic_ob_tag_phaseA_rescale.Scale(10.7/13.2)
            h_cosmic_ob_tag_rescale.Scale(2.1/13.2)
            
            h_cosmic_out.Scale(2.5/13.2)
            h_cosmic_out_ob.Scale(10.7/13.2)
            
        else:
            h_cosmic_tag_phaseA_rescale.Scale(10.7/13.2)
            h_cosmic_tag_rescale.Scale(0/13.2)
        
    else:
        h_bite_tag_phaseA_rescale.Scale(4.1/6.6)
        h_bite_tag_rescale.Scale(2.1/6.6)
        if ob:
            h_cosmic.Scale(2.5/6.6)
            h_cosmic_tag_phaseA.Scale(2.5/6.6)
            h_cosmic_tag.Scale(2.5/6.6)
            h_cosmic_ob.Scale(4.1/6.6)
            h_cosmic_ob_tag_phaseA_rescale.Scale(4.1/6.6)
            h_cosmic_ob_tag_rescale.Scale(2.5/6.6)

            h_cosmic_out.Scale(2.5/6.6)
            h_cosmic_out_ob.Scale(4.1/6.6)

        else:
            h_cosmic_tag_phaseA_rescale.Scale(4.1/6.6)
            h_cosmic_tag_rescale.Scale(2.1/6.6)
            
    
    for i in range(0, h_cosmic_tag.GetNbinsX()):
        h_bite.SetBinContent(i, h_bite.GetBinContent(i)+(h_bite_tag.GetBinContent(i)-h_bite_tag_rescale.GetBinContent(i)))
        h_bite.SetBinContent(i, h_bite.GetBinContent(i)+(h_bite_tag_phaseA.GetBinContent(i)-h_bite_tag_phaseA_rescale.GetBinContent(i)))
        
        if ob:
            h_cosmic_ob.SetBinContent(i, h_cosmic_ob.GetBinContent(i)+(h_cosmic_ob_tag.GetBinContent(i)-h_cosmic_ob_tag_rescale.GetBinContent(i)))
            h_cosmic_ob.SetBinContent(i, h_cosmic_ob.GetBinContent(i)+(h_cosmic_ob_tag_phaseA.GetBinContent(i)-h_cosmic_ob_tag_phaseA_rescale.GetBinContent(i)))
            h_cosmic_ob.SetBinContent(i, h_cosmic_ob.GetBinContent(i)+h_cosmic.GetBinContent(i)) 
            h_cosmic_ob.SetBinContent(i, h_cosmic_ob.GetBinContent(i)+h_cosmic_tag_phaseA.GetBinContent(i))        
            h_cosmic_ob.SetBinContent(i, h_cosmic_ob.GetBinContent(i)+h_cosmic_tag.GetBinContent(i))
            
            h_cosmic_out_ob.SetBinContent(i, h_cosmic_out_ob.GetBinContent(i)+h_cosmic_out.GetBinContent(i))        
            
                   
        
        h_cosmic.SetBinContent(i, h_cosmic.GetBinContent(i)+(h_cosmic_tag.GetBinContent(i)-h_cosmic_tag_rescale.GetBinContent(i)))
        h_cosmic.SetBinContent(i, h_cosmic.GetBinContent(i)+(h_cosmic_tag_phaseA.GetBinContent(i)-h_cosmic_tag_phaseA_rescale.GetBinContent(i)))
        

for kind in histograms:
    histograms[kind].SetBinContent(1, histograms[kind].GetBinContent(1)*2)

if ob:
    bg_histograms = [h_cosmic_out_ob,h_nue, h_numu, h_nc, h_cosmic_ob, h_bite, h_cosmic_ob_tag_phaseA_rescale, h_cosmic_ob_tag_rescale, h_bite_tag_rescale, h_bite_tag_phaseA_rescale]
    tag_histo = [h_bite_tag_rescale, h_bite_tag_phaseA_rescale, h_cosmic_ob_tag_phaseA_rescale, h_cosmic_ob_tag_rescale]
else:
    bg_histograms = [h_cosmic_out,h_nue, h_numu, h_nc, h_cosmic, h_bite, h_cosmic_tag_rescale, h_cosmic_tag_phaseA_rescale, h_bite_tag_rescale, h_bite_tag_phaseA_rescale]
    tag_histo = [h_cosmic_tag_rescale, h_cosmic_tag_phaseA_rescale, h_bite_tag_rescale, h_bite_tag_phaseA_rescale]
    
for h in bg_histograms:
    for i in range(0,h.GetNbinsX()+1):
        if h not in tag_histo:
            h_bg.SetBinContent(i,h_bg.GetBinContent(i)+h.GetBinContent(i))

        h_bg_notagger.SetBinContent(i,h_bg_notagger.GetBinContent(i)+h.GetBinContent(i))

if not ob:
    legend.AddEntry(h_cosmic,"Cosmics In Time (%d events, scale factor %.1f)" % (h_cosmic.Integral(2,bins),scaling_weights["cosmic"]),"f")
    legend.AddEntry(h_cosmic_out_ob,"Cosmics Out Of Time (%d events, scale factor %.1f)" % (h_cosmic_out.Integral(2,bins),scaling_weights["cosmicoutoftime"]),"f")
    
else:
    legend.AddEntry(h_cosmic,"Cosmics In Time (%d events, scale factor %.1f)" % (h_cosmic_ob.Integral(2,bins),scaling_weights["ob_cosmic"]),"f")
    legend.AddEntry(h_cosmic_out,"Cosmics Out Of Time (%d events, scale factor %.1f)" % (h_cosmic_out_ob.Integral(2,bins),scaling_weights["ob_cosmicoutoftime"]),"f")
    
print 
#if tag_cosmic:
#    if ob:
#        legend.AddEntry(h_cosmic_ob_tag_phaseA,"Tagged cosmic rays - phase A (%d events)" % h_cosmic_ob_tag_phaseA_rescale.Integral(),"f")
#        legend.AddEntry(h_cosmic_ob_tag,"Tagged cosmic rays - phase B (%d events)" % h_cosmic_ob_tag_rescale.Integral(),"f")
#    else:
#        legend.AddEntry(h_cosmic_tag_phaseA,"Tagged cosmic rays - phase A (%d events)" % h_cosmic_tag_phaseA_rescale.Integral(),"f")
#        legend.AddEntry(h_cosmic_tag,"Tagged cosmic rays - phase B (%d events)" % h_cosmic_tag_rescale.Integral(),"f")

legend.AddEntry(h_bite,"B.I.T.E. (%d events, scale factor %.1f)" % (h_bite.Integral(2,bins), scaling_weights["bite"]),"f")

#if tag_bite:
#    legend.AddEntry(h_bite_tag_phaseA,"Tagged B.I.T.E. - phase A (%d events)" % h_bite_tag_phaseA_rescale.Integral(),"f")
#    legend.AddEntry(h_bite_tag,"Tagged B.I.T.E. - phase B (%d events)" % h_bite_tag_rescale.Integral(),"f")
    
legend.AddEntry(h_lee,"Low Energy Excess (%d events)" % h_lee.Integral(),"f")

print("Excess w/o tagger:", "{:.2f}".format(sigmaCalc(h_lee,h_bg_notagger)), "sigma")
print("Excess w/ tagger:", "{:.2f}".format(sigmaCalc(h_lee,h_bg)), "sigma")

#print(h_nue.Integral()+h_numu.Integral()+h_nc.Integral()+h_bite.Integral()+h_bite_tag_phaseA_rescale.Integral()+h_bite_tag_rescale.Integral())
#print(h_cosmic_ob.Integral())
#print(h_cosmic.Integral())

h_total.Add(h_nue)
h_total.Add(h_numu)
h_total.Add(h_nc)
h_total.Add(h_bite)

#h_total.Add(h_bite_tag_phaseA_rescale)
#h_total.Add(h_bite_tag_rescale)


#h_total.Add(h_cosmic_out)


if ob:
    h_total.Add(h_cosmic_out_ob)
    
    h_total.Add(h_cosmic_ob)
 
    #h_total.Add(h_cosmic_ob_tag_phaseA_rescale)
    #h_total.Add(h_cosmic_ob_tag_rescale)
else:
    h_total.Add(h_cosmic_out)
    
    h_total.Add(h_cosmic)
    
    #h_total.Add(h_cosmic_tag_phaseA_rescale)
    #h_total.Add(h_cosmic_tag_rescale)

h_total.Add(h_lee)
h_lee.SetLineStyle(7)
legend.SetTextFont(42)
legend.SetShadowColor(0)
legend.SetBorderSize(0)


if not tag_bite:
    h_bite.SaveAs("/Users/soleti/larlite/UserDev/LowEnergyExcess/scripts/h_bite.root")
if not tag_cosmic and not ob:
    h_cosmic.SaveAs("/Users/soleti/larlite/UserDev/LowEnergyExcess/scripts/h_cosmic.root")
if not ob:
    h_cosmic_out.SaveAs("/Users/soleti/larlite/UserDev/LowEnergyExcess/scripts/h_cosmic_out.root")
        
if not tag_bite and not tag_cosmic and not ob:
    h_bg_notagger.SaveAs("/Users/soleti/larlite/UserDev/LowEnergyExcess/scripts/bg_total.root")
    

f_notag_noob = TFile("/Users/soleti/larlite/UserDev/LowEnergyExcess/scripts/bg_total.root"); 
f_cosmic_notag_noob = TFile("/Users/soleti/larlite/UserDev/LowEnergyExcess/scripts/h_cosmic.root")
f_bite_notag_noob = TFile("/Users/soleti/larlite/UserDev/LowEnergyExcess/scripts/h_bite.root")
f_cosmic_out_notag_noob = TFile("/Users/soleti/larlite/UserDev/LowEnergyExcess/scripts/h_cosmic_out.root")

h_notag_noob = f_notag_noob.Get("h_bg_notagger")
h_cosmic_notagnoob = f_cosmic_notag_noob.Get("h_cosmic")
h_bite_notag_noob = f_bite_notag_noob.Get("h_bite")
h_cosmic_out_notag_noob = f_cosmic_out_notag_noob.Get("h_cosmic_out")

if ob:
    h_cosmic_ratio = h_cosmic_ob.Clone()
    h_cosmic_out_ratio = h_cosmic_out_ob.Clone()
else:
    h_cosmic_ratio = h_cosmic.Clone()
    h_cosmic_out_ratio = h_cosmic_out.Clone()
    

h_cosmic_ratio.SetName("h_cosmic_ratio")
h_cosmic_ratio.Sumw2()
h_cosmic_ratio.Divide(h_cosmic_notagnoob)

h_cosmic_out_ratio.SetName("h_cosmic_out_ratio")
h_cosmic_out_ratio.Sumw2()
h_cosmic_out_ratio.Divide(h_cosmic_out_notag_noob)

h_bite_ratio = h_bite.Clone()
h_bite_ratio.SetName("h_bite_ratio")
h_bite_ratio.Sumw2()
h_bite_ratio.Divide(h_bite_notag_noob)

h_ratio = h_bg.Clone()
h_ratio.SetName("h_ratio")
h_ratio.Sumw2()
h_ratio.Divide(h_notag_noob)


for i in xrange(h_ratio.GetNbinsX()):
    
    if h_cosmic_out_ratio.GetBinContent(i):
        h_cosmic_out_ratio.SetBinContent(i,(1-h_cosmic_out_ratio.GetBinContent(i))*100)
        #h_cosmic_out_ratio.SetBinError(i,0)
        
        h_cosmic_out_ratio.SetBinError(i,h_cosmic_out_ratio.GetBinError(i)*20)
    
    if h_cosmic_ratio.GetBinContent(i):
        print (1-h_cosmic_ratio.GetBinContent(i))*100
    
        h_cosmic_ratio.SetBinContent(i,(1-h_cosmic_ratio.GetBinContent(i))*100)
        #h_cosmic_ratio.SetBinError(i,0)
        
        h_cosmic_ratio.SetBinError(i,h_cosmic_ratio.GetBinError(i)*20)
    
    if h_bite_ratio.GetBinContent(i):
        h_bite_ratio.SetBinContent(i,(1-h_bite_ratio.GetBinContent(i))*100)
        #h_bite_ratio.SetBinError(i,0)
        
        h_bite_ratio.SetBinError(i,h_bite_ratio.GetBinError(i)*20)
        
    if h_ratio.GetBinContent(i):
        h_ratio.SetBinContent(i,(1-h_ratio.GetBinContent(i))*100)
        #h_ratio.SetBinError(i,0)
        
        h_ratio.SetBinError(i,h_ratio.GetBinError(i)*20)
        
    

h_ratio.GetYaxis().SetTitle("Bkg. decrease [%] ")



c = TCanvas("c","",800,800)
c.cd()    
pad_top = TPad("pad_top","",0, 0.35,1,1)
pad_top.SetBottomMargin(0)
pad_top.Draw()
pad_top.cd()
h_total.Draw("hist")
h_total.GetYaxis().SetRangeUser(1,460)
#h_total.GetYaxis().SetLabelSize(16)
#h_total.GetYaxis().SetLabelFont(43)
#h_total.GetYaxis().SetLabelSize(14)
h_bg.SetLineColor(kBlack)
h_bg.Draw("esame")
l = TLine(0.1,0,0.1,h_bg_notagger.GetMaximum()*1.1)
l.SetLineWidth(3)
legend.SetTextFont(43)
legend.SetTextSize(13)
l.Draw()
a = TArrow(0.097,h_bg_notagger.GetMaximum()*1.1,0.3,h_bg_notagger.GetMaximum()*1.1)
a.SetLineWidth(3)
a.SetArrowSize(0.04)
a.Draw()
legend.Draw()

t_tick = TPaveText(0.0616,0.0327,0.128,0.0978,"brNDC")
t_tick.SetShadowColor(0)
t_tick.SetFillColor(kWhite)
t_tick.SetTextAlign(11)

t_tick.SetBorderSize(0)
t_tick.SetTextFont(42)
t_tick.AddText("0.05")
t = TPaveText(0.37,0.72,0.65,0.82,"brNDC")
t.AddText("#scale[2]{MicroBooNE Preliminary}")
#t.AddText("#scale[2]{2e20 POT}")

if sbn == 2:
    t.AddText("#scale[2]{1.3e21 POT}")
if sbn == 1:
    t.AddText("#scale[2]{6.6e20 POT}")
    
t.SetShadowColor(0)
t.SetFillColor(0)
t.SetTextAlign(11)

t.SetBorderSize(0)
t.Draw()


c.cd()

pad_bottom = TPad("pad_bottom","",0, 0, 1, 0.35)
pad_bottom.SetTopMargin(0)
pad_bottom.SetBottomMargin(0.20)
pad_bottom.Draw()
pad_bottom.cd()
h_ratio.Draw("hist")
h_ratio.SetMarkerStyle(20)
h_ratio.SetLineColor(kBlack)
h_ratio.SetFillColor(0)
h_ratio.GetYaxis().SetTitleOffset(0.45)
h_ratio.GetYaxis().SetRangeUser(0,110)
h_cosmic_ratio.Draw("epsame")
h_cosmic_ratio.SetFillColor(0)
h_cosmic_ratio.SetMarkerStyle(20)

h_cosmic_ratio.SetLineColor(kBlack)
h_cosmic_ratio.SetMarkerColor(colors['cosmic'])
if ob:
    h_cosmic_out_ratio.Draw("epsame")
    h_cosmic_out_ratio.SetFillColor(0)
    h_cosmic_out_ratio.SetMarkerStyle(21)

h_cosmic_out_ratio.SetLineColor(kBlack)
h_cosmic_out_ratio.SetMarkerColor(colors['cosmicoutoftime'])

h_bite_ratio.Draw("epsame")
h_bite_ratio.SetFillColor(0)
h_bite_ratio.SetMarkerStyle(22)

h_bite_ratio.SetFillColor(colors['bite'])
h_bite_ratio.SetMarkerColor(colors['bite'])

legend2 = TLegend(0.48,0.77,0.88,0.95)
legend2.SetNColumns(2)
none = TObject()
legend2.SetTextFont(43)
legend2.SetTextSize(13)

legend2.SetShadowColor(0)
legend2.SetBorderSize(0)
legend2.AddEntry(h_cosmic_ratio, "Cosmics In Time", "lep")
if ob:
    legend2.AddEntry(h_cosmic_out_ratio, "Cosmics Out Of Time", "lep")
legend2.AddEntry(h_bite_ratio, "B.I.T.E.", "lep")
legend2.AddEntry(h_ratio, "Total", "l")
legend2.Draw()

t2 = TPaveText(0.12,0.84,0.44,0.94,"brNDC")
t2.AddText("Uncertainties reduced by a factor of 5")
t2.Draw()
t2.SetShadowColor(0)
#t2.SetTextFont(43)
t2.SetFillColor(0)
t2.SetTextAlign(11)

t2.SetBorderSize(0)
c.Update()


c_total = TCanvas("c_total")
h_total.Draw()
t.Draw()
h_bg.Draw("esame")
c_total.SetBottomMargin(0.11)
t_tick.Draw()
l.Draw()
a.Draw()
#legend.Draw()
c_total.Update()
c_total.SaveAs("h_total.root")
raw_input()