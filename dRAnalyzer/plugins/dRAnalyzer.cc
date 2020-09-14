// -*- C++ -*-
//
// Package:    aa4b/dRAnalyzer
// Class:      dRAnalyzer
//
/**\class dRAnalyzer dRAnalyzer.cc aa4b/dRAnalyzer/plugins/dRAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alejandro Sanchez
//         Created:  Mon, 02 Mar 2020 18:16:10 GMT
//
//

#include "aa4b/dRAnalyzer/interface/dRAnalyzer.h"

//
// constructors and destructor
//
dRAnalyzer::dRAnalyzer(const edm::ParameterSet& iConfig)

{
    // grab particle collection(s) to be read
    genParticleCollectionT_ = consumes<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticleCollection"));

    // now do what ever initialization is needed
    usesResource("TFileService");
    edm::Service<TFileService> fs;

    deltaRTree = fs->make<TTree>("deltaRTree", "deltaRTree");
    deltaRTree->Branch("NumGenParticles",   &NumGenParticles);
    deltaRTree->Branch("NumDaughters",      &NumDaughters);
    deltaRTree->Branch("GenId",             &GenId);
    deltaRTree->Branch("HiggsDaughterId",   &HiggsDaughterId);
    deltaRTree->Branch("HiggsNumDaughters", &HiggsNumDaughters);
    deltaRTree->Branch("aDaughterId",       &aDaughterId);
    deltaRTree->Branch("aNumDaughters",     &aNumDaughters);
    deltaRTree->Branch("bbDeltaR",          &bbDeltaR);
}


dRAnalyzer::~dRAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
dRAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    dRStudy(iEvent, iSetup);
} // end analyze()


// ------------ method called once each job just before starting event loop  ------------
void
dRAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
dRAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
dRAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(dRAnalyzer);
