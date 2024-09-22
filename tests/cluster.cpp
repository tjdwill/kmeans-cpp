#include "../src/kmeans.h"
#include <Eigen/Dense>
#include <iostream>

int main() {
    using Eigen::MatrixXd; using Eigen::VectorXd;
    using Classify::KMeans::cluster; using Classify::KMeans::ClusterRet; using Classify::OpStatus;

    MatrixXd m = MatrixXd::Random(2000000, 4);
    // std::cout << "Input Matrix:\n" << m << "\n\n";
    std::cout << "Input Matrix: (rows, col) = (" << m.rows() << ", " << m.cols() << ")"  << "\n\n";
    unsigned int k=4, ndim=3;
    double threshold = 0.01;
    ClusterRet cluster_data = cluster(m, k, ndim, 100, threshold);
    bool max_iters = false;
    switch (cluster_data.status()) {
        case OpStatus::Success:
            break;
        case OpStatus::MaxIterationsExceeded:
            max_iters = true;
            break;
        default:
            // Bad Input
            std::cerr << "Invalid input to clustering function" << "\n";
            exit(1);
    }

    std::cout << "After clustering with (k, ndim, thresh) = (" << k << ", " << ndim << ", " << threshold << "):" << "\n";
    // std::cout << "Labels: " << cluster_data.labels().transpose() << "\n\n";
    std::cout << "Labels (sz): " << cluster_data.labels().size() << "\n\n";
    std::cout << "Centroids:\n" << cluster_data.centroids() << "\n";
    std::cout << "Max Iterations Exceeded (t/f): " << max_iters << "\n" ;
}

/* ./kmeans.x
 *
 *
Input Matrix: (rows, col) = (2000000, 4)

After clustering with (k, ndim, thresh) = (4, 3, 0.1):
Labels (sz): 2000000

Centroids:
-0.610247 -0.156257 0.0233411
 0.413138 -0.251761  -0.64187
 0.154971  0.619561 0.0927479
 0.379774 -0.551921   0.42839
Max Iterations Exceeded (t/f): 0

real    0m28.558s
user    0m28.301s
sys     0m0.253s
*/
