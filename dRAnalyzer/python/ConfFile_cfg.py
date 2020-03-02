import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:root://cmsxrootd-site.fnal.gov//store/mc/RunIIFall17MiniAODv2/SUSYZHToAA_AATo4B_M-12_TuneCP5_PSweight_13TeV_madgraph_pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/00000/B4310137-98DB-E911-94C9-0025905B85A0.root'
    )
)

process.demo = cms.EDAnalyzer('dRAnalyzer'
    , genParticleCollection = cms.InputTag('prunedGenParticles')
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histo.root')
    #fileName = cms.string(options.outputFile)
    )

process.p = cms.Path(process.demo)
