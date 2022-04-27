void RunSimConvert()
{
    gSystem->Load("libDelphes");
    gSystem->AddIncludePath("-I/home/niko/git/delphes/examples");

    gInterpreter->GenerateDictionary("vector","SimConverter.h;vector");
    gInterpreter->GenerateDictionary("vector<SimConverter::simple_vertex>","SimConverter.h;vector");
    gInterpreter->GenerateDictionary("vector<SimConverter::simple_track>","SimConverter.h;vector");
    gInterpreter->GenerateDictionary("vector<SimConverter::simple_particle>","SimConverter.h;vector");

    TString str;
    str = "./examples/SimConverter.C+";
    gROOT->LoadMacro(str);
    
    str = "SimConverter c";
    gROOT->ProcessLine(str);

    str = Form("c.Loop()");
    gROOT->ProcessLine(str);

}