#include "../src/kmeans.h"
#include <Eigen/Dense>

using Eigen::MatrixXd; using Eigen::VectorXi;

int main() {
    Eigen::Matrix3d m {
        {1.0, 2.0, 3.0},
        {2.0, 4.0, 6.0},
        {3.0, 6.0, 9.0}
    };

    VectorXi labels = VectorXi::Zero(3);
    MatrixXd centroids = Classify::KMeans::calculate_cluster_centroids(m, labels, 1, 3);
    assert(m(1, Eigen::all).isApprox(centroids));
}
