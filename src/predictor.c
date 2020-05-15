//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include "predictor.h"
#include <math.h>

//
// TODO:Student Information
//
const char *studentName = "Yuting Jiang, Yingzhen Qu";
const char *studentID   = "A53298624, ";
const char *email       = "yuj010@eng.ucsd.edu,";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

//
//TODO: Add your own Branch Predictor data structures here
//
int GHR;//global history register
uint8_t* local_BHT;//local branch history table
uint8_t* global_BHT;//global branch history table
uint32_t* PHT;//pattern history table
uint8_t* selector;//branch predictor selector


//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
void
init_predictor()
{
  //
  //TODO: Initialize Branch Predictor Data Structures
  //
  switch (bpType) {
    case STATIC:
      break;
    case GSHARE:
      init_gshare();
      break;
    case TOURNAMENT:
      init_tournament();
      break;
    case CUSTOM:
      init_custom();
      break;
    default:
      break;
  }
}

void
init_gshare()
{
  GHR = 0;
  int global_size = pow(2, ghistoryBits);//initialize global branch history size
  global_BHT = (uint8_t*)malloc(global_size * sizeof(uint8_t));//initialize global branch history table
  for(int i = 0; i < global_size; i++){
    global_BHT[i] = WN;//initialize all the prediction to be Weak NotTaken
  }
}

void
init_tournament()
{
  GHR = 0;
  int local_size = pow(2, lhistoryBits);//initialize local branch history table size
  int global_size = pow(2, ghistoryBits);//initialize global branch history table  size
  int pht_size = pow(2, pcIndexBits);//initialize pattern history table size
  int selector_size = global_size;
  local_BHT = (uint8_t*)malloc(local_size * sizeof(uint8_t));//initialize local branch history table
  global_BHT = (uint8_t*)malloc(global_size * sizeof(uint8_t));//initialize global branch history table
  PHT = (uint32_t*)malloc(pht_size * sizeof(uint32_t));//initialize pattern history  table
  selector = (uint8_t*)malloc(selector_size * sizeof(uint8_t));//initialize predictor selector table
  for(int i = 0; i < global_size; i++){
    global_BHT[i] = WN;//initialize all the global prediction to be Weak NotTaken
  }
  for(int i = 0; i < local_size; i++){
    local_BHT[i] = WN;//initialize all the local prediction to be Weak NotTaken
  }
  for(int i = 0; i < pht_size; i++){
    PHT[i] = 0;//initialize all pattern history to be 0
  }
  for(int i = 0; i < selector_size; i++) {
    selector[i] = 2;//0 for strongly local, 1 for weakly local, 2 for weakly global, 3 for strongly global
  }
}

void init_custom()
{

}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
  //
  //TODO: Implement prediction scheme
  //

  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
      return predict_gshare(pc);
    case TOURNAMENT:
      return predict_tournament(pc);
    case CUSTOM:
      return predict_custom(pc);
    default:
      break;
  }
  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

uint8_t
predict_gshare(uint32_t pc)
{
  uint32_t filter = 0;//filter to compensate the length difference
  int i = 0;
  while(i < ghistoryBits) {
    filter = (filter<<1) + 1;//initialize filter
    i++;
  }
  uint32_t res = (pc & filter) ^ (GHR & filter);
  if(global_BHT[res] > WN){
    return TAKEN;
  } else {
    return NOTTAKEN;
  }
}

uint8_t
predict_tournament(uint32_t pc)
{
  uint32_t filter = 0;//filter to compensate the length difference
  for(int i = 0; i < pcIndexBits; i++) {
    filter = (filter<<1) + 1;//initialize the filter
  }
  int res = selector[GHR];// check the selection
  if(res <2){   //choose local predictor
    int val_PHT = PHT[(pc & filter)];
    int local_prediction = local_BHT[val_PHT];
    if(local_prediction < WT) return NOTTAKEN;
    else return TAKEN;
  } else { //choose global predictor
    int global_prediction = global_BHT[GHR];
    if(global_prediction < WT) return NOTTAKEN;
    else return TAKEN;
  }
}

uint8_t
predict_custom(uint32_t pc)
{

}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void
train_predictor(uint32_t pc, uint8_t outcome)
{
  //
  //TODO: Implement Predictor training
  //
  switch (bpType) {
    case STATIC:
      break;
    case GSHARE:
      train_gshare(pc, outcome);
      break;
    case TOURNAMENT:
      train_tournament(pc, outcome);
      break;
    case CUSTOM:
      train_custom(pc, outcome);
      break;
    default:
      break;
  }
}

void
train_gshare(uint32_t pc, uint8_t outcome)
{
  uint32_t filter = 0;
  for(int i = 0; i < ghistoryBits; i ++) {
    filter = (filter<<1) + 1;
  }
  uint32_t res = (pc & filter) ^ (GHR & filter);
  if(outcome == TAKEN) {
    if(global_BHT[res] < ST) {
      global_BHT[res]++;
    }
    GHR = ((GHR<<1) + 1) & filter;
  } else {
    if(global_BHT[res] > SN) {
      global_BHT[res]--;
    }
    GHR = (GHR<<1) & filter;
  }
}

void
train_tournament(uint32_t pc, uint8_t outcome)
{
  uint32_t filter_pc = 0;// filter for pc to compensate length
  uint32_t filter_pht= 0;// filter for PHT to compensate length
  uint32_t filter_ghr = 0;// filter for GHT to compensate length
  for(int i = 0; i < pcIndexBits; i ++) {
    filter_pc = (filter_pc<<1) + 1;
  }
  for(int i = 0; i < lhistoryBits; i ++) {
    filter_pht = (filter_pht<<1) + 1;
  }
  for(int i = 0; i < ghistoryBits; i ++) {
    filter_ghr = (filter_ghr<<1) + 1;
  }
  int val_PHT = PHT[(pc & filter_pc)];
  int local_prediction = local_BHT[val_PHT];
  int global_prediction = global_BHT[GHR];
  int res = selector[GHR];
  int local_result = NOTTAKEN, global_result = NOTTAKEN;// initial local and global result
  if(local_prediction > WN) local_result = TAKEN;// check local result
  if(global_prediction > WN) global_result = TAKEN;// check global result
  if(outcome == TAKEN) {
    if(global_result != local_result) { //if two predictor makes different prediction, prefer the correct one
      if(local_prediction > WN && res > 0) selector[GHR]--;// if only local predictor is right, selector prefers it
      else if(global_prediction > WN && res < 3) selector[GHR]++;// if only global predictor is right, selector prefers it
    } 
    if(local_prediction < ST) local_BHT[val_PHT]++; // update local BHT
    if(global_prediction < ST) global_BHT[GHR]++;// update global BHT
    PHT[(pc & filter_pc)] = ((val_PHT << 1)+1) & filter_pht;// update local pattern history table
    GHR = ((GHR << 1)+1) & filter_ghr; // update global history register
  } else {
    if(global_result != local_result) { //if two predictor makes different prediction, prefer the correct one
      if(local_prediction < WT && res > 0) selector[GHR]--;// if only local predictor is right, selector prefers it
      else if(global_prediction < WT && res < 3) selector[GHR]++;// if only global predictor is right, selector prefers it
    } 
    if(local_prediction > SN) local_BHT[val_PHT]--; // update local BHT
    if(global_prediction > SN) global_BHT[GHR]--;// update global BHT
    PHT[(pc & filter_pc)] = (val_PHT << 1) & filter_pht;// update local pattern history table
    GHR = (GHR << 1) & filter_ghr; // update global history register
  }
}

void
train_custom(uint32_t pc, uint8_t outcome)
{

}
