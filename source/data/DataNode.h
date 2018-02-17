//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2016-2017.
//  Released under the MIT Software license; see doc/LICENSE
//
//
//  DataNode objects track a specific type of data over the course of a run.
//
//
//  Collection: New data can be pushed or pulled.
//   Add(VAL... v) pushes data to a node
//   AddDatum(VAL v) pushes just one datum, but can be used as an action for a signal.
//
//  Process: What should happen on Reset() ?
//   * Trigger an action to process the prior update's data stored.
//   * Clear all data.
//   * Send data to a stream
//     (or stats automatically have a stream that, if non-null data is sent to?)

#ifndef EMP_DATA_NODE_H
#define EMP_DATA_NODE_H

#include "../base/vector.h"
#include "../base/assert.h"
#include "../meta/IntPack.h"
#include "../tools/FunctionSet.h"
#include "../tools/string_utils.h"

namespace emp {

  // A set of modifiers are available do describe DataNode
  enum class data {
    Current,      // Track most recent value

    Info,         // Include information (name, keyword, description) for each instance.

    Log,          // Track all values since last Reset()
    Archive,      // Track Log + ALL values over time (with purge options)

    Range,        // Track min, max, mean, total
    FullRange,    // Track Range data over time.
    // Stats,        // Track Range + variance, standard deviation, skew, kertosis
    // FullStats,    // Track States + ALL values over time (with purge/merge options)

    Pull,         // Enable data collection on request.

    // Various signals are possible:
    SignalReset,  // Include a signal that triggers BEFORE Reset() to process data.
    SignalData,   // Include a signal when new data is added (as a group)
    SignalDatum,  // Include a signal when each datum is added.
    SignalRange,  // Include a signal for data in a range.
    SignalLimits, // Include a signal for data OUTSIDE a range.

    UNKNOWN       // Unknown modifier; will trigger error.
  };

  // Generic form of DataNodeModule (should never be used; trigger error!)
  template <typename VAL_TYPE, emp::data... MODS> class DataNodeModule {
  public:
    DataNodeModule() { emp_assert(false, "Unknown module used in DataNode!"); }
  };

  // Base form of DataNodeModule (available in ALL data nodes.)
  template <typename VAL_TYPE>
  class DataNodeModule<VAL_TYPE> {
  protected:
    size_t val_count;               // How many values have been loaded?
    emp::vector<VAL_TYPE> in_vals;  // What values are waiting to be included?

    void PullData_impl() { ; }
  public:
    DataNodeModule() : val_count(0), in_vals() { ; }

    using value_t = VAL_TYPE;

    size_t GetCount() const { return val_count; }
    size_t GetResetCount() const { return 0; }    // If reset count not tracked, always return 0.

    const std::string & GetName() const { return emp::empty_string(); }
    const std::string & GetDescription() const { return emp::empty_string(); }
    const std::string & GetKeyword() const { return emp::empty_string(); }

    void SetName(const std::string &) { emp_assert(false, "Invalid call for DataNode config."); }
    void SetDescription(const std::string &) { emp_assert(false, "Invalid call for DataNode config."); }
    void SetKeyword(const std::string &) { emp_assert(false, "Invalid call for DataNode config."); }

    void SetInfo(const std::string &, const std::string & _d="", const std::string & _k="") {
      (void) _d; (void) _k;
      emp_assert(false, "Invalid call for DataNode config.");
    }

    void AddDatum(const VAL_TYPE & val) { val_count++; }

    void Reset() { val_count = 0; }

    void PrintDebug(std::ostream & os=std::cout) {
      os << "BASE DataNodeModule.\n";
    }
  };

  // Specialized forms of DataNodeModule

  /** == data::Current ==
   * This module lets you track the current (i.e. most recently added) value */
  template <typename VAL_TYPE, emp::data... MODS>
  class DataNodeModule<VAL_TYPE, data::Current, MODS...> : public DataNodeModule<VAL_TYPE, MODS...> {
  protected:
    VAL_TYPE cur_val;

    using this_t = DataNodeModule<VAL_TYPE, data::Current, MODS...>;
    using parent_t = DataNodeModule<VAL_TYPE, MODS...>;
    using base_t = DataNodeModule<VAL_TYPE>;
  public:
    DataNodeModule() : cur_val() { ; }

    const VAL_TYPE & GetCurrent() const { return cur_val; }

    void AddDatum(const VAL_TYPE & val) { cur_val = val; parent_t::AddDatum(val); }

    void PrintDebug(std::ostream & os=std::cout) {
      os << "DataNodeModule for data::Current. (level " << (int) data::Current << ")\n";
      parent_t::PrintDebug(os);
    }
  };


  /** == data::Info ==
   * Including this moduel allows you to add information such as a name, 
   * description, and keyword for this node */
  template <typename VAL_TYPE, emp::data... MODS>
  class DataNodeModule<VAL_TYPE, data::Info, MODS...> : public DataNodeModule<VAL_TYPE, MODS...> {
  protected:
    std::string name;
    std::string desc;
    std::string keyword;

    using parent_t = DataNodeModule<VAL_TYPE, MODS...>;
  public:
    DataNodeModule() : name(), desc(), keyword() { ; }

    const std::string & GetName() const { return name; }
    const std::string & GetDescription() const { return desc; }
    const std::string & GetKeyword() const { return keyword; }

    void SetName(const std::string & _in) { name = _in; }
    void SetDescription(const std::string & _in) { desc = _in; }
    void SetKeyword(const std::string & _in) { keyword = _in; }

    void SetInfo(const std::string & _n, const std::string & _d="", const std::string & _k="") {
      name = _n;  desc = _d;  keyword = _k;
    }

    void PrintDebug(std::ostream & os=std::cout) {
      os << "DataNodeModule for data::Info. (level " << (int) data::Info << ")\n";
      parent_t::PrintDebug(os);
    }
  };


  /** == data::Log ==
   * This module lets you log all of the values that have been added since the last re-set*/
  template <typename VAL_TYPE, emp::data... MODS>
  class DataNodeModule<VAL_TYPE, data::Log, MODS...> : public DataNodeModule<VAL_TYPE, MODS...> {
  protected:
    emp::vector<VAL_TYPE> val_set;

    using this_t = DataNodeModule<VAL_TYPE, data::Log, MODS...>;
    using parent_t = DataNodeModule<VAL_TYPE, MODS...>;
    using base_t = DataNodeModule<VAL_TYPE>;

    using base_t::val_count;
  public:
    DataNodeModule() : val_set() { ; }

    const emp::vector<VAL_TYPE> & GetData() const { return val_set; }

    void AddDatum(const VAL_TYPE & val) {
      val_set.push_back(val);
      parent_t::AddDatum(val);
    }

    void Reset() {
      val_set.resize(0);
      parent_t::Reset();
    }

    void PrintDebug(std::ostream & os=std::cout) {
      os << "DataNodeModule for data::Log. (level " << (int) data::Log << ")\n";
      parent_t::PrintDebug(os);
    }
  };

  /** == data::Archive ==
   * This module lets you keep track of historical values in addition to those
   * added since the last re-set. Every time Reset() is called, all values that have
   * been added since the previous time Reset() was called are stored in a vector in 
   * the archive. */
  template <typename VAL_TYPE, emp::data... MODS>
  class DataNodeModule<VAL_TYPE, data::Archive, MODS...> : public DataNodeModule<VAL_TYPE, MODS...> {
  protected:
    emp::vector<emp::vector<VAL_TYPE>> archive;

    using this_t = DataNodeModule<VAL_TYPE, data::Archive, MODS...>;
    using parent_t = DataNodeModule<VAL_TYPE, MODS...>;
    using base_t = DataNodeModule<VAL_TYPE>;

    using base_t::val_count;
  public:
    DataNodeModule() : archive(1) { ; }

    const auto & GetArchive() const { return archive; }
    const emp::vector<VAL_TYPE> & GetData(size_t update) const { return archive[update]; }
    const emp::vector<VAL_TYPE> & GetData() const { return archive.back(); }

    size_t GetResetCount() const { return archive.size(); }

    void AddDatum(const VAL_TYPE & val) {
      archive.back().push_back(val);
      parent_t::AddDatum(val);
    }

    void Reset() {
      archive.resize(archive.size()+1);
      parent_t::Reset();
    }

    void PrintDebug(std::ostream & os=std::cout) {
      os << "DataNodeModule for data::Archive. (level " << (int) data::Archive << ")\n";
      parent_t::PrintDebug(os);
    }
  };

  /** == data::Range ==
   * This module allows this DataNode to store information (min, max, mean, count, and total) about 
   * the distribution of the values that have been added since the last call to Reset().*/
  template <typename VAL_TYPE, emp::data... MODS>
  class DataNodeModule<VAL_TYPE, data::Range, MODS...> : public DataNodeModule<VAL_TYPE, MODS...> {
  protected:
    double total;
    double min;
    double max;

    using this_t = DataNodeModule<VAL_TYPE, data::Range, MODS...>;
    using parent_t = DataNodeModule<VAL_TYPE, MODS...>;
    using base_t = DataNodeModule<VAL_TYPE>;

    using base_t::val_count;
  public:
    DataNodeModule() : total(0.0), min(0), max(0) { ; }

    double GetTotal() const { return total; }
    double GetMean() const { return total / (double) base_t::val_count; }
    double GetMin() const { return min; }
    double GetMax() const { return max; }

    void AddDatum(const VAL_TYPE & val) {
      total += (double) val;
      if (!val_count || min > (double) val) min = (double) val;
      if (!val_count || max < (double) val) max = (double) val;
      parent_t::AddDatum(val);
    }

    void Reset() {
      total = 0.0;
      min = 0.0;
      max = 0.0;
      parent_t::Reset();
    }

    void PrintDebug(std::ostream & os=std::cout) {
      os << "DataNodeModule for data::Range. (level " << (int) data::Range << ")\n";
      parent_t::PrintDebug(os);
    }
  };

  /** == data::FullRange ==
   * This module makes the DataNode store a full history of distributional information measured
   * by data::Range. These numbers describe the distribution of numbers added between calls to Reset().
   * Historical values for each measurement are stored in vectors (except mean, which is calculated
   * from total and count). */
  template <typename VAL_TYPE, emp::data... MODS>
  class DataNodeModule<VAL_TYPE, data::FullRange, MODS...> : public DataNodeModule<VAL_TYPE, MODS...> {
  protected:
    emp::vector<double> total_vals;
    emp::vector<size_t> num_vals;
    emp::vector<double> min_vals;
    emp::vector<double> max_vals;

    using this_t = DataNodeModule<VAL_TYPE, data::FullRange, MODS...>;
    using parent_t = DataNodeModule<VAL_TYPE, MODS...>;
    using base_t = DataNodeModule<VAL_TYPE>;

    using base_t::val_count;
  public:
    DataNodeModule()
      : total_vals(1,0.0), num_vals(1,0), min_vals(1,0.0), max_vals(1,0.0) { ; }

    double GetTotal() const { return total_vals.back(); }
    double GetMean() const { return total_vals.back() / (double) num_vals.back(); }
    double GetMin() const { return min_vals.back(); }
    double GetMax() const { return max_vals.back(); }

    double GetTotal(size_t update) const { return total_vals[update]; }
    double GetMean(size_t update) const { return total_vals[update] / (double) num_vals[update]; }
    double GetMin(size_t update) const { return min_vals[update]; }
    double GetMax(size_t update) const { return max_vals[update]; }

    size_t GetResetCount() const { return total_vals.size(); }

    void AddDatum(const VAL_TYPE & val) {
      total_vals.back() += val;
      num_vals.back() += 1;
      if (!val_count || val < min_vals.back()) min_vals.back() = val;
      if (!val_count || val > max_vals.back()) max_vals.back() = val;
      parent_t::AddDatum(val);
    }

    void Reset() {
      total_vals.push_back(0.0);
      num_vals.push_back(0);
      min_vals.push_back(0.0);
      max_vals.push_back(0.0);
      parent_t::Reset();
    }

    void PrintDebug(std::ostream & os=std::cout) {
      os << "DataNodeModule for data::FullRange. (level " << (int) data::FullRange << ")\n";
      parent_t::PrintDebug(os);
    }
  };

  /** == data::Pull ==
   * This module makes it possible to give the DataNode a function that it can call to calculate
   * new values or sets of values that it will then track. These functions are called every time
   * the PullData method is called on this node, and the values they return are measured as
   * specified by the other modules in this node. */ 
  template <typename VAL_TYPE, emp::data... MODS>
  class DataNodeModule<VAL_TYPE, data::Pull, MODS...> : public DataNodeModule<VAL_TYPE, MODS...> {
  protected:
    emp::FunctionSet<VAL_TYPE()> pull_funs;
    emp::FunctionSet<emp::vector<VAL_TYPE>()> pull_set_funs;

    using this_t = DataNodeModule<VAL_TYPE, data::Pull, MODS...>;
    using parent_t = DataNodeModule<VAL_TYPE, MODS...>;
    using base_t = DataNodeModule<VAL_TYPE>;

    using base_t::in_vals;

    void PullData_impl() {
      in_vals = pull_funs.Run();
      const emp::vector< emp::vector<VAL_TYPE> > & pull_sets = pull_set_funs.Run();
      for (const auto & x : pull_sets) {
        in_vals.insert(in_vals.end(), x.begin(), x.end());
      }
    }

  public:
    DataNodeModule() : pull_funs(), pull_set_funs() { ; }

    void AddPull(const std::function<VAL_TYPE()> & fun) { pull_funs.Add(fun); }
    void AddPullSet(const std::function<emp::vector<VAL_TYPE>()> & fun) { pull_set_funs.Add(fun); }

    void PrintDebug(std::ostream & os=std::cout) {
      os << "DataNodeModule for data::Pull. (level " << (int) data::Pull << ")\n";
      parent_t::PrintDebug(os);
    }
  };

  template <typename VAL_TYPE, typename MOD_PACK> class DataNode_Interface;

  template <typename VAL_TYPE, int... IMODS>
  class DataNode_Interface<VAL_TYPE, IntPack<IMODS...>>
    : public DataNodeModule<VAL_TYPE, (emp::data) IMODS...> {
    using parent_t = DataNodeModule<VAL_TYPE, (emp::data) IMODS...>;
  };

  template <typename VAL_TYPE, emp::data... MODS>
  class DataNode : public DataNode_Interface< VAL_TYPE, pack::RUsort<IntPack<(int) MODS...>> > {
  private:
    using parent_t = DataNode_Interface< VAL_TYPE, pack::RUsort<IntPack<(int) MODS...>> >;
    using parent_t::in_vals;
    using test = IntPack<(int)MODS...>;

  public:

    // Methods to retrieve new data.
    inline void Add() { ; }

    template <typename... Ts>
    inline void Add(const VAL_TYPE & val, const Ts &... extras) {
      parent_t::AddDatum(val); Add(extras...);
    }

    void PullData() {
      parent_t::PullData_impl();                                     // Pull all data into in_vals.
      for (const VAL_TYPE & val : in_vals) parent_t::AddDatum(val);  // Actually add the data.
    }

    // Methods to reset data.
    void Reset() { parent_t::Reset(); }

    // Methods to Print the templated values that a DataNode can produce.
    void PrintCurrent(std::ostream & os=std::cout) const { os << parent_t::GetCurrent(); }
    void PrintLog(std::ostream & os=std::cout,
                 const std::string & spacer=", ",
                 const std::string & eol="\n") const {
      const emp::vector<VAL_TYPE> & data = parent_t::GetData();
      for (size_t i=0; i < data.size(); i++) {
        if (i>0) os << spacer;
        os << data[i];
      }
      os << eol;
    }

    // Methods to debug.
    void PrintDebug(std::ostream & os=std::cout) {
      os << "Main DataNode.\n";
      parent_t::PrintDebug(os);
    }
  };

  // Shortcuts for common types of data nodes...

  /** A node that stores data about the most recent value it recieved, as well as the 
   * distribution (min, max, count, total, and mean) of values it has recieved since
   * the last reset. It also allows you to give it a name, description, and keyword.*/
  template <typename T, emp::data... MODS>
  using DataMonitor = DataNode<T, data::Current, data::Info, data::Range, MODS...>;

  /** A node that stores data about the most recent value it recieved, as well as all 
   * values it has recieved since the last reset. It also allows you to give it a name, 
   * description, and keyword.*/
  template <typename T, emp::data... MODS>
  using DataLog = DataNode<T, data::Current, data::Info, data::Log, MODS...>;
  
  /** A node that stores all data it recieves in an archive (vector of vectors). The inner
   * vectors are groups of data that were recieved between resets. This node also keeps 
   * a record of the min, max, count, and total of each vector, so you don't have to 
   * recalculate it later. Additionally, it allows you to give it a name, description,
   * and keyword.*/
  template <typename T, emp::data... MODS>
  using DataArchive = DataNode<T, data::Info, data::Archive, data::FullRange, MODS...>;
}

#endif
