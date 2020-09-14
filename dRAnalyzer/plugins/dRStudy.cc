#include "aa4b/dRAnalyzer/interface/dRAnalyzer.h"

void
dRAnalyzer::dRStudy(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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

} // end dRStudy()
