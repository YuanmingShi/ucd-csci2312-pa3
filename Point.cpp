//
// Created by Yuanming on 3/29/2016.
//

// Define the function in the point.h

#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <algorithm>
#include <cassert>
#include "Exceptions.h"
#include "Point.h"

namespace Clustering {

    unsigned int Point::__idGen = 0;
    const char Point::POINT_VALUE_DELIM = ',';

    void Point::rewindIdGen()
    {
        __idGen--;

    }

    Point::Point(unsigned int __newDim) //constructor
    {
        __id = 0;
        __id = __idGen++;
        __dim = __newDim;
        __values = new double[__dim];

        for (int i = 0; i < __dim; i++) {
            __values[i] = 0;
        }
        if (__newDim == 0) throw ZeroDimensionsEx();
    }

    Point::Point(unsigned int num, double *newVal) {

        __id = __idGen;
        ++__idGen;
        __dim = num;
        __values = new double[__dim];

        for (int i = 0; i < __dim; ++i) {
            __values[i] = newVal[i];
        }
    }

//copy constructor
    Point::Point(const Point &p) {
        __dim = p.__dim;
        __id = p.__id;
        __values = new double[__dim];

        for (int i = 0; i < __dim; i++) {
            __values[i] = p.__values[i];
        }
    }

//overloaded operator=
    Point &Point::operator=(const Point &p) {
        if(__dim != p.__dim)
            throw DimensionalityMismatchEx(__dim, p.__dim);
        __id = p.__id;
        if(*this == p)
            return *this;

        for(unsigned int i = 0; i < __dim; i++)
            this->__values[i] = p[i];
        return *this;

    }

//destructor
    Point::~Point() {

        delete[] __values;

    }

// Accessors & mutators
    int Point::getId() const {
        return __id;
    }

    unsigned int Point::getDims() const {
        return __dim;
    }

    double Point::getValue(unsigned int i) const {

        if (Point::__dim <= i) {
            throw Clustering::OutOfBoundsEx(__dim, i);
        }

        return __values[i];
    }

    void Point::setValue(unsigned int i, double n) {
        if (i >= __dim)
            throw OutOfBoundsEx(__dim, i);
        __values[i] = n;
    }


//functions
    double Point::distanceTo(const Point &point) const {
        if(point.__dim != __dim)
        throw DimensionalityMismatchEx(__dim, point.__dim);

        double dist = 0;

        for (int i = 0; i < __dim; i++) {
            dist += pow((__values[i] - point.getValue(i)), 2);
        }

        return sqrt(dist);
    }

//overload operator
    Point &Point::operator*=(double n) {
        for (unsigned int i = 0; i < __dim; i++) {
            __values[i] = __values[i] * n;
        }

        return *this;

    }

    Point &Point::operator/=(double n) {
        for (int i = 0; i < __dim; ++i)
            __values[i] /= n;

        return *this;
    }

    const Point Point::operator*(double n) const {
        Point p(*this);
        p *= n;
        return p;
    }

    const Point Point::operator/(double n) const {
        Point p(*this);
        p /= n;

        return p;
    }

    double &Point::operator[](unsigned int index) {
        if( index >= __dim || index < 0)
            throw OutOfBoundsEx(__dim, index);
        return __values[index];
    }

    const double &Point::operator[](unsigned int index) const
    {
        if(index > __dim)
            throw OutOfBoundsEx(__dim, index);
        return __values[index];
    }

// Friends
    Point &operator+=(Point &lhs, const Point &rhs) {
        if(lhs.__dim != rhs.__dim)
            throw DimensionalityMismatchEx(lhs.__dim,rhs.__dim);

        for (unsigned int i = 0; i < lhs.__dim; i++) {
            lhs.__values[i] += rhs.__values[i];
        }

        return lhs;

    }

    Point & operator-=(Point &lhs, const Point &rhs)
    {
        if(lhs.__dim != rhs.__dim)
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);

        for (unsigned int i = 0; i < lhs.getDims(); i++)
        {
            lhs.__values[i] -= rhs.__values[i];
        }

        return lhs;
    }

    const Point operator+(const Point &lhs, const Point &rhs) {
        Point p(lhs);
        p += rhs;
        return p;
    }

    const Point operator-(const Point &lhs, const Point &rhs) {
        Point p(lhs);
        p -= rhs;
        return p;
    }

    bool operator==(const Point &lhs, const Point &rhs) {
        if (lhs.__id != rhs.__id)
            return false;
        bool isEqual = true;
        for (int i = 0; i < lhs.__dim; i++) {
            if (lhs.__values[i] != rhs.__values[i]) {
                isEqual = false;
            }
        }
        return isEqual;
    }

    bool operator!=(const Point &lhs, const Point &rhs) {

        return !(lhs == rhs);
    }

    bool operator<(const Point &lhs, const Point &rhs) {

        if(lhs.__dim != rhs.__dim)
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);

        for(unsigned int i = 0; i < lhs.getDims(); i++)
            if(lhs[i] != rhs[i])
                return lhs[i] < rhs[i];
        return false;
    }

    bool operator>(const Point &lhs, const Point &rhs) {

        return rhs < lhs;
    }


    bool operator<=(const Point &lhs, const Point &rhs) {

        return !(lhs > rhs);
    }

    bool operator>=(const Point &lhs, const Point &rhs) {

        return !(lhs < rhs);
    }


    std::ostream &operator<<(std::ostream &os, const Point &p)
    {
        for(unsigned int i = 0; i < p.getDims() -1; i++)
            os << p[i] << Point::POINT_VALUE_DELIM << ' ';
        os << p[p.getDims()-1];

        return os;
    }

    std::istream &operator>>(std::istream &is, Point &p)
    {

        std::string input;
        std::getline(is, input, ':');
        unsigned int dims = (unsigned int) std::count(input.begin(), input.end(), Point::POINT_VALUE_DELIM) + 1;
        if(p.__dim != dims)
            throw DimensionalityMismatchEx(p.__dim, dims);

        std::stringstream ss(input);

        std::string str;
        for(unsigned int i = 0; i < p.__dim; i++)
        {
            std::getline(ss, str, Point::POINT_VALUE_DELIM);
            p[i] = std::stod(str);
        }
        return is;
    }
}