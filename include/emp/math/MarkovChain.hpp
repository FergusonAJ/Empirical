/**
 *  @note This file is part of Empirical, https://github.com/devosoft/Empirical
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2018-2023.
 *
 *  @file  MarkovChain.hpp
 *  @brief Simple discrete-time Markov Chains and helper functions
 *  @note Status: ALPHA
 *
 */

#ifndef EMP_MARKOV_CHAIN_H

#include <iostream>
#include <unordered_map>
#include <sstream>

#include "../base/vector.hpp"
#include "../math/MMatrix.hpp"

namespace emp{
  
  /// A simple Markov Chain implementation for building discrete-time chains
  template<typename T = double, typename MATRIX_T = emp::MMatrix>
  class MarkovChain{
    private:
      MATRIX_T adj_matrix; ///< Representation of transition probabilities
      std::unordered_map<std::string, size_t> index_map; ///< Stores name of each state

      /// Validate the bookkeeping of the class
      bool ValidateInternals() const{
        bool is_valid = true;
        // Check the adjacency matrix dimensions
        const size_t num_states = GetNumStates();
        if(adj_matrix.num_rows() != num_states){ 
          is_valid = false;
          std::cerr << "Error! Markov chain has incorrect number of lists in adjacency list!\n";
          std::cerr << "  Expected: " << num_states << ". Actual: " << adj_matrix.num_rows() 
              << std::endl;
        }
        for(size_t idx = 0; idx < num_states; ++idx){
          if(adj_matrix.num_cols() != num_states){
            is_valid = false;
            std::cerr << "Error! Adjacency list with incorrect length (Index " << idx << ")\n";
            std::cerr << "  Expected: " << num_states << ". Actual: " << adj_matrix.num_cols()
                << std::endl;
          }
        }
        if(index_map.size() != num_states){
          is_valid = false;
          std::cerr << "Error! Markov chain has incorrect number of states in index map!\n";
          std::cerr << "  Expected: " << num_states << ". Actual: " << index_map.size() 
              << std::endl;
        }
        return is_valid;
      }
      
      /// Validate that each row sums to 1
      bool ValidateProbabilities() const{
        bool is_valid = true;
        const size_t num_states = GetNumStates();
        for(size_t idx_a = 0; idx_a < num_states; ++idx_a){
          double sum = 0;
          for(size_t idx_b = 0; idx_b < num_states; ++idx_b){
            sum += adj_matrix.Get(idx_a,idx_b);
          }
          if(sum != 1.0){
            is_valid = false;
          }
        }
        return is_valid;
      }
    
    public:
      MarkovChain() : adj_matrix(0,0){
        ;
      }

      /// Add a new state with a given name
      size_t AddState(const std::string& name){
        emp_assert(index_map.find(name) == index_map.end());
        const size_t new_idx = adj_matrix.num_rows();
        index_map[name] = new_idx;
        adj_matrix.AddRow(0);
        adj_matrix.AddCol(0);
        return new_idx;
      }

      /// Set the transition between two states
      void SetTransition(size_t idx_a, size_t idx_b, T prob){
        emp_assert(adj_matrix.num_rows() > idx_a);
        emp_assert(adj_matrix.num_cols() > idx_b);
        adj_matrix.Set(idx_a, idx_b, prob);
      }

      /// Set the transition between two states
      void SetTransition(const std::string& name_a, const std::string& name_b, T prob){
        emp_assert(index_map.find(name_a) != index_map.end());
        emp_assert(index_map.find(name_b) != index_map.end());
        const size_t idx_a = index_map[name_a];
        const size_t idx_b = index_map[name_b];
        SetTransition(idx_a, idx_b, prob);
      }
     
      /// Ensure that the bookkeeping is correct and, for each state, all outputs sum to 1 
      bool Validate() const{
        return ValidateInternals() && ValidateProbabilities();
      }

      /// Return const ref to transition values
      const MATRIX_T& GetMatrix() const{
        return adj_matrix;
      }

      /// Get the number of nodes in this chain
      size_t GetNumStates() const{
        return adj_matrix.num_rows();
      }
  };
}

#endif
