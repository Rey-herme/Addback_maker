#include "DataTree.h"


void MakeAddback(string fname){
	
	ifstream files("filename.dat", ios::in | ios::binary); ///open the file containing the path to the data files...
  TChain *tin = new TChain("DataTree","Datatree");
  
	string line;
  while(files.good()){ ///... read filename file
    
    getline(files, line); ///read one line of the file and save it in the string "line", go to the next line
    
    if (line.empty()) continue; ///if the line is empty or not convertible, continue
    
    if (line.find("#") != string::npos) continue; ///# at the beginning of the line => ignore it
    
    tin->Add(line.c_str()); //Add the file to the chain
  }
  
  files.close();
	

	long entries = tin->GetEntries();
	
	DataTree *selector = (DataTree*)TSelector::GetSelector("DataTree.C");
	selector->SetOutfname(fname);
	tin->Process(selector,to_string(entries).c_str());
		

	return;

}
