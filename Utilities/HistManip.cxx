#ifndef KALEKOANA_UTILITIES_HISTMANIP_CXX
#define KALEKOANA_UTILITIES_HISTMANIP_CXX

#include "HistManip.h"

namespace lee{
  namespace util{
  
  bool HistManip::CheckBins(const TH1F * const hist, const std::vector<double> *nbins){
    
    bool debug_me = false;
    
    //Check to make sure each of the bin low boundaries listed in nbins
    //matches with a bin already in hist
    
    for (Int_t inew = 0; inew < nbins->size(); ++inew){
      if(debug_me) std::cout<<"loop over vector provided. checking "<<nbins->at(inew)<<std::endl;
      //Loop over bins in histogram
      Int_t iold = 0;
      while(true){     
	if(debug_me) std::cout<<"\tloop over histo provided. bin edge: "<<hist->GetXaxis()->GetBinLowEdge(iold)<<std::endl;
	//0.000001 is because comparing doubles with "==" sucks.
	if(nbins->at(inew) - hist->GetXaxis()->GetBinLowEdge(iold) < 0.000001){
	  if(debug_me) std::cout<<"\t\tfound match."<<std::endl;
	  break;
	}
	
	iold++;
	
	//+2 is to cover for underflow and overflow bin boundaries
	if( iold == hist->GetNbinsX()+2 ){
	  
	  std::string msg = "";
	  msg += "Problem with Check Bins. No match found! Here's debug info:\n";
	  msg += "\tInput histogram bins: [";
	  for(int david = 0; david < hist->GetNbinsX()+2; david++)
	    msg += std::to_string(hist->GetXaxis()->GetBinLowEdge(david)) + ",";
	  msg += "]\n";
	  msg += "\tInput new bins: [";
	  for(size_t david = 0; david < nbins->size(); david++)
	    msg += std::to_string(nbins->at(david)) + ",";
	  msg += "]\n";
	  msg += "\t\tUnable to rebin because one of the input new bins does not match ";
	  msg += "any of the input histogram bin low edge boundaries.\n";
	  print(larlite::msg::kERROR, __FUNCTION__, msg);
	  return false;
	  
	}
      }
      
    }
    
    return true;
  }
  
  THStack* HistManip::AddTH1FToStack(TH1F * const hist, THStack * const stack){
    
    THStack *result = new THStack();
    
    //Make sure hist binning matches the binning of (any, so 0th) hist in stack
    //(since all hists in the stack have same binning)
    std::vector<double> *histbins = new std::vector<double>;
    for(size_t i = 0; i < hist->GetNbinsX(); i++)
      histbins->push_back(hist->GetXaxis()->GetBinLowEdge(i));
    
    TH1F* blah;
    if((TH1F*)stack->GetHists())
      blah = (TH1F*)stack->GetHists()->At(0);
    else{
      blah = (TH1F*)hist->Clone();
    }
    //      std::string msg = "";
    //      msg += "Stack provided to AddTH1FToStack is empty.";
    //      print(larlite::msg::kERROR, __FUNCTION__, msg);
    //      return result;
    //    }
    
    if(!CheckBins(blah,histbins)){
      std::string msg = "";
      msg += "Stack provided has binning problem.";
      print(larlite::msg::kERROR, __FUNCTION__, msg);
      return result;
    }
    
    result = stack;
    result->Add(hist);
    return result;
    
  }
  
  
  
  TH1F* HistManip::RebinTH1F(TH1F * const hist, const std::vector<double> *newbins){
    
    TH1F *result = hist;
    
    //Check to make sure newbins are all integer multiples of oringal hist bins
    //otherwise TH1F::Rebin does funky stuff.
    if( !CheckBins(hist,newbins) ){
      std::string msg = "";
      msg += "TH1F provided provided has binning problem.";
      print(larlite::msg::kERROR, __FUNCTION__, msg);
      return hist;
    }
    
    size_t nbins = newbins->size();
    Double_t xbins[nbins];
    for (Int_t i = 0; i < nbins; i++)
      xbins[i]=newbins->at(i);
    
    return (TH1F*)result->Rebin(nbins-1,"tmp",xbins);
    
  }
  
  THStack* HistManip::RebinStack(const THStack *stack, const std::vector<double> *newbins){
    
    THStack *result = new THStack();
    
    //Loop over the histograms in the stack, check each has compatible bins
    for(size_t i = 0; i < stack->GetHists()->GetSize(); i++){
      if( !CheckBins((const TH1F*)stack->GetHists()->At(i),newbins) ){
	std::string msg = "";
	msg += "Problem with the binning of histogram " + std::to_string(i);
	msg += "in the stack. Returning null result stack.";
	print(larlite::msg::kERROR, __FUNCTION__, msg);
	return result;
      }
      
      result->Add(RebinTH1F((TH1F*)stack->GetHists()->At(i),newbins));
    }
    
    return result;
  }
  
  
  void HistManip::ConvertToEventsPerBinWidth(  TH1F & hist ){
    
    /// Input histogram is in terms of raw events
    
    /// Loop over bins and divide by bin width
    for (size_t i = 0; i < hist.GetNbinsX(); ++i){
      double new_contents = hist.GetBinContent(i) /
	hist.GetBinWidth(i);
      hist.SetBinContent(i,new_contents);
    }
    
    /// Rename y-axis
    hist.GetYaxis()->SetTitle("Events per Bin Width");
    
  }
  
  void HistManip::ConvertToEvents( TH1F & hist ){
    
    /// Loop over bins and multiply by bin width
    for (size_t i = 0; i < hist.GetNbinsX(); ++i){
      double new_contents = hist.GetBinContent(i) *
	hist.GetBinWidth(i);
      hist.SetBinContent(i,new_contents);
    }
    
    /// Rename y-axis
    hist.GetYaxis()->SetTitle("Events");
    
  }
  
  THStack* HistManip::ConvertToEvents( THStack & stack ){
    
    THStack* result = new THStack();
    
    /// Loop over the histograms in the stack, check each has compatible bins
    for(size_t i = 0; i < stack.GetHists()->GetSize(); i++){
      
      /// Convert each histogram in stack
      ConvertToEvents( *(TH1F*)(stack.GetHists()->At(i)) );
      
      /// Add to new stack
      result->Add( (TH1F*)(stack.GetHists()->At(i)) );
      
    }
    
    return result;
  }
  
  void HistManip::SetZeroErrors( TH1F & hist ){
    
    for (size_t i = 0; i < hist.GetNbinsX(); ++i)
      hist.SetBinError(i,0.);
    
  }   
}
}
#endif
