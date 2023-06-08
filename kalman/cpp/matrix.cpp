
#include <stdio.h>
#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;
using namespace std;


int main(void){
    
    MatrixXd matrix_x;
    MatrixXd matrix_y;
    
    
    
    matrix_x = MatrixXd(2,1);
    matrix_y = MatrixXd(1,2);
    
    matrix_x << 1,3;
    matrix_y << 1,3;
    matrix_y = Transpose(matrix_y);
    cout << matrix_x << endl;
    cout << matrix_y << endl;
    cout << matrix_x + matrix_y << endl;
    
    
    return 0;
}
