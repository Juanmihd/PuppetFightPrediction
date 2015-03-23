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

enum {_SIZE_SEQUENCE = 10, _NUM_ACTIONS = 13};

class SequenceInput{
  std::array<int, _SIZE_SEQUENCE> sequence;
  int top;
  std::size_t size;

  public:
    SequenceInput() : top(0), size(_SIZE_SEQUENCE - 1) {}
    SequenceInput(const std::array<int, _SIZE_SEQUENCE>& n_sequence) : sequence(n_sequence), top(0), size(_SIZE_SEQUENCE - 1) {}
    
    void new_input(int input){
      sequence[top] = input;
      if (++top >= _SIZE_SEQUENCE)
        top = 0;
    }

    void set_size(std::size_t n_size){
      size = n_size;
    }

    std::size_t get_size() const{
      return size;
    }

    std::size_t get_top() const{
      return top;
    }

    int get_element(std::size_t pos) const{
      return sequence[pos];
    }

    bool operator==(const SequenceInput& b){
      if (size != b.size) return false;

      std::size_t size_sequence = size;
      bool equal = true;
      for (int i = size_sequence - 1; equal && i >= 0; --i){
        //Obtain position in the array
        std::size_t pos_a = top - i;
        std::size_t pos_b = b.top - i;
        if (pos_a < 0) pos_a = _SIZE_SEQUENCE - pos_a;
        if (pos_b < 0) pos_b = _SIZE_SEQUENCE - pos_b;
        equal = sequence[pos_a] == b.sequence[pos_b];
      }

      return equal;
    }
};

template <class T>
class MyHash;

template <> 
class MyHash<SequenceInput>{
public:
  std::size_t operator()(SequenceInput const& s) const{
    std::size_t hash_key = 0;
    std::size_t size_sequence = s.get_size();
    for (int i = size_sequence-1; i >= 0; --i){
      //Obtain position in the array
      std::size_t pos = s.get_top() - i;
      if (pos < 0) pos = _SIZE_SEQUENCE - pos;
      hash_key = std::hash<int>()(hash_key) ^ (hash_key<<1);
    }
    return hash_key;
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