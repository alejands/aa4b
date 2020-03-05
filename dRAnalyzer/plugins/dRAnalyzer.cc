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


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <TTree.h>
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class dRAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit dRAnalyzer(const edm::ParameterSet&);
      ~dRAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<reco::GenParticleCollection> genParticleCollectionT_;

      TTree* deltaRTree;
      int NumGenParticles;
      std::vector<int> NumDaughters;
      std::vector<int> GenId;
      std::vector<int> HiggsDaughterId;
      std::vector<int> HiggsNumDaughters;
      std::vector<int> aDaughterId;
      std::vector<int> aNumDaughters;
      std::vector<float> bbDeltaR;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

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
    //using namespace edm;
    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByToken(genParticleCollectionT_, genParticles);

    NumDaughters.clear();
    GenId.clear();
    HiggsDaughterId.clear();
    HiggsNumDaughters.clear();
    aDaughterId.clear();
    aNumDaughters.clear();
    bbDeltaR.clear(); 

    NumGenParticles = genParticles->size();

    for (unsigned iG=0; iG < genParticles->size(); iG++){ // Gen
 
        reco::GenParticleRef iGen(genParticles, iG);
        NumDaughters.push_back( iGen->numberOfDaughters() );
        GenId.push_back( iGen->pdgId() );
  
        unsigned numD;

        if (std::abs(iGen->pdgId()) == 25){ // Higgs
            numD = iGen->numberOfDaughters();
            HiggsNumDaughters.push_back(numD);
            for (unsigned iD=0; iD<numD; iD++){
                HiggsDaughterId.push_back( iGen->daughter(iD)->pdgId() );
            }
        } // end Higgs loop
  
        if (iGen->pdgId() == 36) { // scalar a
            numD = iGen->numberOfDaughters();
            aNumDaughters.push_back(numD);

            for (unsigned iD=0; iD<numD; iD++){
                aDaughterId.push_back( iGen->daughter(iD)->pdgId() );
            }
            if (numD != 2) continue;
            float dR = reco::deltaR( iGen->daughter(0)->eta(),iGen->daughter(0)->phi(), iGen->daughter(1)->eta(),iGen->daughter(1)->phi() );
            bbDeltaR.push_back(dR);
        } // end a loop
 
    } // end Gen loop

    deltaRTree->Fill();

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
