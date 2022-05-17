#define DataTree_cxx
// The class definition in DataTree.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("DataTree.C")
// root> T->Process("DataTree.C","some options")
// root> T->Process("DataTree.C+")
//


#include "DataTree.h"
#include <TH2.h>
#include <TStyle.h>

void DataTree::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   ifstream PSDin("PSD_gate.txt", ios::in | ios::binary); ///open the file containing PSD gates ...
   
   string line;
   stringstream linestream;
   int det, i=0;
   double temp1, temp2, temp3, temp4, temp5; ///definition of temporary variables
   
   while(PSDin.good()){ ///... read PSD_gates file
     
     getline(PSDin, line); ///read one line of the file and save it in the string "line", go to the next line
     linestream.clear();  ///clear the object "linestream"
     linestream.str(line); ///convert the string into streamstring
     
     if (linestream.fail() or line.empty()) continue; ///if the line is empty or not convertible, continue
     
     if (line.find("#") != string::npos) continue; ///# at the beginning of the line => ignore it
     
     linestream >> temp1 >> temp2 >> temp3 >> temp4 >> temp5; ///each object of the line (here 5 : see the PSD_gates.txt) is stored in a temporary variable...
     gate_inf_LaBr3.push_back(temp2); ///... each temporary variable is stored in the right place of the list containing the gates.
     gate_sup_LaBr3.push_back(temp3);
     gate_inf_NaI.push_back(temp4);
     gate_sup_NaI.push_back(temp5);
   }
   
   PSDin.close();
   
   
   ifstream Calibin("Calibration_Coef.txt", ios::in | ios::binary); ///open the file containing the calibration coefficients
   
   
   
   int rightline = 0;
   while(Calibin.good()){ ///... read read the calibration file
     string temp6;
     getline(Calibin, line); ///read one line of the file and save it in the string "line", go to the next line
     linestream.clear();  ///clear the object "linestream"
     linestream.str(line); ///convert the string into streamstring
     
     if (linestream.fail() or line.empty()) continue; ///if the line is empty or not convertible, continue
     
     if (line.find("#") != string::npos) continue; ///# at the beginning of the line => ignore it
     
     if(rightline%2 != 0 or rightline>34){
       rightline++;
       continue;
     }
     rightline++;
     linestream >> temp6 >> temp2 >> temp3; ///each object of the line is stored in a temporary variable...
     b_LaBr3.push_back(temp2);
     a_LaBr3.push_back(temp3);
   }
   
   Calibin.close();
   

   
   openfile = new TFile(outfname.c_str(),"RECREATE");
   tout = new TTree("DataTree","DataTree");
   
   branch0 = tout->Branch("label",&label_out);
   branch1 = tout->Branch("nrj1",&Q1_out);
   branch2 = tout->Branch("nrj2",&Q2_out);
   branch3 = tout->Branch("nrj3",&Q3_out);
   branch4 = tout->Branch("nrj4",&Q4_out);
   branch5 = tout->Branch("time",&time_out);
   branch6 = tout->Branch("pileup",&pu_out);
   branch7 = tout->Branch("multiplicity",&multiplicity_out);
   branch8 = tout->Branch("nrj1_cal",&Q1_cal_out);
   branch9 = tout->Branch("nrj2_cal",&Q2_cal_out);
   
   TString option = GetOption();
   nbEntries = std::stol(option.Data());

}

void DataTree::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t DataTree::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   localentry++;

   fReader.SetLocalEntry(entry);

   if(entry%1000 == 0) printProgBar(localentry*100.0/nbEntries);
   
   
   label_out.clear();
   Q1_out.clear();
   Q2_out.clear();
   Q3_out.clear();
   Q4_out.clear();
   time_out.clear();
   pu_out.clear();
   Q1_cal_out.clear();
   Q2_cal_out.clear();
   multiplicity_out = 0;
   
   
   int physical_multiplicity = 0; // Hold the multiplicity after AddBack
   
   int processed_events[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Hold if a given event was already treated
   
   
   // Start of Add-back procedure
   for(int i=0 ; i < (int)*multiplicity ; i++){ //Addback loop
     
     if(label[i]>20){
       label_out.push_back(label[i]);
       Q1_out.push_back(Q1[i]);
       Q2_out.push_back(Q2[i]);
       Q3_out.push_back(Q3[i]);
       Q4_out.push_back(Q4[i]);
       time_out.push_back(time[i]);
       pu_out.push_back(pu->at(i));
       Q1_cal_out.push_back(Q1_cal[i]);
       Q2_cal_out.push_back(Q2_cal[i]);
       multiplicity_out++;
     }
     else if(processed_events[i]==0){ // If the event was already processed, continue
       
       processed_events[i] = 1; // Otherwise, process the event
       float temp_e = 0;
       float physical_E = 0;
       int physical_i = i;
       
       unsigned char physical_label = label[i]; // For now the physical label
       ULong64_t physical_time = time[i]; // For now the physical time
       double ratio = (double(Q2[i]) - double(Q1[i]))/double(Q1[i]); // Paris psd
       
       if (ratio > gate_inf_LaBr3[label[i] - 1] and ratio < gate_sup_LaBr3[label[i] - 1]){ // if LaBr3 event, use LaBr3 calibration
         
         //physical_E = (float)Q1_cal[i]; // If the data are already properly calibrated, use this
         physical_E = (float)Q1[i]*a_LaBr3[label[i]-1]+b_LaBr3[label[i]-1]; // Use the calibration in the calibration file
         temp_e=physical_E; // Hold the highest energy deposit
         physical_i = i;
       }
       else if (ratio > gate_inf_NaI[label[i] - 1] and ratio < gate_sup_NaI[label[i] - 1]){ // Same as before be for NaI
         physical_E = (float)Q2_cal[i];
         temp_e=physical_E;
         physical_i = i;
       }
       
       for(int j=i+1 ; j<(int)*multiplicity ; j++){ // Check if another event should be added by the addback procedure
         
         if(label[j]<19){ // Monster detectors are not to be considered
           if (processed_events[j] == 0){ // If the event was already processed, it should not be considered
             
             double delta_t = abs(double(time[i]) - double(time[j])); // Compute the time difference between the two events
             
             if(delta_t<time_window and adj_matrix[label[i] - 1][label[j] - 1]!=0){ // If the event is within the time window and the detectors are neighbors, the events should be added
               
               processed_events[j] = 1; // The event is now processed
               ratio = (double(Q2[j]) - double(Q1[j]))/double(Q1[j]); // Compute PSD for Paris
               
               if (ratio > gate_inf_LaBr3[label[j] - 1] and ratio < gate_sup_LaBr3[label[j] - 1]){ // If LaBr3 event
                 //physical_E += (float)Q1_cal[j];
                 physical_E += (float)Q1[j]*a_LaBr3[label[j]-1]+b_LaBr3[label[j]-1]; // Add the energy to the previous one
                 if(temp_e < (float)Q1_cal[j]){ //If the second energy deposit is higher than the first one, take the second as the physical event
                   physical_label = label[j];
                   physical_time = time[j];
                   physical_i = j;
                   //temp_e = (float)Q1_cal[j];
                   temp_e = (float)Q1[j]*a_LaBr3[label[j]-1]+b_LaBr3[label[j]-1];
                 }
               }
               else if (ratio > gate_inf_NaI[label[j] - 1] and ratio < gate_sup_NaI[label[j] - 1]){ // If NaI event
                 physical_E += (float)Q2_cal[j]; // Add the energy to the previous one
                 if(temp_e < (float)Q2_cal[j]){ //If the second energy deposit is higher than the first one, take the second as the physical event
                   physical_label = label[j];
                   physical_time = time[j];
                   temp_e = (float)Q2_cal[j];
                   physical_i = j;
                 }
               }
             }
           }
         }
       }
       if(physical_E>0){ // If the physical energy is not zero, add the event to the list of events
         label_out.push_back(physical_label);
         Q1_out.push_back(-123456);
         Q2_out.push_back(-123456);
         Q3_out.push_back(-123456);
         Q4_out.push_back(-123456);
         time_out.push_back(physical_time);
         pu_out.push_back(pu->at(physical_i));
         Q1_cal_out.push_back(-123456);
         Q2_cal_out.push_back(physical_E);
         multiplicity_out++;
       }
     } 
     // End of Add-back procedure
   }
   tout->Fill();

   return kTRUE;
}

void DataTree::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void DataTree::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   cout << endl;
   tout->AutoSave();
   openfile->Close();
   
}


void DataTree::printProgBar( float percent){
  std::string bar;

  for(int i = 0; i < 50; i++){
    if( i < (int(percent)/2)){
      bar.replace(i,1,"=");
    }else if( i == int(percent/2)){
      bar.replace(i,1,">");
    }else{
      bar.replace(i,1," ");
    }
  }



  std::cout<<"\r [" <<  bar  <<"]";
  std::cout.width( 3 );
  std::cout<< std::fixed;
  std::cout<< std::setprecision(2);

  std::cout<< percent  << "%"<< std::flush;
}