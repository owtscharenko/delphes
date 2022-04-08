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
    TObject *vtx_particle;
    GenParticle * actual_particle;
    SimConverter::simple_particle pdummy;
    SimConverter::simple_vertex vdummy;

    TString in_file = "/media/niko/big_data/delphes_output/delphes_100_pileup_0.root";
    TChain chain("Delphes");
    chain.Add(in_file);
    ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
    Long64_t numberOfEntries = treeReader->GetEntries();

    TClonesArray *branchParticle  = treeReader->UseBranch("Particle");
    TClonesArray *branchVertex  = treeReader->UseBranch("Vertex");
    TClonesArray *branchTrack  = treeReader->UseBranch("Track");

    TString out_file_name = "/media/niko/big_data/delphes_output/delphes_100_pileup_0_convert.root";
    TFile *nfile = TFile::Open((out_file_name), "RECREATE");
    TTree *SimOut = new TTree("DelphesSim", "Simplified Delphes Output");

    std::vector<SimConverter::simple_vertex> vertices;
    SimConverter::simple_track tracks;
    std::vector<SimConverter::simple_particle> particles;

    SimOut->Branch("Vertex", &vertices);
    SimOut->Branch("Track", &tracks);
    SimOut->Branch("Particle", &particles);
    SimOut->Branch("evts", &evnum, "evnum/I");

    TH1F * n_inaccessible_particles;
    n_inaccessible_particles = new TH1F("n_ghost", "Number empty particle pointer per event", 100, 0,0);

    for(Int_t entry = 0; entry < numberOfEntries; ++entry) 
    {
        vertices.clear();
        particles.clear();
        treeReader->ReadEntry(entry);
        evnum = entry;
        for (Int_t i=0; i<branchVertex->GetEntriesFast(); i++)
        {
            n_inacc = 0;
            Vertex *vtx = (Vertex*) branchVertex->At(i);
            vdummy.pos.SetXYZ(vtx->X, vtx->Y, vtx->Z);
            vdummy.time = vtx->T;
            vdummy.vertexID = vtx->Index;
            vdummy.IsPU = vtx->IsPU;
            vdummy.sumpt2 = vtx->SumPT2;
            vdummy.ndf = vtx->NDF;

            Int_t nTracksVtx = vtx->Constituents.GetEntriesFast(); // Use this or actually accessible number of particles?
            vdummy.nTracks = nTracksVtx;

            vertices.push_back(vdummy);

            for(Int_t c=0; c < nTracksVtx; c++)
            {
                pdummy.vertexID.clear();
                vtx_particle = vtx->Constituents.At(c);
                if(vtx_particle == 0) 
                {
                    n_inacc +=1;
                    continue;
                }
                actual_particle = (GenParticle*) vtx_particle;
                pdummy.pos.SetXYZ(actual_particle->X, actual_particle->Y, actual_particle->Z);
                pdummy.time = actual_particle->T;
                pdummy.mom.SetXYZ(actual_particle->Px, actual_particle->Py, actual_particle->Pz);
                pdummy.charge = actual_particle->Charge;
                pdummy.mass = actual_particle->Mass;
                pdummy.E = actual_particle->E;
                pdummy.eta = actual_particle->Eta;
                pdummy.phi = actual_particle->Phi;
                pdummy.PID = actual_particle->PID;
                pdummy.vertexID.push_back(vtx->Index);
                
                particles.push_back(pdummy);
            }
        }
        n_inaccessible_particles->Fill(n_inacc);
        SimOut->Fill();
        
        // GenParticle *gen = (GenParticle*) branchParticle->At(i);
        
        // Track *trk = (Track*) branchTrack->At(i);

    }
    n_inaccessible_particles->Write();
    SimOut->Write();
    delete nfile;
}

