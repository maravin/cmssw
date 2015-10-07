#include "Validation/HcalHits/interface/SimHitsValidationHcal.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

SimHitsValidationHcal::SimHitsValidationHcal(const edm::ParameterSet& ps) {

  g4Label  = ps.getUntrackedParameter<std::string>("moduleLabel","g4SimHits");
  hcalHits = ps.getUntrackedParameter<std::string>("HitCollection","HcalHits");
  verbose_ = ps.getUntrackedParameter<bool>("Verbose", false);

  tok_hits_ = consumes<edm::PCaloHitContainer>(edm::InputTag(g4Label,hcalHits));

  edm::LogInfo("HitsValidationHcal") << "Module Label: " << g4Label << "   Hits: "
				     << hcalHits;

}

SimHitsValidationHcal::~SimHitsValidationHcal() {}

void SimHitsValidationHcal::bookHistograms(DQMStore::IBooker &ib, edm::Run const &run, edm::EventSetup const &es) {
  
    edm::LogInfo("HitsValidationHcal") << "Histograms booked";
    ib.setCurrentFolder("HcalHitsV/SimHitsValidationHcal");

    //Histograms for Hits
    
    std::string divisions[nType]={"HB0","HB1","HE0","HE1","HE2",
				  "HO0","HFL0","HFS0","HFL1","HFS1",
				  "HFL2","HFS2","HFL3","HFS3"};
    
    std::string divisions1[nType]={"HB depth1","HB depth2 ","HE depth1","HE depth2","HE depth3",
				   "HO depth1","HFL depth1", "HFS depth1","HFL depth2","HFS depth2",
				   "HFL depth3","HFS depth3","HFL depth4","HFS depth4"};
    
    double etaLow[nType]={-18.5,-18.5,-31.5,-31.5,-31.5,-17.5,-43.5,
			  -43.5,-42.5,-42.5,-42.5,-42.5,-42.5,-42.5};
    double etaHigh[nType]={18.5,18.5,31.5,31.5,31.5,17.5,43.5,43.5,42.5,42.5,42.5,42.5,42.5,42.5};
    int etaBins[nType]={37,37,62,63,63,35,87,87,85,85,85,85,85,85};
    char name[40], title[100];
    
    for (int i=0; i<nType; ++i) {
      sprintf (name, "HcalHitEta%s", divisions[i].c_str());
      sprintf (title, "Hit energy as a function of eta tower index in %s", divisions1[i].c_str());
      meHcalHitEta_[i] = ib.book1D(name, title, etaBins[i], etaLow[i], etaHigh[i]);
      
      sprintf (name, "HcalHitTimeAEta%s", divisions[i].c_str());
      sprintf (title, "Hit time as a function of eta tower index in %s", divisions1[i].c_str());
      meHcalHitTimeEta_[i] = ib.book1D(name, title, etaBins[i], etaLow[i], etaHigh[i]);
      
      sprintf (name, "HcalHitE25%s", divisions[i].c_str());
      sprintf (title, "Energy in time window 0 to 25 for a tower in %s", divisions1[i].c_str());
      meHcalEnergyl25_[i] = ib.book2D(name, title, etaBins[i], etaLow[i], etaHigh[i], 72, 0.5, 72.5);
      
      sprintf (name, "HcalHitE50%s", divisions[i].c_str());
      sprintf (title, "Energy in time window 0 to 50 for a tower in %s", divisions1[i].c_str());
      meHcalEnergyl50_[i] = ib.book2D(name, title, etaBins[i], etaLow[i], etaHigh[i], 72, 0.5, 72.5);
      
      sprintf (name, "HcalHitE100%s", divisions[i].c_str());
      sprintf (title, "Energy in time window 0 to 100 for a tower in %s", divisions1[i].c_str());
      meHcalEnergyl100_[i] = ib.book2D(name, title, etaBins[i], etaLow[i], etaHigh[i], 72, 0.5, 72.5);
      
      sprintf (name, "HcalHitE250%s", divisions[i].c_str());
      sprintf (title, "Energy in time window 0 to 250 for a tower in %s", divisions1[i].c_str());
      meHcalEnergyl250_[i] = ib.book2D(name, title, etaBins[i], etaLow[i], etaHigh[i], 72, 0.5, 72.5);
    }

    sprintf (name, "Energy_HB");
    meEnergy_HB = ib.book1D(name, name, 100,0,1);
    sprintf (name, "Energy_HE");
    meEnergy_HE = ib.book1D(name, name, 100,0,1);
    sprintf (name, "Energy_HO");
    meEnergy_HO = ib.book1D(name, name, 100,0,1);
    sprintf (name, "Energy_HF");
    meEnergy_HF = ib.book1D(name, name, 100,0,50);
    
    //sprintf (name, "Time_HB");
    //metime_HB = ib.book1D(name, name, 300,-150,150);
    //sprintf (name, "Time_HE");
    //metime_HE = ib.book1D(name, name, 300,-150,150);
    //sprintf (name, "Time_HO");
    //metime_HO = ib.book1D(name, name, 300,-150, 150);
    //sprintf (name, "Time_HF");
    //metime_HF = ib.book1D(name, name, 300,-150,150);

    //sprintf (name, "Time_Enweighted_HB");
    //metime_enweighted_HB = ib.book1D(name, name, 300,-150,150);
    //sprintf (name, "Time_Enweighted_HE");
    //metime_enweighted_HE = ib.book1D(name, name, 300,-150,150);
    //sprintf (name, "Time_Enweighted_HO");
    //metime_enweighted_HO = ib.book1D(name, name, 300,-150, 150);
    //sprintf (name, "Time_Enweighted_HF");
    //metime_enweighted_HF = ib.book1D(name, name, 300,-150,150);
}

void SimHitsValidationHcal::analyze(const edm::Event& e, const edm::EventSetup& ) {
  
  
  LogDebug("SimHitsValidationHcal") << "Run = " << e.id().run() << " Event = " 
				    << e.id().event();
  
  std::vector<PCaloHit>               caloHits;
  edm::Handle<edm::PCaloHitContainer> hitsHcal;
  
  bool getHits = false;
  e.getByToken(tok_hits_,hitsHcal); 
  if (hitsHcal.isValid()) getHits = true;
  
  LogDebug("SimHitsValidationHcal") << "SimHitsValidationHcal.: Input flags Hits " << getHits;

  if (getHits) {
    caloHits.insert(caloHits.end(),hitsHcal->begin(),hitsHcal->end());
    LogDebug("SimHitsValidationHcal") << "SimHitsValidationHcal: Hit buffer " 
				      << caloHits.size(); 
    analyzeHits (caloHits);
  }
}

void SimHitsValidationHcal::analyzeHits (std::vector<PCaloHit>& hits) {

  int nHit = hits.size();
  double entotHB = 0, entotHE = 0, entotHF = 0, entotHO = 0; 
  double timetotHB = 0, timetotHE = 0, timetotHF = 0, timetotHO = 0; 
  int    nHB=0, nHE=0, nHO=0, nHF=0;
  
  std::map<std::pair<HcalDetId,int>,energysum> map_try;
  map_try.clear();
  
  std::map<std::pair<HcalDetId,int>,energysum>::iterator itr;
  
  for (int i=0; i<nHit; i++) {
    double energy    = hits[i].energy();
    double time      = hits[i].time();
    unsigned int id_ = hits[i].id();
    HcalDetId id     = HcalDetId(id_);
    int itime        = (int)(time);
    int det          = id.det();
    int subdet       = id.subdet();
    int depth        = id.depth();
    int eta          = id.ieta();
    int phi          = id.iphi();
    unsigned int dep = hits[i].depth();
        
    // The following is a somewhat complicated method to identify the histogram to fill.
    // When the number of histograms is changed, this need to be changed as well.
    int type         =-1;
    if (subdet == static_cast<int>(HcalBarrel)) {
      entotHB += energy;
      timetotHB += time;
      nHB++;
      type     = depth-1;
    } else if (subdet == static_cast<int>(HcalEndcap)) {
      entotHE += energy;
      timetotHE += time;
      nHE++;
      type     = depth+1;
    } else if (subdet == static_cast<int>(HcalOuter)) {
      entotHO += energy;
      timetotHO += time;
      nHO++;
      type = 5;
    } else if (subdet == static_cast<int>(HcalForward)) {
      entotHF += energy;
      timetotHF += time;
      nHF++;
      type     = depth+5+2*dep;
    }

    std::pair<HcalDetId,int> id0(id,type);
    //   LogDebug("HitsValidationHcal") << "Id and type " << id << ":" << type;
    energysum ensum;
    if (map_try.count(id0) != 0) ensum =  map_try[id0];
    if (itime<250) {
      ensum.e250 += energy;
      if (itime<100) {
	ensum.e100 += energy;
	if (itime<50) {
	  ensum.e50 += energy;
	  if (itime<25) ensum.e25 += energy;
	}
      }
    }
    map_try[id0] = ensum;

    LogDebug("SimHitsValidationHcal") << "Hit[" << i << "] ID " << std::hex << id_ 
				      << std::dec << " " << id << std::dec<< " Det " << det << " Sub " 
				      << subdet << " depth " << depth << " depthX "
				      << dep << " Eta " << eta << " Phi " << phi 
				      << " E " << energy << " time " << time
				      << " type " << type;

    //    LogDebug("HitsValidationHcal") << "Hit[" << i << "] ID " << std::hex << id_ << "ID="<<std::dec << id << " Det " << det << " Sub " << subdet << " depth " << depth << " depthX " << dep << " Eta " << eta << " Phi " << phi << " E  " << energy << " time  " << time <<" itime  " << itime << " type " << type;
    
    double etax = eta;

    if ( type >= 0) {
      meHcalHitEta_[type]->Fill(etax,energy);
      meHcalHitTimeEta_[type]->Fill(etax,time);
    }
  }
  
  meEnergy_HB->Fill(entotHB);
  meEnergy_HE->Fill(entotHE);
  meEnergy_HF->Fill(entotHF);
  meEnergy_HO->Fill(entotHO);

  //metime_HB->Fill(timetotHB);
  //metime_HE->Fill(timetotHE);
  //metime_HF->Fill(timetotHF);
  //metime_HO->Fill(timetotHO);
    
  //metime_enweighted_HB->Fill(timetotHB,entotHB);
  //metime_enweighted_HE->Fill(timetotHE,entotHE);
  //metime_enweighted_HF->Fill(timetotHF,entotHF);
  //metime_enweighted_HO->Fill(timetotHO,entotHO);
    
  for ( itr = map_try.begin() ; itr != map_try.end(); ++itr)   {
    if ( (*itr).first.second >= 0) {
      HcalDetId id= (*itr).first.first;
      energysum ensum= (*itr).second;
      int eta = id.ieta();
      int phi = id.iphi();
	//KC: Since I have adjusted the histograms to be centered on the integers, we no longer need to adjust the eta and phi
	//double etax= eta-0.5;
	//double phix= phi-0.5;

      double etax = (double) eta;
      double phix = (double) phi;
      meHcalEnergyl25_[(*itr).first.second]->Fill(etax,phix,ensum.e25);
      meHcalEnergyl50_[(*itr).first.second]->Fill(etax,phix,ensum.e50);
      meHcalEnergyl100_[(*itr).first.second]->Fill(etax,phix,ensum.e100);
      meHcalEnergyl250_[(*itr).first.second]->Fill(etax,phix,ensum.e250);

      //LogDebug("HitsValidationHcal") <<" energy of tower ="<<(*itr).first<<"in time 25ns is== "<<(*itr).second.e25<<"in time 25-50ns=="<<(*itr).second.e50<<"in time 50-100ns=="<<(*itr).second.e100<<"in time 100-250 ns=="<<(*itr).second.e250;
    }
  }
  
}

DEFINE_FWK_MODULE(SimHitsValidationHcal);
