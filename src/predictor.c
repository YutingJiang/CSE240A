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

uint8_t* local_BHT;
uint8_t* global_BHT;
uint32_t* PHT;
int GHR;

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
  int size = pow(2, ghistoryBits);
  global_BHT = (uint8_t*)malloc(size * sizeof(uint8_t));
  int i = 0;
  while(i < size){
    global_BHT[i] = WN;
    i++;
  }
}

void
init_tournament()
{

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
  uint32_t filter = 0;
  int i = 0;
  while(i < ghistoryBits) {
    filter = (filter<<1) + 1;
    i++;
  }
  uint32_t res = (pc & filter) ^ (GHR & filter);
  if(global_BHT[res] < WT){
    return NOTTAKEN;
  } else {
    return TAKEN;
  }
}

uint8_t
predict_tournament(uint32_t pc)
{

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
  int i = 0;
  while(i < ghistoryBits) {
    filter = (filter<<1) + 1;
    i++;
  }
  uint32_t res = (pc & filter) ^ (GHR & filter);
  if(outcome == NOTTAKEN) {
    GHR = GHR<<1;
    if(global_BHT[res] > SN) {
      global_BHT[res]--;
    }
  } else {
    GHR = (GHR<<1) + 1;
    if(global_BHT[res] < ST) {
      global_BHT[res]++;
    }
  }
}

void
train_tournament(uint32_t pc, uint8_t outcome)
{

}

void
train_custom(uint32_t pc, uint8_t outcome)
{

}