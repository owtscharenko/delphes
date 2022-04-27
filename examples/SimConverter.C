#define SimConverter_C
#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#else
class ExRootTreeReader;
class ExRootResult;
#endif
#include "SimConverter.h"
#include <TH1.h>
#include <iostream>


void SimConverter::Loop(){

    Int_t evnum;
    Int_t n_inacc = 0;
    Int_t isPU = -1;
    TObject *vtx_particle;
    GenParticle *actual_particle;
    // GenParticle *actual_track_particle;
    
    SimConverter::simple_vertex vdummy{};
    SimConverter::simple_track tdummy{};

    TString in_file = "/media/niko/big_data/delphes_output/delphes_100_pileup_50_dz_50cm_track_smearing.root";
    // TString in_file = "/media/niko/big_data/delphes_output/delphes_1_pileup_0.root";
    TChain chain("Delphes");
    chain.Add(in_file);
    ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
    Long64_t numberOfEntries = treeReader->GetEntries();

    TClonesArray *branchParticle  = treeReader->UseBranch("Particle");
    TClonesArray *branchVertex  = treeReader->UseBranch("Vertex");
    TClonesArray *branchTrack  = treeReader->UseBranch("Track");

    TString out_file_name = "/media/niko/big_data/cernbox/delphes/delphes_100_pileup_50_dz_50cm_track_smearing.root";
    TFile *nfile = TFile::Open((out_file_name), "RECREATE");
    TTree *SimOut = new TTree("DelphesSim", "Simplified Delphes Output");

    std::vector<SimConverter::simple_vertex> vertices{};
    std::vector<SimConverter::simple_track> tracks{};
    std::vector<SimConverter::simple_particle> particles{};
    std::vector<SimConverter::simple_particle> vtxparticles{};
    // std::vector<SimConverter::simple_particle> trkparticles{};

    SimOut->Branch("Vertex", &vertices);
    SimOut->Branch("Track", &tracks);
    SimOut->Branch("VtxParticle", &vtxparticles);
    SimOut->Branch("Particle", &particles);
    // SimOut->Branch("TrkParticle", &trkparticles);
    SimOut->Branch("evts", &evnum, "evnum/I");

    TH1F * n_inaccessible_particles;
    n_inaccessible_particles = new TH1F("n_ghost", "Number of empty particle pointer per event", 100, 0,0);
    // pileup vertex
    TH1F * PUvtxPosX;
    PUvtxPosX = new TH1F("xPU", "X position of pileup vertices", 100, 0,0);
    TH1F * PUvtxPosY;
    PUvtxPosY = new TH1F("yPU", "Y position of pileup vertices", 100, 0,0);
    TH1F * PUvtxPosZ;
    PUvtxPosZ = new TH1F("zPU", "Z position of pileup vertices", 100, 0,0);
    TH1F * PUVtxTime;
    PUVtxTime = new TH1F("TPU", "relative time of pileup vertices", 100, 0,0);
    // delphes smeared vertex
    TH1F * VtxPosX;
    VtxPosX = new TH1F("xVtx", "X position of vertices", 100, 0,0);
    TH1F * VtxPosY;
    VtxPosY = new TH1F("yVtx", "Y position of vertices", 100, 0,0);
    TH1F * VtxPosZ;
    VtxPosZ = new TH1F("zVtx", "Z position of vertices", 100, 0,0);
    TH1F * VtxTime;
    VtxTime = new TH1F("TVtx", "relative time of vertices", 100, 0,0);
    // Gen Particle pos (MC truht vertex)
    TH1F * GenVtxPosX;
    GenVtxPosX = new TH1F("GenxVtx", "X position of GenParticle", 100, -40,40);
    TH1F * GenVtxPosY;
    GenVtxPosY = new TH1F("GenyVtx", "Y position of GenParticle", 100, -40,40);
    TH1F * GenVtxPosZ;
    GenVtxPosZ = new TH1F("GenzVtx", "Z position of GenParticle", 100, -40,40);
    TH1F * GenVtxTime;
    GenVtxTime = new TH1F("GenTVtx", "relative time of GenParticle", 100, 0,0);

    TH1F * TrackResX;
    TrackResX = new TH1F("TrackResX", "X residual of GenPart - Track", 100, -5,5);
    TH1F * TrackResY;
    TrackResY = new TH1F("TrackResY", "Y  residual of GenPart - Track", 100, -5,5);
    TH1F * TrackResZ;
    TrackResZ = new TH1F("TrackResZ", "Z  residual of GenPart - Track", 100, -140,140);

    for(Int_t entry = 0; entry < numberOfEntries; ++entry) 
    {
        vertices.clear();
        vtxparticles.clear();
        particles.clear();
        tracks.clear();
        treeReader->ReadEntry(entry);
        evnum = entry;
        for (Int_t i=0; i<branchVertex->GetEntriesFast(); i++)
        {
            vdummy.trackX.clear();
            vdummy.trackY.clear();
            vdummy.trackZ.clear();
            vdummy.trackID.clear();

            isPU = -1;
            vdummy.IsPU = -1;
            n_inacc = 0;
            Vertex *vtx = (Vertex*) branchVertex->At(i);
            vdummy.x = vtx->X;
            vdummy.y = vtx->Y;
            vdummy.z = vtx->Z;
            vdummy.time = vtx->T;
            vdummy.vertexID = vtx->Index;
            vdummy.IsPU = vtx->IsPU;
            vdummy.sumpt2 = vtx->SumPT2;
            vdummy.ndf = vtx->NDF;

            Int_t nTracksVtx = vtx->Constituents.GetEntriesFast(); // Use this or actually accessible number of particles?
            vdummy.nTracks = nTracksVtx;
            isPU = vtx->IsPU;

            if (vdummy.IsPU==1){
                PUvtxPosX->Fill(vdummy.x);
                PUvtxPosY->Fill(vdummy.y);
                PUvtxPosZ->Fill(vdummy.z);
                PUVtxTime->Fill(vdummy.time);
            }
            if (vdummy.IsPU==0){
                VtxPosX->Fill(vdummy.x);
                VtxPosY->Fill(vdummy.y);
                VtxPosZ->Fill(vdummy.z);
                VtxTime->Fill(vdummy.time);
            }

            for(Int_t c=0; c < nTracksVtx; c++)
            {
                SimConverter::simple_particle pdummyvtx{};
                pdummyvtx.vertexID.clear();
                pdummyvtx.IsPU = -1;
                vtx_particle = vtx->Constituents.At(c);
                if(vtx_particle == 0) // nullpointer to particle. Do not yet understand how this happens. If particle associated to vertex, it should be accessible?
                {
                    n_inacc +=1;
                    continue;
                }
                actual_particle = (GenParticle*) vtx_particle;
                pdummyvtx.x = actual_particle->X;
                pdummyvtx.y = actual_particle->Y;
                pdummyvtx.z = actual_particle->Z;
                
                pdummyvtx.time = actual_particle->T;
                
                pdummyvtx.px = actual_particle->Px;
                pdummyvtx.py = actual_particle->Py;
                pdummyvtx.pz = actual_particle->Pz;

                pdummyvtx.charge = actual_particle->Charge;
                pdummyvtx.mass = actual_particle->Mass;
                pdummyvtx.E = actual_particle->E;
                pdummyvtx.eta = actual_particle->Eta;
                pdummyvtx.phi = actual_particle->Phi;
                pdummyvtx.PID = actual_particle->PID;
                // pdummyvtx.IsPU = actual_particle->IsPU;
                pdummyvtx.IsPU = isPU;
                pdummyvtx.vertexID.push_back(vtx->Index);
                
                GenVtxPosX->Fill(pdummyvtx.x);
                GenVtxPosY->Fill(pdummyvtx.y);
                GenVtxPosZ->Fill(pdummyvtx.z);
                GenVtxTime->Fill(pdummyvtx.time);

                // connect track particle with vtx. Find more elegant solution for future.
                for (Int_t i=0; i<branchTrack->GetEntriesFast(); i++){
                    Track *trk = (Track*) branchTrack->At(i);
                    GenParticle* actual_track_particle = (GenParticle*)trk->Particle.GetObject();
                    if (actual_track_particle==vtx_particle){
                        vdummy.trackX.push_back(trk->X);
                        vdummy.trackY.push_back(trk->Y);
                        vdummy.trackZ.push_back(trk->Z);
                        vdummy.trackID.push_back(i);
                    }
                }
                vtxparticles.push_back(pdummyvtx);
            }
            vertices.push_back(vdummy);
        }
        n_inaccessible_particles->Fill(n_inacc);
        
        for (Int_t i=0; i<branchTrack->GetEntriesFast(); i++){
            Track *trk = (Track*) branchTrack->At(i);
            tdummy.IsPU = -1;
            tdummy.x = trk->X;
            tdummy.y = trk->Y;
            tdummy.z = trk->Z;
            tdummy.time = trk->T;
            tdummy.eta = trk->Eta;
            tdummy.phi = trk->Phi;
            tdummy.PT = trk->PT;
            tdummy.P = trk->P;
            tdummy.mass = trk->Mass;
            tdummy.charge = trk->Charge;
            tdummy.PID = trk->PID;
            // tdummy.IsPU = trk->IsPU;
            tdummy.trackID = i;
            tdummy.vertexID = trk->VertexIndex;
            tdummy.errorDZ = trk->ErrorDZ;
            tdummy.errorD0 = trk->ErrorD0;

            GenParticle* actual_track_particle = (GenParticle*)trk->Particle.GetObject();
            if (actual_track_particle!=0){
                tdummy.GenX = actual_track_particle->X;
                tdummy.GenY = actual_track_particle->Y;
                tdummy.GenZ = actual_track_particle->Z;
                tdummy.GenMass = actual_track_particle->Mass;
                tdummy.GenCharge = actual_track_particle->Charge;
                tdummy.GenP = actual_track_particle->P;
                tdummy.GenPT = actual_track_particle->PT;
                tdummy.GenEta = actual_track_particle->Eta;
                tdummy.GenPhi = actual_track_particle->Phi;
                tdummy.GenTime = actual_track_particle->T;
                tdummy.GenPID = actual_track_particle->PID;
            }

            tracks.push_back(tdummy);
            TrackResX->Fill(tdummy.GenX - tdummy.x);
            TrackResY->Fill(tdummy.GenY - tdummy.y);
            TrackResZ->Fill(tdummy.GenZ - tdummy.z);
        }

        for (Int_t i=0; i<branchParticle->GetEntriesFast(); i++){
            GenParticle *gen = (GenParticle*) branchParticle->At(i);
            SimConverter::simple_particle pdummy{};
            pdummy.vertexID.clear();
            pdummy.x = gen->X;
            pdummy.y = gen->Y;
            pdummy.z = gen->Z;
            pdummy.time = gen->T;
            pdummy.eta = gen->Eta;
            pdummy.phi = gen->Phi;
            pdummy.PT = gen->PT;
            pdummy.P = gen->P;
            pdummy.mass = gen->Mass;
            pdummy.charge = gen->Charge;
            // pdummy.IsPU = gen->IsPU;
            pdummy.px = gen->Px;
            pdummy.py = gen->Py;
            pdummy.pz = gen->Pz;
            pdummy.PID = gen->PID;
            pdummy.time = gen->T;
            pdummy.E =gen->E;
            
            particles.push_back(pdummy);
        }
        
        SimOut->Fill();

    }
    n_inaccessible_particles->Write();
    PUvtxPosX->Write();
    PUvtxPosY->Write();
    PUvtxPosZ->Write();
    PUVtxTime->Write();
    VtxPosX->Write();
    VtxPosY->Write();
    VtxPosZ->Write();
    VtxTime->Write();
    GenVtxPosX->Write();
    GenVtxPosY->Write();
    GenVtxPosZ->Write();
    GenVtxTime->Write();
    TrackResX->Write();
    TrackResY->Write();
    TrackResZ->Write();

    SimOut->Write();
    delete nfile;
}

