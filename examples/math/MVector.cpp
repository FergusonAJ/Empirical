//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2018-2021.
//  Released under the MIT Software license; see doc/LICENSE
//
//
//  Some example code for using emp::MVector

#include <iostream>
#include "emp/math/MVector.hpp"


int main(){
  // Default constructor -> Zero vector
  emp::MVector a(3);
  std::cout << "a = " << a << std::endl;

  // Initializer list -> Copy those values over
  emp::MVector b({1,2,3});
  std::cout << "b = " << b << std::endl;

  // Empiciral vector -> copy values over
  emp::vector<double> tmp_vec = {3,4,5};
  emp::MVector d(tmp_vec);
  std::cout << "d = " << d << std::endl;
  
  // Setter
  std::cout << "Changing middle value to 10" << std::endl;
  d[1] = 10; 
  std::cout << "d = " << d << std::endl;
  
  // Addition
  std::cout << "a + b = " << a + b << std::endl;
  std::cout << "a + d = " << a + d << std::endl;
  std::cout << "b + d = " << b + d << std::endl;
  // Subtraction
  std::cout << "a - b = " << a - b << std::endl;
  std::cout << "a - d = " << a - d << std::endl;
  std::cout << "b - d = " << b - d << std::endl;
  // Multiplication
  std::cout << "2 * b = " << 2 * b << std::endl;
  std::cout << "b * 2 = " << b * 2 << std::endl;
  // Division
  std::cout << "b / 2 = " << b / 2 << std::endl;
  // Pairwise mult
  std::cout << "b pairwise multiply b = " << b.PairwiseMult(b) << std::endl;
  // Dot product
  std::cout << "b dot d = " << b.Dot(d) << std::endl;
  // Cross product
  std::cout << "b cross d = " << b.Cross(d) << std::endl;

  // Comparisons
  std::cout << "a == b = " << (a == b) << std::endl;
  std::cout << "a == <0,0,0> = " << (a == emp::MVector(3)) << std::endl;
  std::cout << "a == <0,0,0,0> = " << (a == emp::MVector(4)) << std::endl;
  std::cout << "a != b = " << (a != b) << std::endl;
  std::cout << "a != <0,0,0> = " << (a != emp::MVector(3)) << std::endl;
  std::cout << "a != <0,0,0,0> = " << (a != emp::MVector(4)) << std::endl;

  // Magnitude
  std::cout << "|a|^2 = " << a.MagnitudeSquared() << std::endl;
  std::cout << "|b|^2 = " << b.MagnitudeSquared() << std::endl;
  std::cout << "|d|^2 = " << d.MagnitudeSquared() << std::endl;
  std::cout << "|a| = " << a.Magnitude() << std::endl;
  std::cout << "|b| = " << b.Magnitude() << std::endl;
  std::cout << "|d| = " << d.Magnitude() << std::endl;

  // Cross product
  std::cout << "b x d = " << b.Cross(d) << std::endl;
  
  // Normalize
  std::cout << "a.Normalized() = " << a.Normalized() << std::endl;
  std::cout << "b.Normalized() = " << b.Normalized() << std::endl;
  std::cout << "d.Normalized() = " << d.Normalized() << std::endl;
  
  // Modifiying operations
  std::cout << "a = " << a << std::endl;
  std::cout << "b = " << b << std::endl;
  std::cout << "d = " << d << std::endl;

  std::cout << "a += b " << std::endl;
  a += b;
  std::cout << "a = " << a << std::endl;

  std::cout << "d -= a " << std::endl;
  d -= a;
  std::cout << "d = " << d << std::endl;

  std::cout << "a *= 5" << std::endl;
  a *= 5;
  std::cout << "a = " << a << std::endl;

  std::cout << "a /= 2.5" << std::endl;
  a /= 2.5;
  std::cout << "a = " << a << std::endl;

  std::cout << "a.Normalize() " << std::endl;
  a.Normalize();
  std::cout << "a = " << a << std::endl;

  
  return 0;
}
