// assign_labels.cpp
// Tests the label assignment function.
#include "../src/kmeans.h"
#include <Eigen/Dense>
#include <iostream>

int main() {
    Eigen::MatrixXd m = Eigen::MatrixXd::Random(10, 3);
    Eigen::MatrixXd centroids = Classify::KMeans::choose_centroids(m, 4, 3);
    Eigen::VectorXi labels = Classify::KMeans::assign_labels(m, centroids);

    std::cout << "Original Matrix:\n" << m << "\n"
        << "\nCentroids:\n" <<  centroids << "\n";
    std::cout << "\nLabels:\n" << labels << "\n";
}
