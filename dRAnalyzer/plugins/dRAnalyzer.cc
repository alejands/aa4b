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
      TH1F* hId;
      TH1F* hHDId;
      TH1F* hHNumD;
      TH1F* hADId;
      TH1F* hANumD;
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

   hNumGenParticles = fs->make<TH1F>("NumGenParticles", "Number of Gen Particles in Event", 100, 0, 500);
   hNumDaughters = fs->make<TH1F>("NumDaughters", "Number of Daughters", 20, 0, 20);
   hId = fs->make<TH1F>("Id", "Gen Particle pdgId", 80, -40, 40);
   hHDId = fs->make<TH1F>("HDId", "Higgs Daughter ID", 80,-40,40);
   hHNumD = fs->make<TH1F>("HNumD", "Higgs Number of Daughters", 20,0,20);
   hADId = fs->make<TH1F>("ADId", "a Daughter Id", 80,-40,40);
   hANumD = fs->make<TH1F>("ANumD", "a Number of Daughters", 20,0,20);
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
   for (unsigned int iG=0; iG < genParticles->size(); iG++){

      reco::GenParticleRef iGen(genParticles, iG);
      hNumDaughters->Fill(iGen->numberOfDaughters());
      hId->Fill(iGen->pdgId());

      if (std::abs(iGen->pdgId()) == 25){ // Higgs
         //TODO Fill H and a info hists
      }
   }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


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
