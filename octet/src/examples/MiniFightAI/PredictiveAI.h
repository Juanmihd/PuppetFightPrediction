//
// This is the clas with the container of the predictive AI
// This will be highly parametrizable to be able to use for different kind of applications
// It will be able to gather inputs, and produce a predicted inpute
//
// It will be needed to append a "decision" in case that it's needed to perform any action to solve the predicted input
//

#ifndef _PREDICTIVEAI_H_
#define _PREDICTIVEAI_H_

#include <unordered_map>

class PredictiveAI{
  unsigned int num_actions;
  unsigned int dimension_nGram;
  unsigned int cur_input;
  std::unordered_map<unsigned int, unsigned int> nGram;

public:
  PredictiveAI() : num_actions(0) {}

  PredictiveAI(int n_num_actions, int n_dimension_nGram) : num_actions(n_num_actions), dimension_nGram(n_dimension_nGram){}

  void new_input(unsigned int input){

  }
};


#endif