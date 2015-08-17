import sys
from ROOT import gSystem
gSystem.Load("libLowEnergyExcess_Utilities")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Utilities..."

sys.exit(0)

