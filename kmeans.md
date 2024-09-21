# K-Means Clustering Implementation

**20 September 2024**

This project is an excuse to get acclimated to `Eigen`. I plan to create a simple k-means clustering function to cluster a group of data points represented by an Eigen matrix.

## Assumptions

1. The data must be packaged as an Eigen MatrixXd. Until I learn templates, I assume the `double` type for the coefficients. Since we tend to write a given data point horizontally (i.e. `(x, y, z, ...)`, each row of the matrix corresponds to one data point. Therefore, an input collection of data is:

```cpp
MatrixXd m = {
    {},  // D_0
    {},  // D_1
    {},  // D_2
  //...
    {}   // D_{n-1}
}
```

2. Assume the input matrix is read-only. 

I don't want to guarantee that the input matrix is unmodified, so operations *cannot* mutate it.

3. Allow specifiable dimensions

- To begin with, specify how many columns are to be taken into consideration.
- Ultimately, enable specifying *which* columns (ex. {0, 2, 5}).

## Return Data

Successful case: 
    
At the time of writing, there're two primary pieces of data to return: the cluster label of each data point and the cluster centroids. 

Since the labels and centroids are both coupled to the input data points themselves, I'm not sure if I should 
package the input data together with the k-means data (requiring ownership of the input data) or simply require the user to maintain the association themselves. I may try the latter first.

- Should the centroids be packaged as an `Eigen` matrix?
- Should the labels be a column vector?

### Prototype

```cpp
#include <Eigen/Dense>
using Eigen::MatrixXd;

struct KMeansRet {
    MatrixXd d_centroids;   // each row is a cluster centroid
    VectorXi d_labels;      // The ith entry maps the ith row of the input matrix to a cluster
}
```

Note that this solution implicitly requires that the order of the data remain static. Sorting the rows will result in losing the cluster associations. 

One way to combat this is to append the cluster labels to the input matrix as an additional column, but that requires building a new matrix entirely which could be costly for large data.

I have no measurements though, so I can't make a conclusive judgement.

## Error handling

In case of error, what to do? Do I define my own error types? Throw exceptions? This may be something that will need to be decided as I progress through the implementation.

I *could* return an enum that encodes the result cases by including it among the results of the operation...

### Conditions of Failure

1. Invalid arguments
    - Number of dimensions exceeds input matrix column count
    - Threshold is non-positive.
2. MaxIterationError
3. NaN?
    - Need to read up on floating-point arithmetic to understand when this can occur.

## General flow

1. Assign labels
    - For a given data point, calculate its distance to the each centroid. The label is the centroid number with the shortest distance.
2. Calculate new cluster centroids
    - Mean of all intra-cluster vectors.
3. Compare old cluster centroids to new ones 
    - If change is greater than threshold, go to step 1.
    - Else, Stop.

*Note*: In practice, there needs to be a maximum iteration count for cases where the loop is effectively infinite. The counter is checked each iteration and the program will return an error if exceeded.
