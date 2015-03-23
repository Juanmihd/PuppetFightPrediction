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

enum {_SIZE_SEQUENCE = 10};

class SequenceInput{
  std::array<int, _SIZE_SEQUENCE> sequence;
  int top;
  std::size_t size;

  public:
    SequenceInput() : top(0), size(1) {}
    SequenceInput(const std::array<int, _SIZE_SEQUENCE>& n_sequence) : sequence(n_sequence), top(0), size(_SIZE_SEQUENCE - 1) {}
    
    void new_input(int input){
      sequence[top] = input;
      if (++top > _SIZE_SEQUENCE-1)
        top = 0;
    }

    void set_size(std::size_t n_size){
      size = n_size;
    }

    std::size_t get_size() const{
      return size;
    }

    void grow_sequence(){
      ++size;
    }

    std::size_t get_top() const{
      return top;
    }

    int get_element(std::size_t pos) const{
      int n_pos = top - 1 - pos;
      if (n_pos < 0) 
        n_pos = _SIZE_SEQUENCE + n_pos;
      return sequence[n_pos];
    }

    void print_sequence(){
      for (int i = size - 1; i > 0; --i){
        //Obtain position in the array
        std::size_t pos = top - i;
        if (pos < 0) pos = _SIZE_SEQUENCE - pos;
        printf("%i ", sequence[pos]);
      }
      printf("\n");
    }

    void print_debug_all(){
      printf("Sequence: top %i and size %i\n", top, size);
      for (const int& i : sequence){
        printf("%i ", i);
      }
      printf("\n");
    }
};


bool operator==(const SequenceInput& a, const SequenceInput& b){
  if (a.get_size() != b.get_size()) return false;
  
  std::size_t size_sequence = a.get_size();
  bool equal = true;
  for (int i = 0; equal && i < size_sequence; ++i){
    equal = a.get_element(i) == b.get_element(i);
  }

  return equal;
}

bool operator!=(const SequenceInput& a, const SequenceInput& b){
  return !(a == b);
}

template <class T>
class MyHash;

template <> 
class MyHash<SequenceInput>{
public:
  std::size_t operator()(SequenceInput const& s) const{
    std::size_t hash_key = 0;
    std::size_t size_sequence = s.get_size();
    for (int i = size_sequence-1; i > 0; --i){
      //Obtain position in the array
      std::size_t pos = s.get_top() - 1 - i;
      if (pos < 0) pos = _SIZE_SEQUENCE + pos;
      hash_key = std::hash<int>()(hash_key) ^ (hash_key<<1);
    }
    return hash_key;
  }
};

class PredictiveAI{
  std::size_t dimension_nGram;
  SequenceInput cur_sequence;
  // The 1Gram is an array
  std::array<int, 15> oneGram;
  // The rest of the nGram representation is stored in a Hash table.
  std::unordered_map<SequenceInput, unsigned int, MyHash<SequenceInput>> nGram;

  void reset_n_gram(){
    for (int i = 0; i < 15; ++i){
      oneGram[i] = 0;
    }
  }

public:
  PredictiveAI() : dimension_nGram(1){
    reset_n_gram();
  }

  PredictiveAI(std::size_t n_dimension_nGram) : dimension_nGram(n_dimension_nGram){
    reset_n_gram();
  }

  void init(std::size_t size){
    cur_sequence.set_size(4);
    cur_sequence.new_input(0);
    cur_sequence.new_input(0);
    cur_sequence.new_input(0);
    cur_sequence.new_input(0);
  }

  void new_input(std::size_t input){
    cur_sequence.new_input(input);
    int temp_size = cur_sequence.get_size();
    //n-gram
    for (int i = 2; i <= 4; ++i){
      cur_sequence.set_size(i);
      auto search = nGram.find(cur_sequence);
      if (search == nGram.end()){
        nGram[cur_sequence] = 1;
      }
      else{
        ++nGram[cur_sequence];
      }
    }
    //1-gram
    ++oneGram[input];

    cur_sequence.set_size(temp_size);
  }

  int get_last(){
    return cur_sequence.get_element(0);
  }

  int predict(){

  }

};


#endif