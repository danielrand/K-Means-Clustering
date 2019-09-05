//  Project 7: K Means Clustering
//  Created by Daniel Rand on 3/31/19.

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class Point {
    
private:
    
    int Xcoord, Ycoord, label;
    double distance;
    
public:
    
    Point (int x, int y, double d) {
        Xcoord = x;
        Ycoord = y;
        distance = d;
    }
    
    Point () {
        
    }
    
    int getLabel () {
        return label;
    }
    
    void setLabel (int l) {
        label = l;
    }
    
    void setXcoord (int x) {
        Xcoord = x;
    }
    
    int getX () {
        return Xcoord;
    }
    
    void setYcoord (int y) {
        Ycoord = y;
    }
    
    int getY () {
        return Ycoord;
    }
    
    double getD () {
        return distance;
    }
    
    void setDistance (double d) {
        distance = d;
    }
};

class KMeans {
    
private:
    
    int K, numPoints, numRows, numCols, minVal, maxVal, change;
    int ** imgAry;
    Point *pointSet;
    Point *Kcentroids;
    
public:
    
    KMeans (int k, int numR, int numC, int minV, int maxV, int **array, Point *centroids) {
        K = k;
        numRows = numR;
        numCols = numC;
        minVal = minV;
        maxVal = numC;
        imgAry = array;
        Kcentroids = centroids;
    }
    void setNumPoints(int num) {
        numPoints = num;
    }
    
    int extract2DPts (ifstream &inFile, ofstream &outFile1) {
        numPoints = 0;
        int num;
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                inFile >> num;
                if (num > 0) {
                    numPoints++;
                    outFile1 << i << " " << j << endl;
                }
            }
        }
        pointSet = new Point [numPoints];
        return numPoints;
    }
    
    void loadPointSet (ifstream &outFile1) {
        if (outFile1.is_open()) {
            int row, col;
            for (int i = 0; i < numPoints; i++) {
                outFile1 >> row >> col;
                pointSet[i] = Point(row, col,9999.0);
            }
        }
    }
    
    void assignLabel () {
        int front = 0, back = numPoints -1, label = 1;
        while (front <= back) {
            pointSet[front].setLabel(label);
            front++;
            label++;
            pointSet[back].setLabel(label);
            back--;
            label++;
            if (label > K)
                label = 1;
        }
    }
    
    void kMeansClustering (ofstream &outFile2) {
        int iteration = 0;
        assignLabel();
        do {
            point2Image(outFile2);
            prettyPrint(outFile2, iteration);
            change = 0;
            computeCentroids();
            int index = 0;
            while (index < numPoints) {
                distanceMinLabel(pointSet[index]);
                index++;
            }
            iteration++;
        } while (change > 0);
    }
    
    void computeCentroids () {
        int *sumX = new int [K+1];
        int *sumY = new int [K+1];
        int *totalPt = new int [K+1];
        for (int i = 0; i < K+1; i++) {
            sumX[i] = 0;
            sumY[i] = 0;
            totalPt[i] = 0;
        }
        int index = 0;
        while (index < numPoints) {
            int label = pointSet[index].getLabel();
            sumX[label] += pointSet[index].getX();
            sumY[label] += pointSet[index].getY();
            totalPt[label]++;
            index++;
        }
        for (int label = 1; label <= K; label++ ) {
            if (totalPt[label] != 0) {
                Kcentroids[label].setXcoord((int)(sumX[label]/totalPt[label]));
                Kcentroids[label].setYcoord((int)(sumY[label]/totalPt[label]));
            }
        }
    }
    
    void distanceMinLabel (Point &p) {
        double minDist = 99999;
        int minLabel = 0;
        int label = 1;
        while (label <= K) {
            int dist = computeDist(p, Kcentroids[label]);
            if (dist < minDist) {
                minLabel = label;
                minDist = dist;
            }
            label++;
        }
        if (p.getLabel() != minLabel) {
            p.setLabel(minLabel);
            p.setDistance(minDist);
            change++;
        }
    }
    
    double computeDist (Point &pt, Point &centroid) {
        int x1 = centroid.getX(), y1 = centroid.getY(), x2 = pt.getX(), y2 = pt.getY();
        return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
    }
    
    void writePtSet (ofstream &outFile3) {
        outFile3 << numPoints << endl;
        outFile3 << numRows << " " << numCols << endl;
        for (int i = 0; i < numPoints; i++) {
            Point p = pointSet[i];
            outFile3 << p.getX() << " " << p.getY() << " " <<  p.getLabel() << endl;
        }
    }
    
    void point2Image (ofstream &outFile2) {
        for (int i = 0; i < numPoints; i++) {
            Point p = pointSet[i];
            imgAry[p.getX()][p.getY()] = p.getLabel();
        }
    }
    
    void prettyPrint (ofstream &outFile2, int iteration) {
        outFile2 << "\n\n************ Result of iteration #" << iteration << " ************" << "\n";
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                if (imgAry[i][j] > 0)
                    outFile2 << imgAry[i][j] << " ";
                else outFile2 << " ";
            } outFile2 << endl;
        }
    }
};

int main(int argc, const char * argv[]) {
    if (argc != 6) {
        cout << "ERROR: Illegal command line arguments!\n\nPlease run again with the following arguments:";
        cout << "\n\n1) inFile\n2) integer K\n3) outFile1\n4) outFile2\n5) outFile3\n\n";
        return 0;
    }
    ifstream inFile;
    ofstream outFile1, outFile2, outFile3;
    int K, numRows, numCols, minVal, maxVal;
    int **imgAry;
    Point *Kcentroids;
    inFile.open(argv[1]);
    if (inFile.is_open()) {
        inFile >> numRows >> numCols >> minVal >> maxVal;
        imgAry = new int*[numCols];
        for (int i = 0; i < numRows; i++) {
            imgAry[i] = new int [numCols];
        }
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j< numCols; j++) {
                imgAry[i][j] = 0;
            }
        }
        outFile1.open(argv[3]);
        outFile2.open(argv[4]);
        outFile3.open(argv[5]);
        istringstream iss( argv[2] );
        iss >> K;
        Kcentroids = new Point [K+1];
        KMeans kMeansCluster (K, numRows, numCols, minVal, maxVal, imgAry, Kcentroids);
        kMeansCluster.extract2DPts(inFile, outFile1);
        outFile1.close();
        ifstream outFile1Read;
        outFile1Read.open(argv[3]);
        kMeansCluster.loadPointSet(outFile1Read);
        kMeansCluster.kMeansClustering(outFile2);
        kMeansCluster.writePtSet(outFile3);
        inFile.close();
        cout << "Success.\n\nOutput can be found in the following files:\n\n";
        cout << argv[3] << endl << argv[4] << endl << argv[5] << endl;
        outFile1Read.close();
        outFile2.close();
        outFile3.close();
    }
    else
        cout << "Unable to open inFile" << endl;
    return 0;
}
