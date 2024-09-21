#include "kmeans.h"
#include <Eigen/Dense>
#include <limits>


namespace Classify { namespace KMeans {
    using Eigen::MatrixXd; using Eigen::VectorXi;
    
    const double _eps = std::numeric_limits<double>::epsilon();
    const double SMALLEST_THRESH = 20 * _eps;


    /* KMeansRet */
    KMeansRet::KMeansRet(VectorXi labels, MatrixXd centroids, OpStatus status_code)
        : d_labels(labels), d_centroids(centroids), d_status(status_code) {}
    KMeansRet::KMeansRet(OpStatus status_code)
        : d_status(status_code) {}

    const VectorXi& KMeansRet::labels() const { return d_labels; }
    const MatrixXd& KMeansRet::centroids() const { return d_centroids; }
    const OpStatus& KMeansRet::status() const { return d_status; }

    /* Clustering  */
    // Given data and centroids, assign each data point a label. Labels range from [0..n)
    VectorXi assign_labels(const MatrixXd& data, const MatrixXd& centroids) {
        
    };

    const KMeansRet kcluster(
            const MatrixXd& data,
            unsigned int k,
            unsigned int ndim,
            unsigned int max_iterations,
            double threshold,
            MatrixXd initial_centroids
     ) {
        // Validation
        // NOTE: consider splitting Classify::OpStatus::InvalidInput into multiple specific cases.
        // You'd lose generality, however, and would need to demote it to the Classify::KMeans namespace.
        if (
                threshold < SMALLEST_THRESH ||
                ndim < 1 || ndim > (unsigned int) data.cols() ||
                k < 1 || k > data.rows()
        ) { return KMeansRet(OpStatus::InvalidInput); }

        if (initial_centroids.size() > 0) {
            if (initial_centroids.rows() < k || initial_centroids.cols() < ndim)
                return KMeansRet(OpStatus::InvalidInput); 
        } else {
            // Randomly select centroids w/o replacement
            // Require `k` unique data points
            // 
            long max_range = (long) data.rows();
        }

        
    }



}} // namespace Classify
