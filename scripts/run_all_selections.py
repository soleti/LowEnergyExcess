import os, datetime, sys

_use_reco = False

input_base = '/Users/soleti/data/'
output_dir = '/Users/soleti/larlite/UserDev/LowEnergyExcess/output/'
if _use_reco:
	output_dir = '/Users/davidkaleko/larlite/UserDev/LowEnergyExcess/output/70KV/recoemmaster/with_reco_noenergysmear_retrained/'


cosmics_files =  input_base + 'cosmic_mcinfo.root'
cosmics_files += ' ' + input_base + 'cosmic_opdigit.root'

if _use_reco:
	# cosmics_files += ' ' + input_base + 'osc_cosmics_70kv_all_reco3d_fuzzyshower.root'
	# cosmics_files += ' ' + input_base + 'osc_cosmics_70kv_all_reco3d_kalmanhitcc.root'
	cosmics_files += ' ' + input_base + 'osc_cosmics_70kv_all_recoem_master_noshowereneegysmear.root'

dirt_files = '/Users/soleti/data/bnb_numu_mcinfo.root'
dirt_files += ' /Users/soleti/data/bnb_numu_opdigit.root'

bnb_files =  input_base + 'bnb_signalnue_mcinfo.root'
bnb_files += ' ' + input_base + 'bnb_signalnue_opdigit.root'

if _use_reco:
	# bnb_files += ' ' + input_base + 'osc_bnb_70kv_all_reco3d_fuzzyshower.root'
	# bnb_files += ' ' + input_base + 'osc_bnb_70kv_all_reco3d_kalmanhitcc.root'
	bnb_files += ' ' + input_base + 'osc_bnb_70kv_all_recoem_master_noshowereneegysmear.root'
		
lee_files = '/Users/soleti/data/LEEgen_mcinfo_all.root'

if _use_reco:
	lee_files += ' /Users/davidkaleko/Data/larlite/LEEgen_mcinfo_recoemtest.root'

starttime = datetime.datetime.now()
print "run_all_selections start time is",starttime
#if cosmics_files: os.system('python singleE_cosmic_selection.py %s %s %s'%('reco' if _use_reco else 'mc',cosmics_files,output_dir))
#if dirt_files: os.system('python singleE_dirt_selection.py %s %s %s'%('reco' if _use_reco else 'mc',dirt_files,output_dir))
#if bnb_files: os.system('python singleE_nc_selection.py %s %s %s'%('reco' if _use_reco else 'mc',bnb_files,output_dir))
#if bnb_files: os.system('python singleE_nue_selection.py %s %s %s'%('reco' if _use_reco else 'mc',bnb_files,output_dir))
if dirt_files: os.system('python singleE_numu_selection.py %s %s %s'%('reco' if _use_reco else 'mc',bnb_files,output_dir))
#if lee_files: os.system('python singleE_LEE_selection.py %s %s %s'%('reco' if _use_reco else 'mc',lee_files,output_dir))
print "run_all_selections total time duration is",datetime.datetime.now()-starttime
