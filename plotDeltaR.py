from __future__ import print_function, division
from ROOT import gROOT, kTRUE, gStyle, TH1F, TCanvas, TFile

mass = "30"

gROOT.SetBatch(kTRUE)

gStyle.SetOptStat(111111)

f_in = TFile.Open("out/m-"+mass+".root")
cc = TCanvas("cc", "", 1200,900)

dR_hist = f_in.demo.Get("BBDeltaR")
dR_hist.SetTitle("a->bb (m_{a} = "+mass+" Gev) Gen Level #DeltaR(b,b)")
dR_hist.GetXaxis().SetTitle("#DeltaR(b,b)")
dR_hist.SetMaximum(52000)
dR_hist.Draw()
cc.Update()
cc.SaveAs("img/deltaR_m-"+mass+".pdf")
