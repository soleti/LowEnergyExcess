#####################################################################
# Please do not modify this and push it to the repository unless all other uses of this analysis
# are aware of what you are changing. If you want to make changes to this script, please do it 
# in your own local area (IE in an ipython notebook or something).
#####################################################################

# This is the production script to make stacked backgrounds for the Low Energy Excess analysis.
# This script assumes you have already run the various "singleE_*_selection.py" scripts to actually
# run the analysis and generate ttrees.
# All this script does is read in those TTrees, apply analysis cuts, and make the stacked
# background histogram from the TTrees

# To use this script, you will need to have the following packages installed:
# numpy
# matplotlib
# pandas

# To figure out how to install these things, google it. If you use pip, consider
# >> pip install numpy
# for example.

# Also in this script are hard-coded paths to where the input files live.
# If yours live in a different place, you will have to modify this.

#from ROOT import TFile, TTree
import os
import numpy as np
import matplotlib.pylab as plt
import pandas as pd
from root_numpy import root2array
from collections import OrderedDict

#####################################################################
##  These are some sample analysis cuts one may want to apply
##  Look at how these are formatted and used if you want to apply your own
#10cm from all sides
defaultcut = '_e_Edep > 50.'
fidvolcut = '_x_vtx > 10 and _x_vtx < 246.35 and _y_vtx > -106.5 and _y_vtx < 106.5 and _z_vtx > 10 and _z_vtx < 1026.8'
tracklencut = '_longestTrackLen < 100.'
BGWcut = '_flash_time > 0. and _flash_time < 0.2'
#####################################################################

# Default x-axis variable for stacked histograms
default_plot_variable = '_e_nuReco'

# Where the output files live that contain ttrees to plot from
filebase = os.environ['LARLITE_USERDEVDIR']+'/LowEnergyExcess/output/'
filenames = OrderedDict([
	                     ('nue','singleE_nue_selection_mc.root'),
                         ('numu','singleE_numu_selection_mc.root'),
                         ('nc','singleE_nc_selection_mc.root'),
                         ('cosmic','singleE_cosmic_selection_mc.root'),
                         ('lee','singleE_LEE_selection_mc.root')
                        ])

treenames = { 
	      'nue' : 'beamNuE',
          'cosmic' : 'cosmicShowers',
          'numu' : 'beamNuMu',
          'nc' : 'beamNC',
          'lee' : 'LEETree'
          }

labels = { 
	      'nue' : 'Beam Intrinsic Nue',
          'cosmic' : 'CRY Cosmic (Scaled to BGW Exposure Time)',#'CRY Cosmic, in-time',
          'numu' : 'Beam Intrinsic Numu',
          'nc' : 'Beam Intrinsic NC', 
          'lee' : 'Scaled Low Energy Excess'
          }

colors = { 
	      'nue' : '#269729', #kGreen-2
          'numu' : '#4B4EAC', #kBlue-5
          'nc' : '#6B70F5', #kBlue-9
          'cosmic' : '#D12C2C', #kRed-3
          'lee' : '#E65C00' #orangish
          }

#These weights are to scale to 6.6e20 POT
scaling_weights = { 
	         'nue' : 6.6e20/(2.706e15*99600), #99600 is # of total BNB events looped over
             'cosmic' : 2.52, #(211,000 ms total exposure)/(6.4ms * 13100 evts generated) 
             #(this cosmic weight for if NOT using flash matching)
             'numu' : 6.6e20/(2.706e15*99600),
             'nc' : 6.6e20/(2.706e15*99600),
             'lee' : 1 #LEE weights to scale to 6.6e20 are fully contained in the output tree
                  }

# Read in all the ttrees to pandas dataframes
dfs = OrderedDict()
for key, filename in filenames.iteritems():
    dfs.update( { key : pd.DataFrame( root2array( filebase + filename, treenames[key] ) ) } )

# Uncomment this if you want to see what variables are stored in the dataframes
# dfs['cosmic'].info()

# This function makes a weighted numpy histogram from the dataframes
# You give it a "query" (analysis cut), the variable you want to have
# on the x-axis, and an optional "scale factor" which is just to convert
# units (IE from MEV to GEV)
# It returns a dictionary of { sample name : histogram }
def gen_histos( binning = np.linspace(0,10,1), myquery='', plotvar = default_plot_variable, scalefactor = 1.):
    nphistos = OrderedDict()

    for key, df in dfs.iteritems():
        mydf = df.query(myquery) if myquery else df
        # Cosmic weights are applied slightly differently
        # Once we switch to full flash-matching (and not just
        # scaling to beam gate open exposure time) this will
	    # change.
        if key == 'cosmic':
            myweights = np.ones(mydf[default_plot_variable].shape[0])
        else:
            myweights = np.array(mydf['_weight'])
        myweights *= scaling_weights[key]
        nphistos.update( {key : np.histogram(mydf[plotvar]/scalefactor,
                                     bins=binning,
                                     weights=myweights)} )
    return nphistos

# This function loops over the dataframes and makes the 
# stacked background. It takes as input the dictionary of already-created
# histograms and just draws them prettily
def plot_fullstack( binning = np.linspace(0,10,1), myquery='', plotvar = default_plot_variable, \
                    scalefactor = 1., user_ylim = None):

    fig = plt.figure(figsize=(10,6))
    plt.grid(True)
    lasthist = 0
    myhistos = gen_histos(binning=binning,myquery=myquery,plotvar=plotvar,scalefactor=scalefactor)
    for key, (hist, bins) in myhistos.iteritems():
      if key == 'cosmic': continue
      plt.bar(bins[:-1],hist,
              width=bins[1]-bins[0],
              color=colors[key],
              bottom = lasthist,
              edgecolor = 'k',
              label='%s: %d Events'%(labels[key],sum(hist)))
      lasthist += hist
     

    plt.title('CCSingleE Stacked Backgrounds',fontsize=25)
    plt.ylabel('Events',fontsize=20)
    if plotvar == '_e_nuReco':
        xstring = 'Reconstructed Neutrino Energy [GeV]' 
    elif plotvar == '_e_CCQE':
        xstring = 'CCQE Energy [GeV]'
    else:
        xstring = plotvar
    plt.xlabel(xstring,fontsize=20)
    plt.legend()
    plt.xticks(list(plt.xticks()[0]) + [binning[0]])
    plt.xlim([binning[0],binning[-1]])
    #plt.ylim([0,130])

if __name__ == '__main__':
  mybins = np.linspace(0.1,3.0,15)
  plot_fullstack(binning=mybins, myquery=defaultcut+' and '+tracklencut, \
    plotvar='_e_nuReco', scalefactor=1000.)
  plt.show()


