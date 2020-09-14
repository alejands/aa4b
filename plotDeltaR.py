from __future__ import print_function, division
from ROOT import gROOT, kTRUE, gStyle, TH1F, TCanvas, TFile

mass = "60"
xtalDeltaR = 0.0174
nDeltaRBins = int(1.65//xtalDeltaR)

gROOT.SetBatch(kTRUE)

gStyle.SetOptStat(111111)

f_in = TFile.Open("out/m-"+mass+".root")
cc = TCanvas("cc", "", 1200,900)

deltaRTree = f_in.Get("demo").Get("deltaRTree")
hDeltaR = TH1F("hDeltaR", "a->bb (m_{a} = "+mass+" GeV) Gen Level #DeltaR(b,b)", nDeltaRBins,0,nDeltaRBins*xtalDeltaR)
hDeltaR.SetMaximum(52000)
hDeltaR.GetXaxis().SetTitle("#DeltaR(b,b)")

print("loop over tree")
for entry in xrange(deltaRTree.GetEntries()):
    if entry%10000 == 0: print(entry)
    deltaRTree.GetEntry(entry)
    for deltaR in deltaRTree.bbDeltaR:
        hDeltaR.Fill(deltaR)

hDeltaR.Draw()
cc.Update()
cc.SaveAs("img/deltaR_m-"+mass+".pdf")
