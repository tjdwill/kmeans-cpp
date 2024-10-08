// kmeans.h
// Defines the interface for k-means clustering operations 
#ifndef CLASSIFY_K_MEANS

#ifndef EIGEN_DENSE
#define EIGEN_DENSE
#include <Eigen/Dense>
#endif

#ifndef INCLUDED_STD_LIMITS
#define INCLUDED_STD_LIMITS
#include <limits>
#endif

namespace Classify {
    
    /* An enum for encoding general operation results.
     * 
     */
    enum class OpStatus {
        Success,
        InvalidInput,
        MaxIterationsExceeded,
        Unset
    };
    namespace KMeans {
        constexpr double _eps = std::numeric_limits<double>::epsilon();
        constexpr double SMALLEST_THRESH = 20 * _eps;
        /*
         *  The type that composes the results of a k-means clustering operation.
         *  d_labels: 
         *      A vector containing the cluster labels of each row of the input data.
         *  d_centroids:
         *      The calculated centroids. Each row index corresponds to a cluster group.
         *  d_status:
         *      The operation status.
         */
        using Eigen::MatrixXd; using Eigen::VectorXi;


        class ClusterRet {
            const VectorXi d_labels { VectorXi(0) }; 
            const MatrixXd d_centroids { MatrixXd(0, 0) };  // the resultant cluster centroids
            const OpStatus d_status { OpStatus::Unset };

            public:
            /* Constructors, Destructors, Movers */
            // Does this constructor copy the input data?
            // If so, how can I make it so that it's an ownership transfer?
            // Also, I don't actually want the user to be able to construct objects of this type,
            // so how could I restrict the constructor to internal use only?
            ClusterRet(VectorXi labels, MatrixXd centroids, OpStatus status_code);
            ClusterRet(OpStatus status_code);

            /* Methods */
            const VectorXi& labels() const;
            const MatrixXd& centroids() const;
            const OpStatus& status() const;
        };

        /* K-means clustering configuration
         *
         * Params:
         *      data
         *          A matrix of data points. Each data point is a row in the matrix.
         *     
         *      k
         *          Number of clusters into which to group the data
         *
         *      initial_centroids
         *          Allows the user to provide an initial guess for the cluster centroids. If default value is used, the initial centroids are selected
         *          randomly from among the input data. If the centroids cannot be generated within a set number of attempts, a runtime exception is thrown.
         *          If the user *does* provide initial centroids, he/she should be sure all elements are unique.
         *          Checks are not performed for equivalence amongst rows.
         *
         *      ndim
         *          The number of dimensions to cluster (# of columns to take into consideration)
         *
         *      threshold
         *          Granularity of centroid movement. Once all centroidal motion is beneath this value, 
         *          the clustering is considered complete. The lowest value is taken as 20*machine episilon for a double.
         *
         *      max_iterations
         *          user-defined limiter to prevent infinite loops. If this value is exceeded, the function returns the current data with
         *          the status as Classify::OpStatus::MaxIterationsExceeded.
         *
         */
        const ClusterRet cluster(
                const MatrixXd& data,
                int k = 2,
                int ndim = 1,
                int max_iterations = 100,
                double threshold = 20*std::numeric_limits<double>::epsilon(),
                MatrixXd initial_centroids = MatrixXd(0, 0)
         );

        /// Given input data, select elements of data to serve as initial means
        MatrixXd choose_centroids(const MatrixXd& data, int k, int ndim);
        
        /// Given data and centroids, assign each data point a label. Labels range from [0..n)
        VectorXi assign_labels(const MatrixXd& data, const MatrixXd& centroids);

        /// Given data and labels, calculate the centroid of each cluster
        MatrixXd calculate_cluster_centroids(const MatrixXd& data, const VectorXi& labels, int k, int ndim); 
    } // namespace KMeans
} // namespace Classify

#endif /* CLASSIFY_K_MEANS */
