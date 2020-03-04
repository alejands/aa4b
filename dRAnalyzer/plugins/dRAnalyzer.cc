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

      TH1F* hNumGenParticles;
      TH1F* hNumDaughters;
      TH1F* hGenId;
      TH1F* hHiggsDaughterId;
      TH1F* hHiggsNumDaughters;
      TH1F* hADaughterId;
      TH1F* hANumDaughters;
      TH1F* hBBDeltaR;
      //TH1F* hMass;
      //TH1F* hpT;
      //TH1F* hEta;
      //TH1F* hPhi;
      //TH1F* hDR;
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

   const float EEXtalDeltaR = 0.0174;
   const int nDeltaRBins = (int)(1.65/EEXtalDeltaR);

   hNumGenParticles =   fs->make<TH1F>("NumGenParticles", "Number of Gen Particles in Event",   100, 0, 500);
   hNumDaughters =      fs->make<TH1F>("NumDaughters", "Number of Daughters",                   20, 0, 20);
   hGenId =             fs->make<TH1F>("GenId", "Gen Particle pdgId",                           80, -40, 40);
   hHiggsDaughterId =   fs->make<TH1F>("HiggsDaughterId", "Higgs Daughter ID",                  20,20,40);
   hHiggsNumDaughters = fs->make<TH1F>("HiggsNumDaughters", "Higgs Number of Daughters",        5,0,5);
   hADaughterId =       fs->make<TH1F>("ADaughterId", "a Daughter Id",                          80,-40,40);
   hANumDaughters =     fs->make<TH1F>("ANumDaughters", "a Number of Daughters",                20,0,20);
   hBBDeltaR =          fs->make<TH1F>("BBDeltaR", "bb #DeltaR",                               nDeltaRBins,0,nDeltaRBins*EEXtalDeltaR);
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
 
    hNumGenParticles->Fill(genParticles->size());
    for (unsigned iG=0; iG < genParticles->size(); iG++){
 
        reco::GenParticleRef iGen(genParticles, iG);
        hNumDaughters->Fill(iGen->numberOfDaughters());
        hGenId->Fill(iGen->pdgId());
  
        unsigned numD;
        if (std::abs(iGen->pdgId()) == 25){ // Higgs
            numD = iGen->numberOfDaughters();
   
            bool decaysToAA = true;
            for (unsigned iD=0; iD<numD; iD++) if (iGen->daughter(iD)->pdgId() != 36) decaysToAA = false;
   
            if (decaysToAA){
                hHiggsNumDaughters->Fill(numD);
                for (unsigned iD=0; iD<numD; iD++){
                    hHiggsDaughterId->Fill( iGen->daughter(iD)->pdgId() );
                }
            }
        } // end Higgs loop
  
        if (iGen->pdgId() == 36) { // scalar a
            numD = iGen->numberOfDaughters();

            hANumDaughters->Fill(numD);
            for (unsigned iD=0; iD<numD; iD++){
                hADaughterId->Fill( iGen->daughter(iD)->pdgId() );
            }
            if (numD != 2) continue;
            float dR = reco::deltaR( iGen->daughter(0)->eta(),iGen->daughter(0)->phi(), iGen->daughter(1)->eta(),iGen->daughter(1)->phi() );
            hBBDeltaR->Fill(dR);
        } // end a loop
 
    } // end Gen loop


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
