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
    // This will be used "as globals" for the creation of sequence and hash tables
    enum {_SIZE_SEQUENCE = 5, _NUM_ACTIONS = 9, _DEBUGGING = 1};

    /// @brief This class is a simple sequence. It can receive new inputs.
    /// This clase is made as a "queue" of four elements.
    class SequenceInput{
      // The array that will contain the information
      std::array<int, _SIZE_SEQUENCE> sequence;
      // The top of the queue
      int top;
      // The size of the queue
      std::size_t size;

      public:
        /// @brief This is the default constructor of the Sequence (with top in zero and size 1)
        SequenceInput() : top(0), size(1) {}
        
        /// @brief This is a copy constructor receiveng a given sequence
        SequenceInput(const std::array<int, _SIZE_SEQUENCE>& n_sequence) : sequence(n_sequence), top(0), size(_SIZE_SEQUENCE - 1) {}
    
        /// @brief This is used to create a new input
        /// The size is fixed, therefore, the queue will be always the same size
        void new_input(int input){
          sequence[top] = input;
          if (++top > _SIZE_SEQUENCE-1)
            top = 0;
        }

        /// @brief Set the size of the queue
        /// It checks if it's bigger than the available space
        void set_size(std::size_t n_size){
          if (n_size > _SIZE_SEQUENCE) size = n_size;
          size = n_size;
        }

        /// @brief Returns the size of the queue
        std::size_t get_size() const{
          return size;
        }

        /// @brief It makes grow the queue.
        /// It checks that the size of the queue doesn't exceed the capacity
        void grow_sequence(){
          ++size;
          if (size > _SIZE_SEQUENCE) size = _SIZE_SEQUENCE;
        }

        /// @brief It returns the position of the top (not the content of the top)
        std::size_t get_top() const{
          return top;
        }

        /// @brief It sets the VALUE of the top (not the position!)
        void set_top(int value){
          if (top != 0)
            sequence[top - 1] = value;
          else
            sequence[_SIZE_SEQUENCE - 1] = value;
        }

        /// @brief It gets the element in a given position (from the top!).
        /// It checks that it doesn't exceed the size of the queue
        int get_element(std::size_t pos) const{
          if (pos > size) pos = size;
          int n_pos = top - 1 - pos;
          if (n_pos < 0) 
            n_pos = _SIZE_SEQUENCE + n_pos;
          return sequence[n_pos];
        }

        /// @brief This function just prints the sequence on the screen
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

        /// @brief This is a debug printer, that will print the whole sequence and the top position
        void print_debug_all(){
          printf("Sequence: top %i and size %i\n", top, size);
          for (const int& i : sequence){
            printf("%i ", i);
          }
          printf("\n");
        }
    };
    
    /// @brief This is the operator == for comparison of sqeuences
    bool operator==(const SequenceInput& a, const SequenceInput& b){
      if (a.get_size() != b.get_size()) return false;
  
      std::size_t size_sequence = a.get_size();
      bool equal = true;
      for (int i = 0; equal && i < size_sequence; ++i){
        equal = a.get_element(i) == b.get_element(i);
      }

      return equal;
    }

    /// @brief This is the operator != for comparison of sequences
    /// This has been implemented as a derivated of operator==
    bool operator!=(const SequenceInput& a, const SequenceInput& b){
      return !(a == b);
    }

    /// @brief This is the class with the Hash function for the Sequence
    template <class T>
    class MyHash;
    template <> 
    class MyHash<SequenceInput>{
    public:
      /// @brief This is the hash function of a sequence
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

    /// @brief This class contains the whole information of the ngram prediction. 
    /// Internally it works with an array for the 1gram, a hash table for the
    /// sum of the total ocurrencies of a n-1 sequence, and the ngram
    /// This last ngram is made as well as a hash table
    class PredictiveAI{
      // This is the total dimension of the ngram
      std::size_t dimension_nGram;
      // This is the treshold used for an hierarchic ngram approach
      std::size_t treshold;
      // This is the current sequence (used to predict and to store)
      SequenceInput cur_sequence;
      // The 1Gram is an array
      std::array<int, 15> oneGram;
      // The rest of the nGram representation is stored in a Hash table.
      std::unordered_map<SequenceInput, unsigned int, MyHash<SequenceInput>> nGram;
      // The rest of the nGram representation is stored in a Hash table.
      std::unordered_map<SequenceInput, unsigned int, MyHash<SequenceInput>> nGram_sums;

      /// @brief This will reset the information of the ngrams. It's kept privated.
      /// To reset the AI from outside, use resetAI()
      void reset_n_gram(){
        for (int i = 0; i < oneGram.size(); ++i){
          oneGram[i] = 0;
        }
        nGram.clear();
        nGram_sums.clear();
      }

    public:
      /// @brief Default constructor of PredictiveAI
      PredictiveAI() : dimension_nGram(4), treshold(6){
        reset_n_gram();
      }

      /// @brief Constructor with the size of the ngram
      PredictiveAI(std::size_t n_dimension_nGram) : dimension_nGram(n_dimension_nGram), treshold(6){
        reset_n_gram();
      }

      /// @brief This function will be used to initialize the ngram (with 0 values as sequence)
      void init(std::size_t size){
        cur_sequence.set_size(size);
        for (std::size_t i = 0; i != size; ++i){
          cur_sequence.new_input(0);
        }
      }

      /// @brief This will set the dimension of the ngram.
      void set_dimension(std::size_t n_dimension){
        dimension_nGram = n_dimension;
      }

      /// @brief This function will reset the AI and make it forget all stored information
      void resetAI(){
        reset_n_gram();
        cur_sequence.new_input(0);
        cur_sequence.new_input(0);
        cur_sequence.new_input(0);
        cur_sequence.new_input(0);
      }

      /// @brief This method is used to add a new user input. 
      /// It updates the current sequence and the ngram information
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

      /// @brief This method give the last input received
      int get_last(){
        return cur_sequence.get_element(0);
      }

      /// @brief This method will obtain a prediction with a hierarchic approach
      /// It will use the dimension of the gram, so it can be 1..ngram!
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

      /// @brief This is used to print all the current information of the ngram
      /// This method will ignore the 'sum' of the (n-1)gram values!
      void print_nGrams(){

      }
    };

  }
}


#endif