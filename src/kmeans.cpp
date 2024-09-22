#include "kmeans.h"
#include <Eigen/Dense>
#include <experimental/random>
#include <limits>
#include <set>
// #include <iostream>

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
    // VectorXi assign_labels(const MatrixXd& data, const MatrixXd& centroids) {};

    /// Given input data, select elements of data to serve as initial means
    /// Due to how this is implemented, it is not possible to have a centroid be the origin.
    MatrixXd choose_centroids(const MatrixXd& data, unsigned int k, unsigned int ndim) {
        // TODO: I don't want to use longs if not needed. Can I get away with [u]ints?
        unsigned long max_range = (unsigned long) data.rows();
        assert(max_range > 0);
        const auto MAX_ATTEMPTS = std::max(max_range/10,  (unsigned long) 10000);
        
        // Create a set of row indices. As we attempt to fill in the centroid matrix, we want to simulate
        // selection rows without replacement. This is done by removing items from the set.
        std::set<unsigned long> choices {};
        assert(choices.empty());
        for (unsigned long i = 0; i < max_range; ++i)
           choices.insert(i);

        MatrixXd centroids = MatrixXd::Zero(k, ndim);
        unsigned long centroid_rows_chosen = 0;
        unsigned int num_attempts = 0;

choose_row:
        while (centroid_rows_chosen < k && num_attempts <= MAX_ATTEMPTS) {
            // try to find a unique centroid point.
            // The row indices are contained in the `choices` set.
            // Choose a random index of the *set* to then select a random *row index*
            ++num_attempts;
            unsigned long rand_idx = std::experimental::randint((unsigned long) 0, choices.size());
            unsigned long j = 0;  // is there an `enumerate` method for C++ iterators?
            for (auto key : choices) {
                if (j == rand_idx) {
                    Eigen::MatrixXd centroid = data(key, Eigen::all);
                    // check currently-found centroids to see if this row is already present
                    for (unsigned long row = 0; row < centroid_rows_chosen; ++row){
                        // NOTE: Threshold can be decreased down to _eps if need be.
                        if (centroid.isApprox(centroids(row, Eigen::all), SMALLEST_THRESH)) {
                            // Ensure this key isn't chosen it again.
                            choices.erase(key);
                            // Try again.
                            goto choose_row;
                        }
                    }
                    centroids(centroid_rows_chosen, Eigen::all) = centroid;
                    ++centroid_rows_chosen;
                    choices.erase(key);
                    break;
                } else {
                    ++j;
                }
            }
        }

        if (num_attempts > MAX_ATTEMPTS) {
            // TODO:
            // return error code?
            // would need to change return type..
        }
       
        return centroids;
    };
    
//
//    const KMeansRet kcluster(
//            const MatrixXd& input_data,
//            unsigned int k,
//            unsigned int ndim,
//            unsigned int max_iterations,
//            double threshold,
//            MatrixXd initial_centroids
//     ) {
//        /* Validation */
//        // NOTE: consider splitting Classify::OpStatus::InvalidInput into multiple specific cases.
//        // You'd lose generality, however, and would need to demote it to the Classify::KMeans namespace.
//        if (input_data.size() == 0) { return KMeansRet(OpStatus::Success); }
//        MatrixXd data = input_data.block(input_data.rows(), ndim, 0,0);
//
//        if (
//                threshold < SMALLEST_THRESH ||
//                ndim < 1 || ndim > (unsigned int) data.cols() ||
//                k < 1 || k > data.rows()
//        ) { return KMeansRet(OpStatus::InvalidInput); }
//
//        if (initial_centroids.size() == 0) {
//            // Randomly select centroids w/o replacement
//            initial_centroids = choose_centroids(data, k, ndim);
//        } else if (initial_centroids.rows() < k || initial_centroids.cols() < ndim) {
//            return KMeansRet(OpStatus::InvalidInput); 
//        }
//
//        
//    }
//
//

}} // namespace Classify
