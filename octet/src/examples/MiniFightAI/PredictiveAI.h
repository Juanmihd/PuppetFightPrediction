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
#include <array>

enum {SIZE_SEQUENCE = 4, NUM_ACTIONS = 13};

class SequenceInput{
  std::array<int, SIZE_SEQUENCE> sequence;
  int top;

  public:
    SequenceInput() : sequence( {{ -1, -1, -1, -1 }}), top(0) {}
    SequenceInput(const std::array<int, 4>& n_sequence) : sequence(n_sequence), top(0) {}
    
    void new_input(int input){
      sequence[top] = input;
      if (++top > SIZE_SEQUENCE)
        top = 0;
    }
};

template <class T>
class MyHash;

template <> 
class MyHash<SequenceInput>{
public:
  std::size_t operator()(SequenceInput const& s) const{

  }
};

class PredictiveAI{
  std::size_t dimension_nGram;
  // The whole nGram representation is stored in a Hash table.
  std::unordered_map<SequenceInput, unsigned int> nGram;

public:
  PredictiveAI() : dimension_nGram(1){}

  PredictiveAI(std::size_t n_dimension_nGram) : dimension_nGram(n_dimension_nGram){}

  void new_input(std::size_t input){

  }
};


#endif