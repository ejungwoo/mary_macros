//#define SAVE_VA
//#define SAVE_DIST_POCA
//#define SAVE_DETAIL // ndf, length, charge, pdg
//#define SAVE_GOODNESS // good vertex, good track

TClonesArray *vertexArray = nullptr;
//TClonesArray *clusterArray = nullptr;
TClonesArray *recoTrackArray = nullptr;
TClonesArray *vaddTrackArray = nullptr;

  Bool_t BeamSigma20, fGoodVertex, fGoodTrack;
   Int_t fNumClusters, fNumClusters2, fPDG, fPDG2, fCharge, fCharge2;
Long64_t fEventID, fNumTracks, fNumTracks2;
Double_t fVx, fVy, fVz;
Double_t fTrackLength,  fDist,  fdEdx,  fP,  fPX,  fPY,  fPZ,  fPOCAX,  fPOCAY,  fPOCAZ;
Double_t fTrackLength2, fDist2, fdEdx2, fP2, fPX2, fPY2, fPZ2, fPOCAX2, fPOCAY2, fPOCAZ2;

void set_reco_tree(TTree *tree);
void set_beam_tree(TTree *tree);
void set_event_tree(TTree *tree);
void set_track_tree(TTree *tree);



Bool_t fCutByRecoTrack = false;

const    Int_t fCutNumClusters = 30;
const    Int_t fCutMinNumClusters = 5;
const    Int_t fCutDist = 5; // [mm]
const Double_t fCutTrackLength = 50; // [mm]

const Double_t fCutVZMin =  -13.8;
const Double_t fCutVZMax =  -10.3;
const Double_t fCutVXMin =  -15;
const Double_t fCutVXMax =   15; 
const Double_t fCutVYMin = -246.06;
const Double_t fCutVYMax = -206.06;

//const Double_t fCutVZMin = -DBL_MAX;
//const Double_t fCutVZMax =  DBL_MAX;
//const Double_t fCutVXMin = -DBL_MAX;
//const Double_t fCutVXMax =  DBL_MAX;
//const Double_t fCutVYMin = -DBL_MAX;
//const Double_t fCutVYMax =  DBL_MAX;



void make_summary(int fRun = 2894)
{
  TString tag = "develop.1710.62716a4.set_covn";
  TString pathToReco = "/Users/ejungwoo/spirit/pid_from_reco/reco_data/";
  TString pathToRefined = "/Users/ejungwoo/spirit/data/";
  TString pathToSave = "/Users/ejungwoo/spirit/pid_from_reco/data/";


  auto reco_tree = new TChain("cbmsim");
  reco_tree -> Add(pathToReco+"/example.reco.root");
  set_reco_tree(reco_tree);


  auto beam_file = new TFile(pathToRefined+Form("/run%d.refined.root",fRun),"read");
  auto beam_tree = (TTree *) beam_file -> Get("beam");
  set_beam_tree(beam_tree);


  auto event_name = TString(Form(pathToSave+"/summary_event_run%d.",fRun))+tag+".root";
  TFile *event_file = new TFile(event_name,"recreate");
  TTree *event_tree = new TTree("events","");
  set_event_tree(event_tree);


  auto track_name = TString(Form(pathToSave+"/summary_track_run%d.",fRun))+tag+".root";
  auto track_file = new TFile(track_name,"recreate");
  auto track_tree = new TTree("tracks","");
  set_track_tree(track_tree);


  /*****************************************************************************************/
  /*****************************************************************************************/
  /*****************************************************************************************/


  Long64_t numEvents = reco_tree -> GetEntries();
  for (Long64_t entry = 0; entry < numEvents; ++entry)
  {
    fEventID = entry;
    if (entry % 500 == 0)
      cout << "Event " << entry << " / " << numEvents << " - "<< 100.*entry/numEvents << "%" << endl;

    beam_tree -> GetEntry(entry);
    if (!fBeamSigma20)
      continue;

    reco_tree -> GetEntry(entry);
    if (vertexArray->GetEntries()!= 1)
      continue;

    auto choosen_vertexid = 0;
    auto numVertex = vertexArray -> GetEntries();
    auto vertex = (STVertex *) vertexArray -> At(0);

    auto posVertex = vertex -> GetPos();
    fVx = posVertex.X();
    fVy = posVertex.Y();
    fVz = posVertex.Z();

    Bool_t missingVaddTrack = false;
    if (vaddTrackArray == nullptr) {
      missingVaddTrack = true;
      fCutByRecoTrack = true;
    }

    if (missingVaddTrack)
      vaddTrackArray = recoTrackArray;

    fNumTracks  = recoTrackArray -> GetEntries();
    fNumTracks2 = vaddTrackArray -> GetEntries();

    fGoodVertex = false;
    event_tree -> Fill();
    if (fVz<fCutVZMin || fVz>fCutVZMax || fVx<fCutVXMin || fVx>fCutVXMax || fVy<fCutVYMin || fVy>fCutVYMax)
      continue;
    fGoodVertex = true;

    for (auto vaddTrackID = 0; vaddTrackID < fNumTracks2; ++vaddTrackID)
    {
      auto vaddTrack = (STRecoTrack *) vaddTrackArray -> At(vaddTrackID);
      STRecoTrack *recoTrack = nullptr;

      auto recoTrackID = vaddTrack -> GetParentID();
      if (missingVaddTrack)
        recoTrack = vaddTrack;
      else
        recoTrack = (STRecoTrack *) recoTrackArray -> At(recoTrackID);

      if (recoTrack -> GetVertexID() != choosen_vertexid)
        continue;

      auto dedxArray  = recoTrack -> GetdEdxPointArray();
      auto dedxArray2 = vaddTrack -> GetdEdxPointArray();

      fNumClusters  = dedxArray -> size();
      fNumClusters2 = dedxArray2 -> size();

      if (fNumClusters<fCutMinNumClusters || fNumClusters2<fCutMinNumClusters)
        continue;

      fGoodTrack = false;

      //XXX CUT
      if (fCutByRecoTrack) {
        if (fNumClusters < fCutNumClusters)
          continue;
      } else {
        if (fNumClusters2 < fCutNumClusters)
          continue;
      }

#ifdef SAVE_DETAIL
      fPDG  = STPID::GetPDG(recoTrack -> GetPID());
      fPDG2 = STPID::GetPDG(vaddTrack  -> GetPID());
#endif

      TVector3 poca  = recoTrack -> GetPOCAVertex() - posVertex;
      TVector3 poca2 = vaddTrack -> GetPOCAVertex() - posVertex;

      fDist  = poca.Mag();
      fDist2 = poca2.Mag();

      //XXX CUT
      if (fCutByRecoTrack) {
        if (fDist > fCutDist)
          continue;
      } else {
        if (fDist2 > fCutDist)
          continue;
      }

#ifdef SAVE_DIST_POCA
      fPOCAX  = poca.X();
      fPOCAY  = poca.Y();
      fPOCAZ  = poca.Z();
      fPOCAX2 = poca2.X();
      fPOCAY2 = poca2.Y();
      fPOCAZ2 = poca2.Z();
#endif

      fTrackLength  = (dedxArray->at(fNumClusters-1)).fLength;
      fTrackLength2 = (dedxArray2->at(fNumClusters2-1)).fLength;

      //XXX CUT
      if (fCutByRecoTrack) {
        if (fTrackLength < fCutTrackLength)
          continue;
      } else {
        if (fTrackLength2 < fCutTrackLength)
          continue;
      }

      fCharge  = recoTrack -> GetCharge();
      fCharge2  = vaddTrack  -> GetCharge();

      //XXX CUT
      if (fCutByRecoTrack) {
        if (fCharge==0)
          continue;
      } else {
        if (fCharge2==0)
          continue;
      }

      fGoodTrack = true;

      auto mom  = recoTrack -> GetMomentum();
      auto mom2 = vaddTrack -> GetMomentum();

      fdEdx  = recoTrack -> GetdEdxWithCut(0.0,0.7);
      fP = mom.Mag()/fCharge;
      fPX = mom.X();
      fPY = mom.Y();
      fPZ = mom.Z();

#ifdef SAVE_VA
      fdEdx2 = vaddTrack -> GetdEdxWithCut(0.0,0.7);
      fP2 = mom2.Mag()/fCharge2;
      fPX2 = mom2.X();
      fPY2 = mom2.Y();
      fPZ2 = mom2.Z();
#endif

      track_tree -> Fill();
    }
  }

  track_file -> cd();
  track_tree -> Write();
  cout << "OUTPUT: " << track_name << endl;

  event_file -> cd();
  event_tree -> Write();
  cout << "OUTPUT: " << event_name << endl;

  gApplication -> Terminate();
  return;
}

void set_reco_tree(TTree *tree) {
  tree -> SetBranchAddress("STVertex", &vertexArray);
  tree -> SetBranchAddress("STRecoTrack", &recoTrackArray);
  //tree -> SetBranchAddress("STHitCluster", &clusterArray);
  tree -> SetBranchAddress("VATracks", &vaddTrackArray);
}

void set_beam_tree(TTree *tree) {
  tree -> SetBranchAddress("sigma20", &fBeamSigma20);
}

void set_event_tree(TTree *tree) {
#ifdef SAVE_GOODNESS
  tree -> Branch("goodv",   &fGoodVertex);
#endif

  tree -> Branch("eventid", &fEventID);
  tree -> Branch("ntracks", &fNumTracks);
  tree -> Branch("vx", &fVx);
  tree -> Branch("vy", &fVy);
  tree -> Branch("vz", &fVz);
#ifdef SAVE_VA
  tree -> Branch("ntracks2", &fNumTracks2);
  tree -> Branch("vx2", &fVx2);
  tree -> Branch("vy2", &fVy2);
  tree -> Branch("vz2", &fVz2);
#endif
}

void set_track_tree(TTree *tree) {
#ifdef SAVE_GOODNESS
  tree -> Branch("goodt",   &fGoodTrack);
#endif

#ifdef SAVE_DETAIL
  tree -> Branch("ndf",     &fNumClusters);
  tree -> Branch("length",  &fTrackLength);
  tree -> Branch("pdg",     &fPDG);
#endif
  tree -> Branch("dedx",    &fdEdx);
  tree -> Branch("p",       &fP);
  tree -> Branch("px",      &fPX);
  tree -> Branch("py",      &fPY);
  tree -> Branch("pz",      &fPZ);
#ifdef SAVE_DIST_POCA
  tree -> Branch("dist",    &fDist);
  tree -> Branch("pocax",   &fPOCAX);
  tree -> Branch("pocay",   &fPOCAY);
  tree -> Branch("pocaz",   &fPOCAZ);
#endif

#ifdef SAVE_VA
#ifdef SAVE_DETAIL
  tree -> Branch("ndf2",    &fNumClusters2);
  tree -> Branch("length2", &fTrackLength2);
  tree -> Branch("pdg2",    &fPDG2);
#endif
  tree -> Branch("dedx2",   &fdEdx2);
  tree -> Branch("p2",      &fP2);
  tree -> Branch("px2",     &fPX2);
  tree -> Branch("py2",     &fPY2);
  tree -> Branch("pz2",     &fPZ2);
#ifdef SAVE_DIST_POCA
  tree -> Branch("dist2",   &fDist2);
  tree -> Branch("pocax2",  &fPOCAX2);
  tree -> Branch("pocay2",  &fPOCAY2);
  tree -> Branch("pocaz2",  &fPOCAZ2);
#endif
#endif
}
