//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu May 12 15:37:18 2022 by ROOT version 6.22/08
// from TTree DataTree/faster to tree root test : group2tree.C
// found on file: ../root/run110_merger.root
//////////////////////////////////////////////////////////

#ifndef DataTree_h
#define DataTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>



class DataTree : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderArray<unsigned char> label = {fReader, "label"};
   TTreeReaderArray<int> Q1 = {fReader, "nrj1"};
   TTreeReaderArray<int> Q2 = {fReader, "nrj2"};
   TTreeReaderArray<int> Q3 = {fReader, "nrj3"};
   TTreeReaderArray<int> Q4 = {fReader, "nrj4"};
   TTreeReaderArray<float> Q1_cal = {fReader, "nrj1_cal"};
   TTreeReaderArray<float> Q2_cal = {fReader, "nrj2_cal"};
   TTreeReaderArray<unsigned long long> time = {fReader, "time"};
   TTreeReaderValue<vector<bool>> pu = {fReader, "pileup"};
   TTreeReaderValue<UShort_t> multiplicity = {fReader, "multiplicity"};


   DataTree(TTree * /*tree*/ =0) { }
   virtual ~DataTree() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   void printProgBar( float);
   void SetOutfname(string fname) {outfname = fname;}
   
   ClassDef(DataTree,0);


   //declare your varaibles or histo here
   long nbEntries;
   int localentry = 0;
   string outfname;

   vector<double> gate_inf_LaBr3, gate_sup_LaBr3, gate_inf_NaI, gate_sup_NaI;
   vector<double> a_LaBr3, b_LaBr3;
   TFile * openfile;
   TTree * tout;
   TBranch *branch0,*branch1,*branch2,*branch3,*branch4,*branch5, *branch6, *branch7, *branch8, *branch9;
   vector<int> Q1_out;
   vector<int> Q2_out;
   vector<int> Q3_out;
   vector<int> Q4_out;
   vector<ULong64_t> time_out;
   vector<unsigned char> label_out;
   vector<bool> pu_out;
   UShort_t multiplicity_out;
   vector<float> Q1_cal_out;
   vector<float> Q2_cal_out;
   
   int adj_matrix[18][18] = {
     {1,1,0,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0},
     {1,1,1,1,1,1,0,0,0, 0,0,0,0,0,0,0,0,0},
     {0,1,1,0,1,1,0,0,0, 0,0,0,0,0,0,0,0,0},
     {1,1,0,1,1,0,1,1,0, 0,0,0,0,0,0,0,0,0},
     {1,1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,0},
     {0,1,1,0,1,1,0,1,1, 0,0,0,0,0,0,0,0,0},
     {0,0,0,1,1,0,1,1,0, 0,0,0,0,0,0,0,0,0},
     {0,0,0,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,0},
     {0,0,0,0,1,1,0,1,1, 0,0,0,0,0,0,0,0,0},
     
     {0,0,0,0,0,0,0,0,0, 1,1,0,1,1,0,0,0,0},
     {0,0,0,0,0,0,0,0,0, 1,1,1,1,1,1,0,0,0},
     {0,0,0,0,0,0,0,0,0, 0,1,1,0,1,1,0,0,0},
     {0,0,0,0,0,0,0,0,0, 1,1,0,1,1,0,1,1,0},
     {0,0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,1},
     {0,0,0,0,0,0,0,0,0, 0,1,1,0,1,1,0,1,1},
     {0,0,0,0,0,0,0,0,0, 0,0,0,1,1,0,1,1,0},
     {0,0,0,0,0,0,0,0,0, 0,0,0,1,1,1,1,1,1},
     {0,0,0,0,0,0,0,0,0, 0,0,0,0,1,1,0,1,1}
   };
   double time_window = 7e3;
};

#endif

#ifdef DataTree_cxx
void DataTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t DataTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef DataTree_cxx
