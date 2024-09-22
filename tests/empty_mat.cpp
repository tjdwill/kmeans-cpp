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
    // Results: Turns out that generating a block creates a new structure instead of a view into the parent.
    std::cout << "Modified 3x3:\n" << block << "\n";
    std::cout << "4x4 Block mat:\n" << a << "\n";

    // Try building a matrix with push operator
    // Update: Didn't work. Instead, create an empty array 
    // and fill in each row as needed.
    Eigen::Matrix4d build = Eigen::Matrix4d::Zero();
    Eigen::Vector4d arr = Eigen::Vector4d::LinSpaced(1, 16);
    for (int i = 0; i < 4; ++i) {
        build(i, Eigen::all) = arr;
    }

    std::cout << "4x4 built mat:\n" << build << "\n";
}
