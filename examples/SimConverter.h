#ifndef SimConverter_h
#define SimConverter_h

#include <TMath.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TVectorD.h>
#include <TLorentzVector.h>
#include <TClonesArray.h> 

#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"

class SimConverter
{
    public:

    TTree   *fChain;
    Int_t   fCurrent;

    UInt_t  event_number;

    SimConverter();
    virtual ~SimConverter();


    struct simple_vertex {
        TVector3 pos;
        Float_t time;
        UInt_t IsPU;
        Int_t vertexID;
        UInt_t ndf;
        Float_t sumpt2;
        UInt_t nTracks;
    };
    
    struct simple_track {
        TVector3 pos;
        Float_t time;
        Float_t eta;
        Float_t phi;
        Float_t PT;
        Float_t P;
        Float_t mass;
        UInt_t IsPU;
        Int_t trackID;
        Int_t vertexID;
    };

    struct simple_particle{
        TVector3 pos;
        TVector3 mom;
        Float_t time;
        Float_t mass;
        Float_t E;
        Float_t P;
        Float_t PT;
        Float_t charge;
        Float_t eta;
        Float_t phi;
        UInt_t IsPU;
        Int_t PID;
        std::vector<Int_t> trackID;
        std::vector<Int_t> vertexID;
    };

    void Loop();

    // virtual Int_t    GetEntry(Long64_t entry);
    // virtual Long64_t LoadTree(Long64_t entry);
    // virtual Bool_t   Notify();
    // virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef SimConverter_C
    SimConverter::SimConverter() : fChain(0)
    {
        // TString in_file = "/media/niko/big_data/delphes_output/only_vtx_pileup_100evts.root"
        // TChain chain("Delphes");
        // chain.Add(in_file);
        // ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
    }

    SimConverter::~SimConverter() 
    {
        if (!fChain) return;
        delete fChain->GetCurrentFile();
    }

    // Int_t SimConverter::GetEntry(Long64_t entry) 
    // {
    //     // Read contents of entry.
    //     if (!fChain) return 0;
    //     return fChain->GetEntry(entry);   
    // }

    // Long64_t SimConverter::LoadTree(Long64_t entry) 
    // {
    //     // Set the environment to read one entry
    //     if (!fChain) return -5;
    //     Long64_t centry = fChain->LoadTree(entry);
    //     if (centry < 0) return centry;
    //     if (fChain->GetTreeNumber() != fCurrent) {
    //         fCurrent = fChain->GetTreeNumber();
    //         Notify();
    //     }
    //     return centry;
    // }

    // Bool_t SimConverter::Notify() 
    // {
    //     // The Notify() function is called when a new file is opened. This
    //     // can be either for a new TTree in a TChain or when when a new TTree
    //     // is started when using PROOF. It is normally not necessary to make changes
    //     // to the generated code, but the routine can be extended by the
    //     // user if needed. The return value is currently not used.

    //     return kTRUE;
    // }

    // void SimConverter::Show(Long64_t entry) 
    // {
    //     // Print contents of entry.
    //     // If entry is not specified, print current entry
    //     if (!fChain) return;
    //     fChain->Show(entry);
    // }
#endif
