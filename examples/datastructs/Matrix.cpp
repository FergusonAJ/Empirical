//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2018-2021.
//  Released under the MIT Software license; see doc/LICENSE
//
//
//  Some example code for using emp::Matrix (*not* MMatrix)

#include <iostream>
#include "emp/datastructs/Matrix.hpp"

int main(){
  std::cout << "******** DOUBLE ********" << std::endl;
  {
    emp::Matrix<double> mat_3x3_base(3,3);
    std::cout << "3 x 3:" << std::endl;
    std::cout << mat_3x3_base << std::endl;
    
    emp::Matrix<double> mat_3x3_default(3,3,7);
    std::cout << "3 x 3 (alt):" << std::endl;
    std::cout << mat_3x3_default << std::endl;
    
    emp::Matrix<double> mat_4x2(4,2);
    std::cout << "4 x 2:" << std::endl;
    std::cout << mat_4x2 << std::endl;

    emp::Matrix<double> mat_2x2(2,2, {1,2,3,4});
    std::cout << "2 x 2:" << std::endl;
    std::cout << mat_2x2 << std::endl;

    emp::vector<double> v({1,2,3,4,5,6,7,8,9});
    emp::Matrix mat_3x3_1(3,3, v);
    std::cout << "M1(3 x 3):" << std::endl;
    std::cout << mat_3x3_1 << std::endl;
    
    emp::vector<double> v2({3,4,5,1,2,3,7,8,9});
    emp::Matrix mat_3x3_2(3,3,v2);
    std::cout << "M2(3 x 3):" << std::endl;
    std::cout << mat_3x3_2 << std::endl;
    
    // Get
    std::cout << "M2[1]:" << std::endl;
    std::cout << mat_3x3_2[1] << std::endl;
    std::cout << "M2.GetRow(2):" << std::endl;
    std::cout << mat_3x3_2.GetRow(2) << std::endl;
    std::cout << "M2[1][2]:" << std::endl;
    std::cout << mat_3x3_2[1][2] << std::endl;
    std::cout << "M2.GetCol(2):" << std::endl;
    std::cout << mat_3x3_2.GetCol(2) << std::endl;
    
    // Set 
    std::cout << "M2(3 x 3):" << std::endl;
    std::cout << mat_3x3_2 << std::endl;
    std::cout << "M2[1][2] = 9:" << std::endl;
    mat_3x3_2[1][2] = 9;
    std::cout << "M2(3 x 3):" << std::endl;
    std::cout << mat_3x3_2 << std::endl;
    std::cout << "M2.SetRow(2) = <0,1,0>" << std::endl;
    mat_3x3_2.SetRow(2, emp::vector<double>{0,1,0});
    std::cout << mat_3x3_2 << std::endl;
    std::cout << "M2.SetCol(0) = <7,2,9>" << std::endl;
    mat_3x3_2.SetCol(0, emp::vector<double>{7,2,9});
    std::cout << mat_3x3_2 << std::endl;
    
    // Transpose
    std::cout << "M2:" << std::endl;
    std::cout << mat_3x3_2 << std::endl;
    std::cout << "M2^T:" << std::endl;
    std::cout << mat_3x3_2.Transposed() << std::endl;
  }
  
  std::cout << "******** CHAR *********" << std::endl;
  {
    emp::Matrix<char> mat_3x3_base(3,3);
    std::cout << "3 x 3:" << std::endl;
    std::cout << mat_3x3_base << std::endl;
    
    emp::Matrix<char> mat_3x3_default(3,3,'F');
    std::cout << "3 x 3 (alt):" << std::endl;
    std::cout << mat_3x3_default << std::endl;
    
    emp::Matrix<char> mat_4x2(4,2);
    std::cout << "4 x 2:" << std::endl;
    std::cout << mat_4x2 << std::endl;

    emp::Matrix<char> mat_2x2(2,2, {1,2,3,4});
    std::cout << "2 x 2:" << std::endl;
    std::cout << mat_2x2 << std::endl;

    emp::vector<char> v({'a','b','c','d','e','f','g','h','i'});
    emp::Matrix mat_3x3_1(3,3, v);
    std::cout << "M1(3 x 3):" << std::endl;
    std::cout << mat_3x3_1 << std::endl;
    
    emp::vector<char> v2({'z','y','x','w','v','u','t','s','r'});
    emp::Matrix mat_3x3_2(3,3,v2);
    std::cout << "M2(3 x 3):" << std::endl;
    std::cout << mat_3x3_2 << std::endl;
    
    // Get
    std::cout << "M2[1]:" << std::endl;
    std::cout << mat_3x3_2[1] << std::endl;
    std::cout << "M2.GetRow(2):" << std::endl;
    std::cout << mat_3x3_2.GetRow(2) << std::endl;
    std::cout << "M2[1][2]:" << std::endl;
    std::cout << mat_3x3_2[1][2] << std::endl;
    std::cout << "M2.GetCol(2):" << std::endl;
    std::cout << mat_3x3_2.GetCol(2) << std::endl;
    
    // Set 
    std::cout << "M2(3 x 3):" << std::endl;
    std::cout << mat_3x3_2 << std::endl;
    std::cout << "M2[1][2] = q:" << std::endl;
    mat_3x3_2[1][2] = 'q';
    std::cout << "M2(3 x 3):" << std::endl;
    std::cout << mat_3x3_2 << std::endl;
    std::cout << "M2.SetRow(2) = <a,b,c>" << std::endl;
    mat_3x3_2.SetRow(2, emp::vector<char>{'a','b','c'});
    std::cout << mat_3x3_2 << std::endl;
    std::cout << "M2.SetCol(0) = <d,e,f>" << std::endl;
    mat_3x3_2.SetCol(0, emp::vector<char>{'d','e','f'});
    std::cout << mat_3x3_2 << std::endl;
    
    // Transpose
    std::cout << "M2:" << std::endl;
    std::cout << mat_3x3_2 << std::endl;
    std::cout << "M2^T:" << std::endl;
    std::cout << mat_3x3_2.Transposed() << std::endl;
  }

}
