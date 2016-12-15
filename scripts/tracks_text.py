import os
from ROOT import TGeoVolume, TBRIK,TPad, TView, gStyle, gDirectory, TFile, TH1F, TCanvas, TPolyLine3D, kBlack, kRed, kBlue, TLegend, kAzure, kOrange, kMagenta
from array import array
import math

c1 = TCanvas("c1")
p1 = TPad("p1","p1",0.05,0.05,0.95,0.95)
p1.Draw()
p1.cd()
view = TView.CreateView(1)
view.SetRange(-200,-300,-300,500,1500,1300)
view.RotateView(1,0.1)
lines = []

f = open("tracks_text.txt","r")



for line in f.readlines():

    coord = [float(i) for i in line.split(",")]
    print coord
    #print chain._start_x, chain._start_y,chain._start_z
    #print chain._end_x,chain._end_y,chain._end_z
    start = coord[:3]
    print start
    direction = coord[3:]
    print direction
    polyline = TPolyLine3D(2)
    polyline.SetPoint(0, start[0], start[1], start[2])
    polyline.SetPoint(1, start[0]-100000*direction[0], start[1]-100000*direction[1], start[2]-100000*direction[2])
        

    lines.append(polyline)
            
    
for line in lines:
    line.Draw()

TPC = TPolyLine3D(10)
TPC.SetPoint(0,0,116.5,0)
TPC.SetPoint(1,256.35,116.5,0)
TPC.SetPoint(2,256.35,116.5,1036.8)
TPC.SetPoint(3,0,116.5,1036.8)
TPC.SetPoint(4,0,116.5,0)
TPC.SetPoint(5,0,-116.5,0)
TPC.SetPoint(6,256.35,-116.5,0)
TPC.SetPoint(7,256.35,-116.5,1036.8)
TPC.SetPoint(8,0,-116.5,1036.8)
TPC.SetPoint(9,0,-116.5,0)

TPC2 = TPolyLine3D(2)
TPC2.SetPoint(0,256.35,116.5,1036.8)
TPC2.SetPoint(1,256.35,-116.5,1036.8)

TPC3 = TPolyLine3D(2)
TPC3.SetPoint(0,0,116.5,1036.8)
TPC3.SetPoint(1,0,-116.5,1036.8)

TPC4 = TPolyLine3D(2)
TPC4.SetPoint(0,256.35,116.5,0)
TPC4.SetPoint(1,256.35,-116.5,0)

TPC.SetLineWidth(3)
TPC.SetLineColor(kRed)
TPC2.SetLineWidth(3)
TPC2.SetLineColor(kRed)
TPC3.SetLineWidth(3)
TPC3.SetLineColor(kRed)
TPC4.SetLineWidth(3)
TPC4.SetLineColor(kRed)

Top = TPolyLine3D(9)
Top.SetPoint(0,-304.6,659,604.8)
Top.SetPoint(1,-304.6,659,950.9)
Top.SetPoint(2,-131.6,659,950.9)
Top.SetPoint(3,-131.6,659,1124)
Top.SetPoint(4,387.6,659,1124)
Top.SetPoint(5,387.6,659,-87.3)
Top.SetPoint(6,-131.6,659,-87.3)
Top.SetPoint(7,-131.6,659,604.8)
Top.SetPoint(8,-304.6,659,604.8)
Top.SetLineWidth(3)

Under = TPolyLine3D(5)
Under.SetPoint(0,387.6,	-253.4,	210.4)
Under.SetPoint(1,-131.6,	-253.4,	210.4)
Under.SetPoint(2,-131.6,	-253.4,	556.5)
Under.SetPoint(3,387.6,	-253.4,	556.5)
Under.SetPoint(4,387.6,	-253.4,	210.4)
Under.SetLineWidth(3)



Pipe = TPolyLine3D(7)
Pipe.SetPoint(0,378.3,	252.9,	1192.2)
Pipe.SetPoint(1,378.3,	252.9,	-19.1)
Pipe.SetPoint(2,378.3,	99,	-19.1)
Pipe.SetPoint(3,380.1,	99,	-192.1)
Pipe.SetPoint(4,385.7,	-247.1,	-192.2)
Pipe.SetPoint(5,387.6,	-247.1,	1192.2)
Pipe.SetPoint(6,378.3,	252.9,	1192.2)

Pipe.SetLineWidth(3)

FT = TPolyLine3D(5)
FT.SetPoint(0,-129.7,	124.6,	-84.9)
FT.SetPoint(1,-129.7,	124.6,	1126.4)
FT.SetPoint(2,-131.6,	-221.5,	1126.4)
FT.SetPoint(3,-131.6,	-221.5,	-84.9)
FT.SetPoint(4,-129.7,	124.6,	-84.9)
FT.SetLineWidth(3)


TPC.Draw()
TPC2.Draw()
TPC3.Draw()
TPC4.Draw()
Top.Draw()
Under.Draw()
Pipe.Draw()
FT.Draw()
c1.Update()
input()