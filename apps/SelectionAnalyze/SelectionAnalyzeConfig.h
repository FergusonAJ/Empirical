//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2019.
//  Released under the MIT Software license; see doc/LICENSE
//
//  Analyze probabilities for selection using various selection techniques.

// Empirical Includes
#include "config/config.h"

enum SelectionScheme{
    LEXICASE = 0,
    TOURNAMENT = 1, 
    ELITE = 2, 
    ROULETTE = 3
};

EMP_BUILD_CONFIG(SelectionAnalyzeConfig,
    // General Settings
    GROUP(GENERAL, "General settings that apply to all selection schemes"),
    VALUE(SELECTION_SCHEME, size_t, 0, "The type of selection to be analyzed. 0 for lexicase, 1 for "
        "tournament, 2 for elite, 3 for roulette."),
    VALUE(INPUT_FILENAME, std::string, "", "The path of the file containing the data to be used."),
    VALUE(OUTPUT_FILENAME, std::string, "", "The path to a file that will be created to save the "
        "generated selection probabilities. (Leave blank for std::out)"),
    VALUE(NO_COL_HEADINGS, bool, false, "Set to true if column headers have already been "
        "removed."),
    VALUE(VERBOSE, bool, false, "Prints more information during computation, useful for debugging."),
    
    
    // Aggregate Schemes Settings
    GROUP(AGGREGATE, "Settings that apply to all selection schemes that aggregate fitness across "
        " test cases (i.e., not lexicase selection)."),
    VALUE(AGGREGATE_FIT_IDX, size_t, 0, "Index of the column that contains aggregate fitness values "
        "to be used in selection."),
    
    // Lexicase Settings
    GROUP(LEXICASE, "Settings that apply only to lexicase selection (SELECTION_SCHEME = 0)."),
    VALUE(LEXICASE_START_IDX, size_t, 0, "Index of the column (starting at zero) that represents "
        "a test case to be used in lexicase. Note: There should be no columns following the test "
        "case columns."), 
    VALUE(LEXICASE_DO_SUBSAMPLING, bool, false, "If 1, LEXICASE_POP_SAMPLING, LEXICASE_TEST_SAMPLING"
        " and LEXICASE_SUBSAMPLING_SAMPLES will be used. Note: this is an estimated analysis."),
    VALUE(LEXICASE_SUBSAMPLING_GROUP_SIZE, size_t, 0, "How many individuals will be sampled before"
        " doing lexicase selection. For cohort selection, this is cohort size. (a value of zero"
        " gives the whole population (such as in downsampled lexicase)."), 
    VALUE(LEXICASE_SUBSAMPLING_TEST_COUNT, size_t, 0, "Number of tests to sample before running "
        " lexicase selection (0 for all)."),
    VALUE(LEXICASE_SUBSAMPLING_NUM_SAMPLES, size_t, 1000, "Number of times to sample the"
        " configuration. The larger the number, the closer the estimate should"
        " be to the true value."),    
    
    // Tournament Settings
    GROUP(TOURNAMENT, "Settings that apply only to tournament selection (SELECTION_SCHEME = 1)."),
    VALUE(TOURNAMENT_SIZE, size_t, 0, "Number of organisms in each tournament."
        " (0 for whole population)"), 
    VALUE(TOURNAMENT_SAMPLES, size_t, 1000, "Number of times to sample the configuration. "
        "The larger the value, the closer the estimate should be to the true value.") 
)
