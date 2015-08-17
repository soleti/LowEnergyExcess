import sys
from ROOT import gSystem
gSystem.Load("libLowEnergyExcess_EventFilters")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing EventFilters..."

sys.exit(0)

