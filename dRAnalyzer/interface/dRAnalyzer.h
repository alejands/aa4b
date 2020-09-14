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
      void dRStudy(const edm::Event&, const edm::EventSetup&);

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
