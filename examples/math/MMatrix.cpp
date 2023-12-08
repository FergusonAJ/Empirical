//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2018-2021.
//  Released under the MIT Software license; see doc/LICENSE
//
//
//  Some example code for using emp::MMatrix

#include <iostream>
#include "emp/math/MMatrix.hpp"

int main(){
  emp::MMatrix mat_3x3(3,3);
  std::cout << "3 x 3:" << std::endl;
  std::cout << mat_3x3 << std::endl;
  
  emp::MMatrix mat_4x2(4,2);
  std::cout << "4 x 2:" << std::endl;
  std::cout << mat_4x2 << std::endl;

  emp::MMatrix mat_2x2(2,2, {1,2,3,4});
  std::cout << "2 x 2:" << std::endl;
  std::cout << mat_2x2 << std::endl;

  emp::vector<double> v({1,2,3,4,5,6,7,8,9});
  emp::MMatrix mat_3x3_1(3,3, v);
  std::cout << "M1(3 x 3):" << std::endl;
  std::cout << mat_3x3_1 << std::endl;
  
  emp::vector<double> v2({3,4,5,1,2,3,7,8,9});
  emp::MMatrix mat_3x3_2(3,3,v2);
  std::cout << "M2(3 x 3):" << std::endl;
  std::cout << mat_3x3_2 << std::endl;
  
  std::cout << "M1 + M2:" << std::endl;
  std::cout << mat_3x3_1 + mat_3x3_2 << std::endl;

  std::cout << "M2(3 x 3):" << std::endl;
  std::cout << "M2 Row(0): " << mat_3x3_2.GetRow(0) << std::endl;
  std::cout << "M2 Row(1): " << mat_3x3_2.GetRow(1) << std::endl;
  std::cout << "M2 Row(2): " << mat_3x3_2.GetRow(2) << std::endl;
  std::cout << "M2 Col(0): " << mat_3x3_2.GetCol(0) << std::endl;
  std::cout << "M2 Col(1): " << mat_3x3_2.GetCol(1) << std::endl;
  std::cout << "M2 Col(2): " << mat_3x3_2.GetCol(2) << std::endl;

  // Multiplication
  emp::MMatrix mat_3x3_scale_2(3,3, {2,0,0,0,2,0,0,0,2});
  std::cout << "2x Scaling mat: " << std::endl;
  std::cout << mat_3x3_scale_2 << std::endl;
  std::cout << "2x scaling mat * M2" << std::endl;
  std::cout << mat_3x3_scale_2 * mat_3x3_2 << std::endl;

  emp::MVector v_2({1.0, 4, 2});
  std::cout << "Vector: " << v_2 << std::endl;
  std::cout << "2x scaling mat * vector:" << std::endl;
  std::cout << mat_3x3_scale_2 * v_2 << std::endl;
  std::cout << std::endl;
  std::cout << "vector * 2x scaling mat:" << std::endl;
  std::cout << v_2 * mat_3x3_scale_2 << std::endl;
  std::cout << std::endl;

  std::cout << "M1 * 2:" << std::endl;
  std::cout << mat_3x3_1 * 2 << std::endl;
  std::cout << "2 * M1:" << std::endl;
  std::cout << 2 * mat_3x3_1 << std::endl;

  // Transpose
  std::cout << "M2:" << std::endl;
  std::cout << mat_3x3_2 << std::endl;
  std::cout << "M2^T:" << std::endl;
  std::cout << mat_3x3_2.Transposed() << std::endl;

  // Deteriminant
  std::cout << "M3 (2x2): " << std::endl;
  std::cout << mat_2x2 << std::endl;
  std::cout << "|M3| = " << mat_2x2.Determinant() << std::endl;
  std::cout << "|M2| = " << mat_3x3_2.Determinant() << std::endl;
}
