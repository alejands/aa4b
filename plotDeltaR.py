from __future__ import print_function, division
from ROOT import gROOT, kTRUE, gStyle, TH1F, TCanvas, TFile

mass = "12"

gROOT.SetBatch(kTRUE)

f_in = TFile.Open("out/m-"+mass+".root")
cc = TCanvas("cc", "", 1000,800)

dR_hist = f_in.demo.Get("BBDeltaR")
dR_hist.Draw()
cc.Update()
cc.SaveAs("test.pdf")
