/**
 *  @note This file is part of Empirical, https://github.com/devosoft/Empirical
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2018
 *
 *  @file  GenomeTypeBase.h
 *  @brief Base class for all genome types.
 *
 *  Genomes are generic ways of encoding info for Brains or other aspects of organisms.
 *
 *  Each derived genome class much have a data_t type indicating a struct with the set of
 *  information it needs to store in organisms.  It must also have the following functions:
 *    std::string GetClassName()
 *    emp::Config & GetConfig()
 *    void Randomize(emp::Random &)
 */

#ifndef MABE_GENOME_TYPE_BASE_H
#define MABE_GENOME_TYPE_BASE_H

#include <string>

#include "config/config.h"

namespace mabe {

  class GenomeTypeBase {
  private:
    std::string name;
  public:
    GenomeTypeBase() { ; }
    virtual ~GenomeTypeBase() { ; }

    virtual std::string GetClassName() const = 0;
    virtual emp::Config & GetConfig() = 0;

    virtual void Randomize(emp::Random &) = 0;

    const std::string & GetName() { return name; }
    void SetName(const std::string & in_name) { name = in_name; }
  };

}

#endif

