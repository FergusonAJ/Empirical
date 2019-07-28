//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2019.
//  Released under the MIT Software license; see doc/LICENSE
//
//  Analyze probabilities for selection using various selection techniques.

// Empirical Includes
#include "config/config.h"
#include "config/ArgManager.h"
#include "config/command_line.h"
#include "base/vector.h"
#include "tools/IndexMap.h"

// Local Includes
#include "SelectionAnalyzeConfig.h"
#include "SelectionData.h"


int main(int argc, char* argv[]){
    auto args = emp::cl::ArgManager(argc, argv);
    SelectionAnalyzeConfig config;
    std::string config_filename;
    if(!args.UseArg("-CONFIG", config_filename, "Config filename")) 
        config_filename = "config.cfg";
    std::cout << "Loading config: " << config_filename << std::endl;
    if(!config.Read(config_filename)){
        std::cerr << "Unable to load file: " << config_filename << std::endl;
    }
    if (args.ProcessConfigOptions(config, std::cout, config_filename, "config-macros.h") == false)
        exit(-1);
    if (args.TestUnknown() == false){
        std::cerr << "Unknown arguments detected!" << std::endl;
        exit(-1); // If there are leftover args, throw an error.
    } 

    // Load in general, non selection scheme dependent config options    
    SelectionScheme selection_scheme = (SelectionScheme)config.SELECTION_SCHEME();
    std::string input_filename = config.INPUT_FILENAME(); 
    std::string output_filename = config.OUTPUT_FILENAME(); 
    bool verbose = config.VERBOSE(); 
    bool remove_headers = !config.NO_COL_HEADINGS();  
    if(verbose) std::cout << "Config loaded successfully!" << std::endl;
    
    // Check filenames
    if(input_filename.length() <= 0){
        std::cerr << "Error! No input filename set. INPUT_FILENAME must be set in the config!" 
            << std::endl;
        exit(-1);
    }
    std::ofstream out_file;
    if (output_filename.length() > 0) {
        std::cout << "Opening output file: " << output_filename << std::endl;
        out_file.open(output_filename);
    }
    std::ostream & out_stream = (output_filename.length() > 1) ? out_file : std::cout;
 
            
    // Handle the different selection schemes independently
    switch(selection_scheme){
        case LEXICASE:{
            // Load lexicase-specific configuration options
            size_t start_idx = config.LEXICASE_START_IDX();
            if(verbose) std::cout << "Loading in file: "  << input_filename << std::endl;
            SelectionData data(input_filename, 0, remove_headers);
            if(verbose) data.SetVerbose();
            data.SetStartFitnessID(start_idx);
            bool do_subsampling = config.LEXICASE_DO_SUBSAMPLING();
            if(!do_subsampling){
                if(verbose) std::cout << "Doing standard lexicase selection!" << std::endl;
                // Calculate probabilities! (Full lexicase) 
                data.AnalyzeLexicase();
                data.CalcLexicaseProbs();
                data.PrintSelectProbs(out_stream, false);  
            }
            else{
                // Estimate probabilities! (Subsampled lexicase)
                if(verbose) std::cout << "Doing subsampled lexicase selection!" << std::endl;
                size_t sub_pop_count = config.LEXICASE_SUBSAMPLING_GROUP_SIZE();
                if(!sub_pop_count) sub_pop_count = data.GetNumOrgs();
                size_t sub_test_count = config.LEXICASE_SUBSAMPLING_TEST_COUNT();
                if(!sub_test_count) sub_test_count = data.GetNumCriteria();
                size_t sub_trial_count = config.LEXICASE_SUBSAMPLING_NUM_SAMPLES();
                emp::Random random;
                auto result = data.CalcSubsampleLexicaseProbs(sub_pop_count, 
                    sub_test_count, sub_trial_count, random);
                double total = 0.0;
                for (size_t i = 0; i < result.size(); i++) {
                  if (i) out_stream << ',';
                  out_stream << result[i];
                  total += result[i];
                }
                out_stream << std::endl;
                if(verbose) std::cout << "Total prob = " << total << std::endl;
            }
        }
        break;
        case TOURNAMENT:{
            // Load in aggregate fitness data
            size_t agg_fit_idx = config.AGGREGATE_FIT_IDX();
            if(verbose) std::cout << "Loading in file: "  << input_filename << std::endl;
            SelectionData data(input_filename, 0, remove_headers);
            if(verbose) data.SetVerbose();
            data.SetFitnessID(agg_fit_idx);
            // Load in tournament-specific configuration options
            size_t tourney_size = config.TOURNAMENT_SIZE();
            if(!tourney_size) tourney_size = data.GetNumOrgs(); 
            if(verbose) std::cout << "Doing tournament (size = " << tourney_size << 
                ") selection!" << std::endl;
            size_t tourney_trial_count = config.TOURNAMENT_SAMPLES(); 
            // Tournament is just elite with a sampled population
            // Thus, tournamnet is just lexicase with one column and pop sampling!
            emp::Random random;
            auto result = data.CalcSubsampleLexicaseProbs(tourney_size,
                 data.GetNumCriteria(), tourney_trial_count, random);
            double total = 0.0;
            for (size_t i = 0; i < result.size(); i++) {
              if (i) out_stream << ',';
              out_stream << result[i];
              total += result[i];
            }
            out_stream << std::endl;
            if(verbose) std::cout << "Total prob = " << total << std::endl;
        }
        break;
        case ELITE:{
            // Load in aggregate fitness data
            size_t agg_fit_idx = config.AGGREGATE_FIT_IDX();
            if(verbose) std::cout << "Loading in file: "  << input_filename << std::endl;
            SelectionData data(input_filename, 0, remove_headers);
            if(verbose) data.SetVerbose();
            if(verbose) std::cout << "Doing elite selection!" << std::endl;
            data.SetFitnessID(agg_fit_idx);
            
            // Elite is just lexicase on one column!
            data.AnalyzeLexicase();
            data.CalcLexicaseProbs();
            data.PrintSelectProbs(out_stream, false);  
        }
        break;
        case ROULETTE:{
            // Load in aggregate fitness data
            size_t agg_fit_idx = config.AGGREGATE_FIT_IDX();
            if(verbose) std::cout << "Loading in file: "  << input_filename << std::endl;
            SelectionData data(input_filename, 0, remove_headers);
            if(verbose) data.SetVerbose();
            if(verbose) std::cout << "Doing roulette selection!" << std::endl;
            data.SetFitnessID(agg_fit_idx);
            //Calculate probabilities using roulette selection
            emp::vector<double> fit_data = data.GetFitData();
            const size_t num_orgs = data.GetNumOrgs();
            emp::IndexMap fit_map(num_orgs);
            for (size_t i = 0; i < num_orgs; i++) {
              fit_map[i] = fit_data[i];
            }
            for (size_t i = 0; i < num_orgs; i++) {
              out_stream << fit_map.GetProb(i);
              if(i != num_orgs - 1){
                out_stream << ", ";
              }
            }
            out_stream << std::endl;
        }
        break;
    }
}
