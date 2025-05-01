// -*- C++ -*-
//
// Package:    Gen/GenAnalyzerHToEleEle
// Class:      GenAnalyzerHToEleEle
//
/**\class GenAnalyzerHToEleEle GenAnalyzerHToEleEle.cc Gen/GenAnalyzer/plugins/GenAnalyzerHToEleEle.cc

   Description: [one line class summary]

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Ruchi Chudasama
//         Created:  Wed, 04 Dec 2024 15:38:50 GMT
//
//


#include "Gen/GenAnalyzer/interface/GenAnalyzerHToEleEle.h"

using reco::GenParticle;

int nTotal_hee ;
int nPassed_hee ;

unsigned int runID_hee_;
unsigned int lumiID_hee_;
unsigned long long eventID_hee_;


vector<float> V_hee_genA_M_inv_;
vector<float> V_hee_genA_M_;
vector<float> V_hee_dR_A_Ele1_;
vector<float> V_hee_dR_A_Ele2_;
vector<float> V_hee_dR_Ele1_Ele2_;

vector<float> V_hee_A_pt_;
vector<float> V_hee_Ele1_pt_;
vector<float> V_hee_Ele2_pt_;
vector<float> V_hee_A_eta_;
vector<float> V_hee_Ele1_eta_;
vector<float> V_hee_Ele2_eta_;
vector<float> V_hee_A_phi_;
vector<float> V_hee_Ele1_phi_;
vector<float> V_hee_Ele2_phi_;

vector<float> V_hee_Ele1_Ele2_deta_;
vector<float> V_hee_Ele1_Ele2_dphi_;


TLorentzVector SetElesHEE(Float_t Ele_pt, Float_t Ele_eta, Float_t Ele_phi, Float_t Ele_mass){
  TLorentzVector Ele_Candidate;
  Ele_Candidate.SetPtEtaPhiM(Ele_pt, Ele_eta, Ele_phi, Ele_mass);
  return Ele_Candidate;
}

//
// static data member definitions
//

//
// constructors and destructor
//
GenAnalyzerHToEleEle::GenAnalyzerHToEleEle(const edm::ParameterSet& iConfig)
// :
// tracksToken_(consumes<TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tracks")))

{
  isDebug  = iConfig.getParameter<bool>("isDebug");
  print_trigger  = iConfig.getParameter<bool>("print_trigger");

  //now do what ever initialization is needed
  RHTree = fs->make<TTree>("RHTree","Gen info Tree");
  
  RHTree->Branch("Event",  &eventID_hee_);
  RHTree->Branch("Run",  &runID_hee_);
  RHTree->Branch("LumiSection",  &lumiID_hee_);

  RHTree->Branch("GenA_inv",  &V_hee_genA_M_inv_);
  RHTree->Branch("GenA",  &V_hee_genA_M_);
  RHTree->Branch("dR_A_Ele1",  &V_hee_dR_A_Ele1_);
  RHTree->Branch("dR_A_Ele2",  &V_hee_dR_A_Ele2_);
  RHTree->Branch("dR_Ele1_Ele2",  &V_hee_dR_Ele1_Ele2_);

  RHTree->Branch("A_pt",  &V_hee_A_pt_);
  RHTree->Branch("Ele1_pt",  &V_hee_Ele1_pt_);
  RHTree->Branch("Ele2_pt",  &V_hee_Ele2_pt_);
  RHTree->Branch("A_eta",  &V_hee_A_eta_);
  RHTree->Branch("Ele1_eta",  &V_hee_Ele1_eta_);
  RHTree->Branch("Ele2_eta",  &V_hee_Ele2_eta_);
  RHTree->Branch("A_phi",  &V_hee_A_phi_);
  RHTree->Branch("Ele1_phi",  &V_hee_Ele1_phi_);
  RHTree->Branch("Ele2_phi",  &V_hee_Ele2_phi_);

  RHTree->Branch("Ele1_Ele2_deta",  &V_hee_Ele1_Ele2_deta_);
  RHTree->Branch("Ele1_Ele2_dphi",  &V_hee_Ele1_Ele2_dphi_);
   
  genParticlesToken_   = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genParticles"));

}


GenAnalyzerHToEleEle::~GenAnalyzerHToEleEle()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenAnalyzerHToEleEle::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  eventID_hee_ = iEvent.id().event();
  runID_hee_ = iEvent.id().run();
  lumiID_hee_ = iEvent.id().luminosityBlock();

  V_hee_genA_M_inv_.clear();
  V_hee_genA_M_.clear();

  V_hee_dR_A_Ele1_.clear();
  V_hee_dR_A_Ele2_.clear();
  V_hee_dR_Ele1_Ele2_.clear();

  V_hee_A_pt_.clear();
  V_hee_Ele1_pt_.clear();
  V_hee_Ele2_pt_.clear();
  V_hee_A_eta_.clear();
  V_hee_Ele1_eta_.clear();
  V_hee_Ele2_eta_.clear();
  V_hee_A_phi_.clear();
  V_hee_Ele1_phi_.clear();
  V_hee_Ele2_phi_.clear();

  V_hee_Ele1_Ele2_deta_.clear();
  V_hee_Ele1_Ele2_dphi_.clear();



  float genA_mass_inv = -1111.1111;
  float genA_mass = -1111.1111;
  float A_Ele1_dR = -1111.1111;
  float A_Ele2_dR = -1111.1111;
  float Ele1_Ele2_dR = -1111.1111;

  float A_pt = -1111.1111;
  float Ele1_pt = -1111.1111;
  float Ele2_pt = -1111.1111;
  float A_eta = -1111.1111;
  float Ele1_eta = -1111.1111;
  float Ele2_eta = -1111.1111;
  float A_phi = -1111.1111;
  float Ele1_phi = -1111.1111;
  float Ele2_phi = -1111.1111;

  float Ele1_Ele2_deta = -1111.1111;
  float Ele1_Ele2_dphi = -1111.1111;

  edm::Handle<std::vector<reco::GenParticle> > genParticles;
  iEvent.getByToken(genParticlesToken_,   genParticles);



  bool pass = false;
  for (reco::GenParticleCollection::const_iterator iGen = genParticles->begin(); iGen != genParticles->end(); ++iGen) {

    if ( abs(iGen->pdgId()) != 25 || iGen->numberOfDaughters() != 2 ) continue;
    //if ( abs(iGen->daughter(0)->daughter(0)->pdgId()) != 11 || abs(iGen->daughter(0)->daughter(1)->pdgId()) != 11 || abs(iGen->daughter(1)->daughter(0)->pdgId()) != 11 || abs(iGen->daughter(1)->daughter(1)->pdgId()) != 11 ) continue;
    //if ( abs(iGen->daughter(0)->daughter(0)->status()) != 1 || abs(iGen->daughter(0)->daughter(1)->status()) != 1 || abs(iGen->daughter(1)->daughter(0)->status()) != 1 || abs(iGen->daughter(1)->daughter(1)->status()) != 1 ) continue;

    //std::cout << " Daughter 1 pdgID:"<< iGen->daughter(0)->pdgId() << " status:"<< iGen->daughter(0)->status() ;
    //std::cout << " Daughter 2 pdgID:"<< iGen->daughter(1)->pdgId() << " status:"<< iGen->daughter(1)->status()<<std::endl;

    pass = true;

    TLorentzVector GenEle1  = SetElesHEE(iGen->daughter(0)->pt(), iGen->daughter(0)->eta(), iGen->daughter(0)->phi(), iGen->daughter(0)->mass());
    TLorentzVector GenEle2  = SetElesHEE(iGen->daughter(1)->pt(), iGen->daughter(1)->eta(), iGen->daughter(1)->phi(), iGen->daughter(1)->mass());
    TLorentzVector GenA = GenEle1 + GenEle2;

    genA_mass_inv = GenA.M();
    genA_mass = iGen->mass();

    V_hee_genA_M_inv_.push_back( genA_mass_inv );
    V_hee_genA_M_.push_back( genA_mass );


    float dR_A_Ele1 = reco::deltaR( iGen->daughter(0)->eta(), iGen->daughter(0)->phi(), iGen->eta(), iGen->phi());
    float dR_A_Ele2 = reco::deltaR( iGen->daughter(1)->eta(), iGen->daughter(1)->phi(), iGen->eta(), iGen->phi());
    float dR_Ele1_Ele2 = reco::deltaR( iGen->daughter(0)->eta(), iGen->daughter(0)->phi(), iGen->daughter(1)->eta(), iGen->daughter(1)->phi());
    if (dR_Ele1_Ele2 > 0.4) {
      std::cout << "dR: " << dR_Ele1_Ele2 << std::endl;
    }

    A_Ele1_dR = dR_A_Ele1;
    A_Ele2_dR = dR_A_Ele2;
    Ele1_Ele2_dR = dR_Ele1_Ele2;

    V_hee_dR_A_Ele1_.push_back( A_Ele1_dR );
    V_hee_dR_A_Ele2_.push_back( A_Ele2_dR );
    V_hee_dR_Ele1_Ele2_.push_back( Ele1_Ele2_dR );

    A_pt = iGen->pt();
    Ele1_pt = iGen->daughter(0)->pt();
    Ele2_pt = iGen->daughter(1)->pt();
    A_eta = iGen->eta();
    Ele1_eta = iGen->daughter(0)->eta();
    Ele2_eta = iGen->daughter(1)->eta();
    A_phi = iGen->phi();
    Ele1_phi = iGen->daughter(0)->phi();
    Ele2_phi = iGen->daughter(1)->phi();

    V_hee_A_pt_.push_back(A_pt );
    V_hee_Ele1_pt_.push_back(Ele1_pt );
    V_hee_Ele2_pt_.push_back(Ele2_pt );
    V_hee_A_eta_.push_back(A_eta );
    V_hee_Ele1_eta_.push_back(Ele1_eta );
    V_hee_Ele2_eta_.push_back(Ele2_eta );
    V_hee_A_phi_.push_back(A_phi );
    V_hee_Ele1_phi_.push_back(Ele1_phi );
    V_hee_Ele2_phi_.push_back(Ele2_phi );

    Ele1_Ele2_deta = abs(Ele1_eta-Ele2_eta);
  
    Ele1_Ele2_dphi = abs(reco::deltaPhi( Ele1_phi, Ele2_phi )); 

    V_hee_Ele1_Ele2_deta_.push_back( Ele1_Ele2_deta );
    V_hee_Ele1_Ele2_dphi_.push_back( Ele1_Ele2_dphi );

  } // gen particle collection, looping over Higgs.
  nTotal_hee++;
  if (pass) {
    nPassed_hee++;
    //fillTrigger( iEvent, iSetup );
    RHTree->Fill();

  }



#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void
GenAnalyzerHToEleEle::beginJob()
{
  nTotal_hee = 0;
  nPassed_hee = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void
GenAnalyzerHToEleEle::endJob()
{
  std::cout << "  >>>>>> Total events selected events <<<<<  "<<nPassed_hee<<"/"<<nTotal_hee<<std::endl;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenAnalyzerHToEleEle::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenAnalyzerHToEleEle);
