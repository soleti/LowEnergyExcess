import os, datetime, sys
from ROOT import larlite as fmwk
_use_reco = False

input_base = '/Users/soleti/data/'
output_dir = '/Users/soleti/larlite/UserDev/LowEnergyExcess/output/'

cosmics_files =  input_base + 'osc_cosmics_70kv_all_mcinfo.root'
cosmics_files += ' ' + input_base + 'osc_cosmics_70kv_all_opdata.root'

if _use_reco:
	cosmics_files += ' ' + input_base + 'osc_cosmics_70kv_all_reco3d_fuzzyshower.root'
	cosmics_files += ' ' + input_base + 'osc_cosmics_70kv_all_reco3d_kalmanhitcc.root'

dirt_files = None
bnb_files =  input_base + 'osc_bnb_70kv_all_mcinfo.root'
bnb_files += ' ' + input_base + 'osc_bnb_70kv_all_opdata.root'

if _use_reco:
	bnb_files += ' ' + input_base + 'osc_bnb_70kv_all_reco3d_fuzzyshower.root'
	bnb_files += ' ' + input_base + 'osc_bnb_70kv_all_reco3d_kalmanhitcc.root'
		
lee_files = '/Users/soleti/data/LEEgen_mcinfo_all.root'

starttime = datetime.datetime.now()
#os.system('source /Users/soleti/larlite/config/setup.sh')
print "run_all_selections start time is",starttime
if cosmics_files: os.system('source /Users/soleti/larlite/config/setup.sh;python singleE_cosmic_selection.py %s %s %s'%('reco' if _use_reco else 'mc',cosmics_files,output_dir))
if dirt_files: os.system('source /Users/soleti/larlite/config/setup.sh;python singleE_dirt_selection.py %s %s %s'%('reco' if _use_reco else 'mc',dirt_files,output_dir))
if bnb_files: os.system('source /Users/soleti/larlite/config/setup.sh;python singleE_nc_selection.py %s %s %s'%('reco' if _use_reco else 'mc',bnb_files,output_dir))
if bnb_files: os.system('source /Users/soleti/larlite/config/setup.sh;python singleE_nue_selection.py %s %s %s'%('reco' if _use_reco else 'mc',bnb_files,output_dir))
if bnb_files: os.system('source /Users/soleti/larlite/config/setup.sh;python singleE_numu_selection.py %s %s %s'%('reco' if _use_reco else 'mc',bnb_files,output_dir))
if lee_files: os.system('source /Users/soleti/larlite/config/setup.sh;python singleE_LEE_selection.py %s %s %s'%('reco' if _use_reco else 'mc',lee_files,output_dir))
print "run_all_selections total time duration is",datetime.datetime.now()-starttime
