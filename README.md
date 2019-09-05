# K-Means-Clustering

# What does the progam do?
Provided a sample "image" text file consiting of binary values there are k groups, or clusters, of 1's. The first line of the file is the image header containing information about the dimensions and min and max values. The progam will identiy, isolate and label the points from each group, finding the clusters via the k means clustering algorithm. It will print every iteration of the algorithm until the final state, where all groups are properly labeled. This will be outputted to outFile2. See examples below. The other outFiles will contain debugging information about data structures that are being utilized by the algorithm.

## Sample Input
https://github.com/danielrand/K-Means-Clustering/blob/master/Sample_Input/Kmeans_image_data2.txt

## Sample output
https://github.com/danielrand/K-Means-Clustering/blob/master/Sample_Output/outFile2.txt

# How to run
Requires 5 command line arguments:
1) input image in the format outlined in the sample
2) an integer k, denoting the number of clusters in the input image
3) debug output file
4) Algorithm iteration output, and final result
5) debug output file
It should look something like this (using the files provided in sample input for example):
./a.out ../Sample_Input/Kmeans_image_data2.txt 6 ../Sample_Input/outFile1.txt ../Sample_Input/outFile2.txt ../Sample_Input/outFile3.txt 
