#include "fitter.h"

int bruteforce(std::string xml, int massStart = -1){

  bool debug = false;

  // Read our XML file
  FitReader rdr;
  if(rdr.Load(xml))
    return 0;
  Oscillator osc = rdr.GetOscillator();

  // Initialize datasets
  std::cout << "Initializing " << rdr.GetNDatasets() << " datasets!" << std::endl;
  int ndf = 0;
  //std::string dataLoc = "/home/dcianci/Physics/GlobalFits/SBN_3plusN/GlobalFits_v2/data/";
  std::string dataLoc = "../../data/";
  for(int i = 0; i < rdr.GetNDatasets(); i++){
    ndf += rdr.GetDataset(i)->Init(dataLoc,osc,debug);
  }
  std::cout << "Datasets Initialized!" << std::endl;

  // Create output File
  std::string outfile = "brute.root";
	std::cout << "Output File: " << outfile << std::endl;
	TString outputFile = outfile;
	TFile *f = new TFile(outputFile, "RECREATE");
	if(f->IsZombie()){
		std::cout << "Error: couldn't create output file." << std::endl;
		return 0;
	}

  OutTree * chi2Nt = new OutTree("Total");

  // Create a nu model
  //neutrinoModel nuModel;

  int count = 0;
  int grdpts = osc.GridSize();
  float chi2;
  std::cout << "Beginning chi2 loop" << std::endl;
  int mStart, mEnd;
  if(massStart < 0){
    mStart = 0; mEnd = grdpts;
  }
  else{
    mStart = massStart; mEnd = massStart + 1;
  }
  for(int mi = mStart; mi < mEnd; mi++){
    for(int uei = 0; uei < grdpts; uei++){
      for(int umi = 0; umi < grdpts; umi++){
        std::cout << "Progress: " << float(count)/(pow(grdpts,2)*(mEnd-mStart)/(100.f)) << "\% \r";

        neutrinoModel nuModel;
		    nuModel.zero();
        nuModel.Ue[0] = pow(10,(uei/float(grdpts)*TMath::Log10(1/1e-3) + TMath::Log10(1e-3)));
        nuModel.Um[0] = pow(10,(umi/float(grdpts)*TMath::Log10(1./1e-3) + TMath::Log10(1e-3)));
		    nuModel.mNu[0] = pow(10,(mi/float(grdpts)*TMath::Log10(10./.1) + TMath::Log10(.1)));
        //if(osc.RejectModel(nuModel))
        //  continue;

        // Calculate chi2s
        chi2 = 0;
        for(int i = 0; i < rdr.GetNDatasets(); i++){
          chi2 += rdr.GetDataset(i)->Chi2(osc,nuModel,debug);
        }

        chi2Nt->Fill(chi2,ndf,nuModel);
        count ++;
      }
    }
  }

  // Write everything to File
  std::cout << "Writing to file." << std::endl;
  chi2Nt->Write();

  for(int i = 0; i < rdr.GetNDatasets(); i++){
    rdr.GetDataset(i)->Write();
  }

  f->Close();

  return 0;
}

int main(int argc, char* argv[]){

  std::string xml = "";
  int iarg = 0;
  opterr=1;
  int index;
  int massStart = -1;

  const struct option longopts[] = {
    {"xml", 		required_argument, 	0, 'x'},
	  {0,			no_argument, 		0,  0},
  };

  while(iarg != -1){
    iarg = getopt_long(argc,argv, "x:t:", longopts, &index);

    switch(iarg){
		  case 'x':
			  xml = optarg;
			  break;
      case 't':
        massStart = atoi(optarg);
        break;
      case '?':
		  case 'h':
			  std::cout<<"I need an input, friend."<<std::endl;
			  std::cout<<"\t-x\t--xml\t\tInput .xml file for SBNconfig"<<std::endl;
			  return 0;
	  }
  }
  if(xml == ""){
    std::cout << "Gimme an XML input or I won't start, I swear to god." << std::endl;
    return 0;
  }

  bruteforce(xml,massStart);
  return 0;
}
