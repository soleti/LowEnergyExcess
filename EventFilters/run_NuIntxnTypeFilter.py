import sys



if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()
#my_proc.set_verbosity(fmwk.msg.kNORMAL)

# Set input root file
for x in xrange(len(sys.argv)):
    if x == 0: continue
    my_proc.add_input_file(sys.argv[x])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("");

my_modes = [ fmwk.NuIntxnTypeFilter.kQE, fmwk.NuIntxnTypeFilter.kResonant, fmwk.NuIntxnTypeFilter.kDIS ]
my_nus = [ fmwk.NuIntxnTypeFilter.kNue, fmwk.NuIntxnTypeFilter.kNumu ]

import itertools
all_combos = list(itertools.product(my_modes, my_nus))

# Attach a template process
myfilter = fmwk.NuIntxnTypeFilter()
my_proc.add_process(myfilter);

for my_mode, my_nu in all_combos:
    myfilter.setDesiredMode(my_mode)
    myfilter.setDesiredNu(my_nu)

    print "Running my_mode = %d, my_nu = %d." % (my_mode, my_nu)

    # Let's run it.
    my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)

