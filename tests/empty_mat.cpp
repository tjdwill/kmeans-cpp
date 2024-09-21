#include <iostream>
#include <Eigen/Dense>

int main() {
    using Eigen::MatrixXd;
    MatrixXd m(0,0); 
    MatrixXd j {}; 
    assert(m.size() == j.size());
    std::cout << "Empty matrix:\n" << m << "\n";

    // blocks
    
    Eigen::Matrix4d a = Eigen::Matrix4d::Identity();
    std::cout << "4x4 mat:\n" << a << "\n";
    Eigen::Matrix3d block = a.block<3,3>(0, 0);
    std::cout << "3x3 Block mat:\n" << block << "\n";
    block(0,0) = 27;
    std::cout << "Modified 3x3:\n" << block << "\n";
    std::cout << "4x4 Block mat:\n" << a << "\n";
}
