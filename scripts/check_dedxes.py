def gethists():
	infile = '/Users/davidkaleko/Data/larlite/joseph_LEE_files/osc_bnb_70kv_all_mcinfo.root'

	import sys
	from ROOT import *

	dedx_g = TH1F("dedx_g","dedx_g",1000,-1,10)
	dedx_e = TH1F("dedx_e","dedx_e",1000,-1,10)

	f = TFile(infile,"READ")
	t = f.mcshower_mcreco_tree
	t.GetEntry(0)
	b = t.mcshower_mcreco_branch

	entries = t.GetEntriesFast()

	for x in xrange(entries):
		blah = t.GetEntry(x)
		if not b.size(): continue
		for i in xrange(b.size()):
			mcs = b[i]
			if not mcs.DetProfile().E(): continue
			if abs(mcs.PdgCode()) == 11:
				fart = dedx_e.Fill(mcs.dEdx())
			elif abs(mcs.PdgCode()) == 22:
				fart = dedx_g.Fill(mcs.dEdx())

	return (dedx_g, dedx_e)
