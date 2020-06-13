// This is the main function for the NATIVE version of this project.

/*
 * WARNING: ProjectTemplate is deprecated in favor of the Cookiecutter Empirical
 * Project, which you can find at
 * https://github.com/devosoft/cookiecutter-empirical-project
*/

#include <iostream>

#include "base/vector.h"
#include "config/command_line.h"

int main(int argc, char* argv[])
{
  emp::vector<std::string> args = emp::cl::args_to_strings(argc, argv);

  std::cout << "Hello World!" << std::endl;

  std::cout << "WARNING: ProjectTemplate is deprecated in favor of the Cookiecutter Empirical Project, which you can find at https://github.com/devosoft/cookiecutter-empirical-project" << std::endl;

}
