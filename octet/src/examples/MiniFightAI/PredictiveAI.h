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
#include <vector>

namespace octet{
  namespace PuppetFight{
    enum {_SIZE_SEQUENCE = 5, _NUM_ACTIONS = 9, _DEBUGGING = 1};

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

        void set_top(int value){
          if (top != 0)
            sequence[top - 1] = value;
          else
            sequence[_SIZE_SEQUENCE - 1] = value;
        }

        int get_element(std::size_t pos) const{
          int n_pos = top - 1 - pos;
          if (n_pos < 0) 
            n_pos = _SIZE_SEQUENCE + n_pos;
          return sequence[n_pos];
        }

        void print_sequence(){
          printf("Sequence: size %i: ", size);
          for (int i = 0; i < size; ++i){
            //Obtain position in the array
            int pos = top - 1 - i;
            if (pos < 0) 
              pos = _SIZE_SEQUENCE + pos;
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
        for (int i = size_sequence-1; i >= 0; --i){
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
      std::size_t treshold;
      SequenceInput cur_sequence;
      // The 1Gram is an array
      std::array<int, 15> oneGram;
      // The rest of the nGram representation is stored in a Hash table.
      std::unordered_map<SequenceInput, unsigned int, MyHash<SequenceInput>> nGram;
      // The rest of the nGram representation is stored in a Hash table.
      std::unordered_map<SequenceInput, unsigned int, MyHash<SequenceInput>> nGram_sums;

      void reset_n_gram(){
        for (int i = 0; i < oneGram.size(); ++i){
          oneGram[i] = 0;
        }
      }

    public:
      PredictiveAI() : dimension_nGram(4), treshold(6){
        reset_n_gram();
      }

      PredictiveAI(std::size_t n_dimension_nGram) : dimension_nGram(n_dimension_nGram), treshold(6){
        reset_n_gram();
      }

      void init(std::size_t size){
        cur_sequence.set_size(4);
        cur_sequence.new_input(0);
        cur_sequence.new_input(0);
        cur_sequence.new_input(0);
        cur_sequence.new_input(0);
      }

      void set_dimension(std::size_t n_dimension){
        dimension_nGram = n_dimension;
      }

      void resetAI(){
        reset_n_gram();
        nGram.clear();
        nGram_sums.clear();
        cur_sequence.new_input(0);
        cur_sequence.new_input(0);
        cur_sequence.new_input(0);
        cur_sequence.new_input(0);
      }

      void new_input(std::size_t input){
        int temp_size = cur_sequence.get_size();
        //n-gram-sums! (note the < instead of <= )
        for (int i = 1; i < dimension_nGram; ++i){
          cur_sequence.set_size(i);
          auto search = nGram_sums.find(cur_sequence);
          if (search == nGram_sums.end()){
            nGram_sums[cur_sequence] = 1;
          }
          else{
            ++nGram_sums[cur_sequence];
          }
        }
        cur_sequence.new_input(input);
        //n-gram (note the <= instead of < )
        for (int i = 2; i <= dimension_nGram; ++i){
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
        //Debugging
        if (_DEBUGGING == 0){
          printf("\n");
          cur_sequence.print_sequence();
        }
        
        int prediction = -1;
        int temp_size = cur_sequence.get_size();
        //check n-gram
        if (_DEBUGGING == 0) printf("Trying with... ");
        for (int i = dimension_nGram; i > 1 && prediction == -1; --i){
          if (_DEBUGGING == 0) printf("%i ... ", i - 1);
          cur_sequence.set_size(i-1);
          if (nGram_sums[cur_sequence] > treshold){
            prediction = i-1;
          }
        }
        if (_DEBUGGING == 0) printf("\n");
        if (prediction != -1){
          if (_DEBUGGING == 0) printf("Predicting with nGram %i", prediction + 1);
          cur_sequence.set_size(cur_sequence.get_size() + 1);
          SequenceInput tempSequence(cur_sequence);
          tempSequence.new_input(0);
          std::size_t max = 0;
          std::size_t i_max = 0;
          std::vector<std::size_t> various_i_max;
          if (_DEBUGGING == 0) printf("Trying with... ");
          for (int i = 0; i < _NUM_ACTIONS; ++i){
            tempSequence.set_top(i);
            if (nGram[tempSequence] > max){
              various_i_max.clear();
              if (_DEBUGGING == 0) tempSequence.print_sequence();
              max = nGram[tempSequence];
              i_max = i;
            }
            else if (max == oneGram[i]){
              if (_DEBUGGING == 0) tempSequence.print_sequence();
              various_i_max.push_back(i);
            }
          }
          if (various_i_max.size() > 1){
            prediction = random_gen.get(0, various_i_max.size());
          }
          else{
            if (_DEBUGGING == 0) printf("Max selected!\n");
            prediction = i_max;
          }
        }
        else{
          if (_DEBUGGING == 0) printf("Predicting with nGram 1");
          //check 1-gram
          std::size_t max = 0;
          std::size_t i_max = 0;
          std::vector<std::size_t> various_i_max;
          for (std::size_t i = 0; i < oneGram.size(); ++i){
            if (max < oneGram[i]){
              max = oneGram[i];
              i_max = i;
              various_i_max.clear();
            }
            else if (max == oneGram[i]){
              various_i_max.push_back(i);
            }
          }
          if (various_i_max.size() > 1){
            prediction = random_gen.get(0, various_i_max.size());
          }
          else{
            prediction = i_max;
          }
        }
        cur_sequence.set_size(temp_size);
        return prediction;
      }
    };

  }
}


#endif