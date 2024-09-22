#include "../src/kmeans.h"
#include <Eigen/Dense>
#include <iostream>

int main() {
    using Classify::KMeans::choose_centroids;
    Eigen::MatrixXd m = Eigen::MatrixXd::Random(100, 4);    
    // Eigen::MatrixXd m = Eigen::MatrixXd::Zero(100, 4);
    std::cout << "Rand matrix (rows, cols): (" << m.rows() << ", " << m.cols() << ")" << "\n";
    unsigned int ndim = 3;
    Eigen::MatrixXd data = m.block(0, 0, m.rows(), ndim);
    std::cout << "Block matrix (rows, cols): (" << data.rows() << ", " << data.cols() << ")" << "\n";
    Eigen::MatrixXd centroids = choose_centroids(data, 5, 3);
    std::cout << "Calculated centroids:\n" << centroids << "\n";
}
