import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

#inputFile = "dRAnalyzer/filelists/aaTo4b_M-12.txt"
#inputFile = "dRAnalyzer/filelists/aaTo4b_M-15.txt"
#inputFile = "dRAnalyzer/filelists/aaTo4b_M-20.txt"
#inputFile = "dRAnalyzer/filelists/aaTo4b_M-25.txt"
#inputFile = "dRAnalyzer/filelists/aaTo4b_M-30.txt"
#inputFile = "dRAnalyzer/filelists/aaTo4b_M-40.txt"
#inputFile = "dRAnalyzer/filelists/aaTo4b_M-50.txt"
inputFile = "dRAnalyzer/filelists/aaTo4b_M-60.txt"
filelist = []
with open(inputFile) as f:
    for line in f:
        line = line.strip()
        if line == '': continue
        if line[0] == '#': continue
        filelist.append(line)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(filelist)
)

process.demo = cms.EDAnalyzer('dRAnalyzer'
    , genParticleCollection = cms.InputTag('prunedGenParticles')
)

process.TFileService = cms.Service("TFileService",
    #fileName = cms.string('histo.root')
    #fileName = cms.string('out/m-12.root')
    #fileName = cms.string('out/m-15.root')
    #fileName = cms.string('out/m-20.root')
    #fileName = cms.string('out/m-25.root')
    #fileName = cms.string('out/m-30.root')
    #fileName = cms.string('out/m-40.root')
    #fileName = cms.string('out/m-50.root')
    fileName = cms.string('out/m-60.root')
)

process.p = cms.Path(process.demo)
