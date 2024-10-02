#include "kmeans.h"
#include <Eigen/Dense>
#include <experimental/random>
#include <limits>
#include <set>
#include <stdexcept>

namespace Classify { namespace KMeans {
    using Eigen::MatrixXd; using Eigen::VectorXi;
    
    /* ClusterRet */
    ClusterRet::ClusterRet(VectorXi labels, MatrixXd centroids, OpStatus status_code)
        : d_labels(labels), d_centroids(centroids), d_status(status_code) {}
    ClusterRet::ClusterRet(OpStatus status_code)
        : d_status(status_code) {}

    const VectorXi& ClusterRet::labels() const { return d_labels; }
    const MatrixXd& ClusterRet::centroids() const { return d_centroids; }
    const OpStatus& ClusterRet::status() const { return d_status; }

    /* Clustering  */
    // Given data and centroids, assign each data point a label. Labels range from [0..n)
    VectorXi assign_labels(const MatrixXd& data, const MatrixXd& centroids) {
        VectorXi labels = VectorXi::Zero(data.rows());
        int row_idx = 0;
        for (auto row: data.rowwise()) {
            double distance = std::numeric_limits<double>::max();
            int i = 0;
            int label = -1;
            for (auto centroid: centroids.rowwise()) {
                auto dist = (row - centroid).norm();
                if (dist < distance) {
                   label = i;  
                   distance = dist;
                }
                ++i;
            }
            labels(row_idx) = label;
            ++row_idx;
        }
        return labels;
    }

    /// Given input data, select elements of data to serve as initial means
    MatrixXd choose_centroids(const MatrixXd& data, int k, int ndim) {
        long max_range = (long) data.rows();
        assert(max_range > 0);
        const auto MAX_ATTEMPTS = std::max(max_range/10,  (long) 10000);
        
        // Create a set of row indices. As we attempt to fill in the centroid matrix, we want to simulate
        // selection rows without replacement. This is done by removing items from the set.
        std::set<long> choices {};
        assert(choices.empty());
        for (long i = 0; i < max_range; ++i)
           choices.insert(i);

        MatrixXd centroids = MatrixXd::Zero(k, ndim);
        long centroid_rows_chosen = 0;
        int num_attempts = 0;

choose_row:
        while (centroid_rows_chosen < k && num_attempts <= MAX_ATTEMPTS) {
            // Try to find a unique centroid point.
            // The row indices are contained in the `choices` set.
            // Choose a random index of the *set* to then select a random *row index*
            ++num_attempts;
            // NOTE: This conversion is reasonable because it's incredibly unlikely that the number 
            // of rows in a matrix exceeds 4_294_967_295, the max value of a 32-bit integer.
            long rand_idx = (long) std::experimental::randint((unsigned long) 0, choices.size());
            // assert(rand_idx > 0);
            long j = 0;  // is there an `enumerate` method for C++ iterators?
            for (auto key : choices) {
                if (j == rand_idx) {
                    Eigen::MatrixXd centroid = data(key, Eigen::all);
                    // check currently-found centroids to see if this row is already present
                    for (long row = 0; row < centroid_rows_chosen; ++row){
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
            throw std::runtime_error("Could not generate centroids.");
        }
       
        return centroids;
    };
    
    /// Given data and labels, calculates centroids for a given cluster.
    MatrixXd calculate_cluster_centroids(const MatrixXd& data, const VectorXi& labels, int k, int ndim) {
        MatrixXd centroids = Eigen::MatrixXd::Zero(k, ndim);
        VectorXi clust_count = Eigen::VectorXi::Zero(k);
        for (long i = 0; i < labels.size(); ++i) {
            int label = labels(i);
            centroids(label, Eigen::all) += data(i, Eigen::all);
            clust_count(label) += 1;
        }
        for (int i = 0; i < clust_count.size(); ++i) {
            int num_entries = clust_count(i);
            assert(num_entries != 0);
            centroids(i, Eigen::all) /= num_entries;
        }
        return centroids;
    }

    const ClusterRet cluster(
            const MatrixXd& input_data,
            int k,
            int ndim,
            int max_iterations,
            double threshold,
            MatrixXd initial_centroids
     ) {
        /* Validation */
        // NOTE: consider splitting Classify::OpStatus::InvalidInput into multiple specific cases.
        // You'd lose generality, however, and would need to demote it to the Classify::KMeans namespace.
        if (input_data.size() == 0) { return ClusterRet(OpStatus::Success); }

        // PERF: Is there a way to make this a view instead of a copy?
        MatrixXd data = input_data.block(0, 0, input_data.rows(),ndim);
        MatrixXd cluster_centroids;

        if (
                max_iterations < 0 ||
                threshold < SMALLEST_THRESH ||
                ndim < 1 || ndim > (int) data.cols() ||
                k < 1 || k > data.rows()
        ) { return ClusterRet(OpStatus::InvalidInput); }

        if (initial_centroids.size() == 0) 
            // Randomly select centroids w/o replacement
            cluster_centroids = choose_centroids(data, k, ndim);
        else if (initial_centroids.rows() != k || initial_centroids.cols() < ndim)
            return ClusterRet(OpStatus::InvalidInput); 
        else 
            cluster_centroids = initial_centroids.block(0, 0, initial_centroids.rows(), ndim);

        
        bool success = false;
        VectorXi labels;
        for (int count = 0; count < max_iterations; ++count) {
            labels = assign_labels(data, cluster_centroids);
            MatrixXd next_centroids = calculate_cluster_centroids(data, labels, k, ndim);
            Eigen::VectorXd centroid_motion = (next_centroids - cluster_centroids).rowwise().norm();
            auto check = (centroid_motion.array() > threshold);
            if (check.any()) {
                cluster_centroids = next_centroids;
            } else {
                success = true;
                break;
            }
        }
        if (success) {
            return ClusterRet(labels, cluster_centroids, OpStatus::Success);
        } else {
            return ClusterRet(labels, cluster_centroids, OpStatus::MaxIterationsExceeded);
        }
    }  // cluster
}} // namespace Classify
