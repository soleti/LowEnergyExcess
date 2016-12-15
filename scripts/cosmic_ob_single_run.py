import os, datetime, sys
from glob import glob

mcinfo = glob("/Users/soleti/data/matt_cosmic_ob/CRT_3ftOB_00*_larlite_mcinfo.root")
opreco = glob("/Users/soleti/data/matt_cosmic_ob/CRT_3ftOB_00*_larlite_opreco.root")

for i, file in enumerate(mcinfo):
    condor_single = open("single_cosmic_ob_matt.txt","w")
    condor_single.write("mc\n%s\n%s\n/Users/soleti/larlite/UserDev/LowEnergyExcess/output/single_cosmic_ob/crt_%03d.root\n" % (file,opreco[i],i))
    condor_single.close()
    os.system('python condor_singleE_cosmic_selection.py single_cosmic_ob_matt.txt')
