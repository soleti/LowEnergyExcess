import sys, os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILEs $OUTPUT_PATH\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

if sys.argv[1] not in ['reco','mc']:
	msg = '\n'
	msg += 'Specify if you want to use "reco" or "mc" quantities in your first argument to this script!'
	msg += '\n'
	sys.stderr.write(msg)
	sys.exit(1)

from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import ertool
from singleE_config import GetERSelectionInstance

# Create ana_processor instance
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)

use_reco = True if sys.argv[1] == 'reco' else False

# Set input root file
for x in xrange(len(sys.argv)-3):
    my_proc.add_input_file(sys.argv[x+2])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
outfile = sys.argv[-1]+'/'+sys.argv[0][:-3]+'_%s'%('mc' if not use_reco else 'reco')+'.root'

my_proc.set_ana_output_file(outfile)

lee_ana = ertool.ERAnaLowEnergyExcess()
lee_ana.SetTreeName("LEETree")
#lee_ana.SetDebug(False)
lee_ana.SetLEESampleMode(True)


anaunit = GetERSelectionInstance(flash=False)
anaunit._mgr.ClearCfgFile()
anaunit._mgr.AddCfgFile(os.environ['LARLITE_USERDEVDIR']+'/SelectionTool/ERTool/dat/ertool_default%s.cfg'%('_reco' if use_reco else ''))

if use_reco:
    anaunit.SetShowerProducer(False,'showerrecofuzzy')
    anaunit.SetTrackProducer(False,'stitchkalmanhitcc')


anaunit._mgr.AddAna(lee_ana)
my_proc.add_process(anaunit)

my_proc.run()
# my_proc.run(0,500)

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

