# K-Means CPP

In order to improve my C++ and grow comfortable using [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page), I am re-implementing the basics of my [kmeans](https://github.com/tjdwill/kmeans) Python packge in C++. Like its predecessor, this implementation also provides the ability to specify how many dimensions to take into consideration.

## Remarks

This project didn't take as long as I imagined (ironic, given that [my last project](https://github.com/tjdwill/rustrev) took longer than expected). Eigen was really approachable for my current use-case, and its documentation was thorough. Some operations weren't as easy as Numpy, but I was able to get around it by changing how I thought about the problem. For example, in the Python version, I actually group the data together by cluster, something that seemed like it would have been unproductive to do this time. Instead, I require the data to remain static (read-only) and associate data entry point with a label. The user will then need to determine how they would like the process these labels. I actually prefer this line-of-thinking because the user is assured their data remains unchanged, a lesson learned from my brief time with Rust.

### Error handling

Since writing a few projects in Rust as well as venturing into C a little, I have a better sense of the complexities of error handling. In Python, throwing Exceptions is the standard, but in the more low-level languages, it seems that encoding the error, its reason, and possibly the context are preferred—especially when writing a library. Keeping this in mind, I decided that I would return a status code along with the cluster-specific data. 

Something I wasn't quite sure about, however, is how specific each variant within the `enum class` should be. Currently, I keep it general (ex. `Classify::OpStatus::InvalidInput`), but I considered breaking that into more specific variants. The downside of doing so, however, is that user has more cases to consider for switching on the enum. However, keeping it more general requires that the user determine the cause of the incorrect input error, which isn't necessarily the most user-friendly.

A common theme when learning Rust, C, and C++ is that error handling requires considerable and deliberate thought. In the future, I plan to explore more of C++'s error handling capabilities to learn the more "idiomatic" approach. An area of focus will be finding a way to propagate errors that transpired in helper functions (see the `Classify::KMeans::choose_centroids` function for an example of why; there's a big TODO there).

### Performance

For this project, performance was less of a priority than gaining experience, but I still thought about the former throughout this process. Many of the operations require iterating over the rows of matrices, but I didn't assume RowMajorOrder in terms of layout. Also, anecdotally, a matrix with 2000 elements was clustered into four groups @ 0.01 threshold in 0.182s. When bumping that to 2,000,000 elements, the time jumped to 1 minute 12 seconds. Increasing the threshold to 0.1 brought that time down to between 22-35s. 

The problem is, I'm not sure constitutes "fast". Personally, I'd be okay with processing two million data points in a minute (assuming it's only done once), but other users may have different needs. 
If performance is to be improved, here are some areas to consider:

- Efficient data layout and access
- initial centroids generation (esp. finding a better way to choose without replacement; my custom way works, but there's likely a better solution out there).
- centroid calculation (can this be vectorized?)
- label assignment

Honestly, there's still a lot to learn before I can make proper judgements on performance, so I'll look forward to that in the future. 

## Conclusion

This was a great project, and I am now more comfortable:

- importing, linking, and learning a new library
- organizing a compilation component (John Lakos-inspired) to implement some desired functionality
- designing user interfaces
  - I *do*, however, want to find a way to emulate Python's keyword-based argument passing.
