#include<iostream>
#include <string>
#include "Exceptions.h"



using namespace Clustering;

//OutOfBoundsEx

OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) :
        __current(c),
        __rhs(r),
        __name("OutOfBoundsEx")
{}

unsigned int OutOfBoundsEx::getCurrent() const
{
    return __current;
}

int OutOfBoundsEx::getRhs() const
{
    return __rhs;
}

std::string OutOfBoundsEx::getName() const
{
    return __name;
}

//DimensionalityMismatchEx

DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) :
        __current(c),
        __rhs(r),
        __name("DimensionalityMismatchEx")
{}

unsigned int DimensionalityMismatchEx::getCurrent() const
{
    return __current;
}

unsigned int DimensionalityMismatchEx::getRhs() const
{
    return __rhs;
}

std::string DimensionalityMismatchEx::getName() const
{
    return __name;
}


//ZeroClustersEx

ZeroClustersEx::ZeroClustersEx() :
        __name("ZeroClustersEx")
{}

std::string ZeroClustersEx::getName() const
{
    return __name;
}


//DataFileOpenEx

DataFileOpenEx::DataFileOpenEx(std::string filename) :
        __name("DataFileOpenEx"),
        __filename(filename)
{}

std::string DataFileOpenEx::getFilename() const
{
    return __filename;
}

std::string DataFileOpenEx::getName() const
{
    return __name;
}

//ZeroDimensionsEx
ZeroDimensionsEx::ZeroDimensionsEx() :
        __name("ZeroDimensionsEx")
{}

std::string ZeroDimensionsEx::getName() const
{
    return __name;
}


//EmptyClusterEx

EmptyClusterEx::EmptyClusterEx() :
        __name("EmptyClusterEx")
{}

std::string EmptyClusterEx::getName() const
{
    return __name;
}
namespace Clustering //friends:wq
{

    std::ostream& operator<<(std::ostream &os, const OutOfBoundsEx &ex)
    {
        os << ex.getName() << " (current = " << ex.getCurrent() << ", rhs = " << ex.getRhs() << ')';
        return os;
    }

    std::ostream& operator<<(std::ostream &os, const DimensionalityMismatchEx &ex)
    {
        os <<  ex.getName() << " (current = " << ex.getCurrent() << ", rhs = " << ex.getRhs() << ')';
        return os;
    }

    std::ostream& operator<<(std::ostream &os, const ZeroClustersEx &ex)
    {
        os << ex.getName();
        return os;
    }

    std::ostream& operator<<(std::ostream &os, const DataFileOpenEx &ex)
    {
        os << ex.getName() << ": filename = " << ex.getFilename();
        return os;
    }

    std::ostream& operator<<(std::ostream &os, const ZeroDimensionsEx &ex)
    {
        os << ex.getName();
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const EmptyClusterEx &ex)
    {
        os << ex.getName();
        return os;
    }
}