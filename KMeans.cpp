

#include<iostream>
#include<fstream>

#include"Exceptions.h"
#include"Point.h"
#include"Cluster.h"
#include"KMeans.h"

namespace Clustering
{
    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans);
}

using namespace Clustering;

KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) :
        __dimensionality(dim),
        __k(k)
{
    if(k == 0)
        throw ZeroClustersEx();

    __iFileName = filename;
    std::ifstream fin(__iFileName);
    if(filename == "" || !fin)
        throw DataFileOpenEx(__iFileName);

    __maxIter = maxIter;
    __numIter = 0;
    __numNonempty = 1;
    __numMovesLastIter = 1;

    __clusters = new Cluster*[k];
    for(unsigned int i = 0; i < k; i++)
        __clusters[i] = new Cluster(dim);
    fin >> *(__clusters[0]);
    fin.close();

    __initCentroids = new Point*[k];
    for(unsigned int i = 0; i < k; i++)
        __initCentroids[i] = new Point(__dimensionality);

    (*(__clusters[0])).pickCentroids(k, __initCentroids);


}

KMeans::~KMeans()
{
    for(unsigned int i = 0; i < __k; i++)
    {
        delete __clusters[i];
        delete __initCentroids[i];
    }
    delete[] __clusters;
    delete[] __initCentroids;
}

unsigned int KMeans::getMaxIter()
{
    return __maxIter;
}

unsigned int KMeans::getNumIters()
{
    return __numIter;
}

unsigned int KMeans::getNumNonemptyClusters()
{
    return __numNonempty;
}

unsigned int KMeans::getNumMovesLastIter()
{
    return __numMovesLastIter;
}

Cluster& KMeans::operator[](unsigned int k)
{
    return *(__clusters[k]);
}

const Cluster& KMeans::operator[](unsigned int c) const
{
    return *(__clusters[c]);
}

namespace Clustering
{
    std::ostream& operator<<(std::ostream &out, const KMeans &kmeans)
    {
        for(unsigned int i = 0; i < kmeans.__k; i++)
            out << kmeans[i];
        return out;
    }
}
void KMeans::run()
{
    unsigned int moves = 100, iter = 0, nearest = 0, nonempty = 0;
    while( moves > 0 && iter < __maxIter )
    {
        moves = 0;
        for(unsigned int i = 0; i < __k; i++)
            for(unsigned int j = 0; j < (*this)[i].getSize(); j++)
            {
                nearest = 0;
                for(unsigned int k = 0; k < __k; k++)
                {
                    if( (((*this)[i])[j]).distanceTo(*(__initCentroids[k])) < (((*this)[i])[j]).distanceTo(*(__initCentroids[nearest])))
                        nearest = k;
                }

                if(nearest != i)
                {
                    Cluster::Move(((*this)[i])[j], (*this)[i], (*this)[nearest]).perform();
                    moves++;
                }

            }
        nonempty = 0;
        for(unsigned int i = 0; i < __k; i++)
        {
            if(!(*this)[i].centroid.isValid())
                (*this)[i].centroid.compute();
            if((*this)[i].getSize() != 0)
                nonempty++;
        }
        __numIter = iter;
        __numMovesLastIter = moves;
        __numNonempty = nonempty;
    }
}