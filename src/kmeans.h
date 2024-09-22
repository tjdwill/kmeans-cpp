// kmeans.h
// Defines the interface for k-means clustering operations 
#ifndef CLASSIFY_K_MEANS

#ifndef EIGEN_DENSE
#define EIGEN_DENSE
#include <Eigen/Dense>
#endif /* EIGEN_DENSE */

#ifndef STD_LIMITS
#define STD_LIMITS
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
        _Unset
    };
    namespace KMeans {
        /*
         *  The type that composes the results of a k-means clustering operation.
         *  d_labels: A vector containing the cluster labels of each row of the input data.
         *  d_centroids: 
         */
        using Eigen::MatrixXd; using Eigen::VectorXi;
        struct KMeansRet {
            private:
                const VectorXi d_labels { VectorXi(0) }; 
                const MatrixXd d_centroids { MatrixXd(0, 0) };  // the resultant cluster centroids
                const OpStatus d_status { OpStatus::_Unset };

            public:
            /* Constructors, Destructors, Movers */
            // Does this constructor copy the input data?
            // If so, how can I make it so that it's an ownership transfer?
            // Also, I don't actually want the user to be able to construct objects of this type,
            // so how could I restrict the constructor to internal use only?
            KMeansRet(VectorXi labels, MatrixXd centroids, OpStatus status_code);
            KMeansRet(OpStatus status_code);

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
         *          randomly from among the input data. If the user *does* provide initial centroids, he/she should be sure all elements are unique.
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
        const KMeansRet kcluster(
                const MatrixXd& data,
                unsigned int k = 2,
                unsigned int ndim = 1,
                unsigned int max_iterations = 100,
                double threshold = 20*std::numeric_limits<double>::epsilon(),
                MatrixXd initial_centroids = MatrixXd(0, 0)
         );
        MatrixXd choose_centroids(const MatrixXd& data, unsigned int k, unsigned int ndim);
    } // namespace KMeans
} // namespace Classify

#endif /* CLASSIFY_K_MEANS */
