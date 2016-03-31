#include<fstream>
#include<sstream>
#include<algorithm>
#include<limits>

#include"Cluster.h"
#include"Point.h"
#include"Exceptions.h"

using namespace Clustering;

const char Cluster::POINT_CLUSTER_ID_DELIM = ':';

unsigned int Cluster::__idGenerator = 0;

//LNode
LNode::LNode(const Point& p, LNodePtr n) : point(p)
{
    next = n;
}

//Cluster

Cluster::Cluster(unsigned int d) :
        __dimensionality(d),
        __size(0),
        __points(NULL),
        __id(++Cluster::__idGenerator),
        centroid(d, *this)
{};

Cluster::Cluster(const Cluster& c) :
        __dimensionality(c.__dimensionality),
        __size(0),
        __points(NULL),
        __id(c.__id),
        centroid(__dimensionality, c)

{
    LNode* adder = c.__points;
    for(unsigned int i = 0; i < c.__size; i++)//add in all the points from the new guy
    {
        this->add(adder->point);
        adder = adder->next;
    }
    centroid.compute();
}

Cluster& Cluster::operator=(const Cluster& c)
{
    __id = c.__id;
    if(*this == c)
        return *this;

    if(__size != 0)
    {
        LNode* remover = this->__points;
        for(unsigned int i = 0; i <= __size+1; i++)//remove all points from what it used to be
        {
            this->remove(remover->point);
            remover = remover->next;
        }
    }

    LNode* adder = c.__points;
    for(unsigned int i = 0; i < c.__size; i++)//add in all the points from the new guy
    {
        this->add(adder->point);
        adder = adder->next;
    }

    centroid.set(c.centroid.get());
    return *this;
}

Cluster::~Cluster()
{
    LNodePtr index, keeper;
    index = __points;

    for(unsigned int i = 0; i < __size; i++)
    {
        keeper = index->next;
        delete index;
        index = keeper;
    }
}

unsigned int Cluster::getSize() const
{
    return __size;
}

unsigned int Cluster::getDimensionality() const
{
    return __dimensionality;
}

unsigned int Cluster::getId() const
{
    return __id;
}

void Cluster::add(const Point& p)
{
    if(p.getDims()!= __dimensionality )
        throw DimensionalityMismatchEx(__dimensionality, p.getDims());
    if(this->contains(p))
        return void();
    if(__size==0)
    {
        LNodePtr node;
        node = new LNode(p, NULL);
        __points = node;
    }

    else if(p < (*this)[0] || ((p <= (*this)[0])&&(p >= (*this)[0])&&(p.getId() < this->getId())))
    {
        LNodePtr node;
        node = new LNode(p, __points);
        __points = node;
    }

    else //anywhere else
    {
        LNodePtr index = __points;
        LNodePtr back;
        while(p > index->point || ((index->point <= p)&&(index->point >= p)&&((index->point).getId() < p.getId())))
        {
            if(index->next == NULL)
                break;
            back = index;
            index = index->next;
        }
        LNodePtr node;
        if(index->next == NULL)//end
        {
            node = new LNode(p, NULL);
            index->next = node;
        }
        else
        {
            node = new LNode(p, index);
            back->next = node;
        }
    }
    centroid.setValid(false);

    ++__size;
}

const Point &Cluster::remove(const Point& p)
{
    if(p.getDims() != __dimensionality)
        throw DimensionalityMismatchEx(__dimensionality, p.getDims());
    if(this->contains(p))
    {
        LNodePtr killer = __points;
        if(p.getId() == __points->point.getId())
        {
            __points = __points->next;
            delete killer;
            __size--;
        }
        else
        {
            LNodePtr back;
            while(p.getId() != killer->point.getId())
            {
                back = killer;
                killer = killer->next;
            }
            back->next = killer->next;
            delete killer;
            __size--;
        }
        this->centroid.setValid(false);
    }
    return p;
}

bool Cluster::contains(const Point& p) const
{
    LNode* finder = __points;
    for(unsigned int i = 0; i < __size; i++)
    {
        if(p == finder->point)
            return true;
        finder = finder->next;
    }

    return false;
}

const Point& Cluster::operator[](unsigned int index) const
{
    if(__size == 0)
        throw EmptyClusterEx();
    if(index >= __size || index < 0)
        throw OutOfBoundsEx(__size, index);
    LNodePtr holder;
    holder = __points;
    for(unsigned int i = 0; i < index; i++)
        holder = holder->next;
    return holder->point;
}

Cluster& Cluster::operator+=(const Point& p)
{
    this->add(p);
    return *this;
}

Cluster& Cluster::operator-=(const Point& p)
{
    this->remove(p);
    return *this;
}

Cluster& Cluster::operator+=(const Cluster& c)
{
    if(c.__dimensionality != __dimensionality)
        throw DimensionalityMismatchEx(__dimensionality, c.__dimensionality);
    LNode* adder = c.__points;
    for(unsigned int i = 0; i < c.__size; i++)
    {
        this->add(adder->point);
        adder = adder->next;
    }
    return *this;
}

Cluster& Cluster::operator-=(const Cluster& c)
{
    if(c.__dimensionality != __dimensionality)
        throw DimensionalityMismatchEx(__dimensionality, c.__dimensionality);
    LNode* remover = c.__points;
    for(unsigned int i = 0; i < c.__size; i++)
    {
        this->remove(remover->point);
        remover = remover->next;
    }
    return *this;
}

void Cluster::pickCentroids(unsigned int k, Point **pointArray)
{
    if(k < __size)
        for(unsigned int i = 0; i < k; i++)
        {
            unsigned int j = int(i*__size/k);
            *pointArray[i] = (*this)[j];
        }
    else if(k >= __size)
    {
        for(unsigned int i = 0; i < k; i++)
        {
            if(i < __size)
                *(pointArray[i]) = (*this)[i];

            else
                for(unsigned int j = 0; j < __dimensionality; j++)
                    (*(pointArray[i]))[j] = std::numeric_limits<double>::max();
        }
    }
}

//Centroid

Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) :
        __dimensions(d),
        __p(d),
        __valid(false),
        __c(c)
{
}

const Point Cluster::Centroid::get() const
{
    return __p;
}

void Cluster::Centroid::set(const Point &p)
{
    __p = p;
    __valid = true;
}

bool Cluster::Centroid::isValid() const
{
    return __valid;
}

void Cluster::Centroid::setValid(bool valid)
{
    __valid = valid;
}

void Cluster::Centroid::compute()
{
    if(__c.__size == 0)
    {
        toInfinity();
        __valid = true;
        return void();
    }
    for(unsigned int i = 0; i < __p.getDims(); i++)
        __p[i] = 0;


    LNode* index = __c.__points;
    for(unsigned int i = 0; i < __c.__size; i++)
    {
        __p += index->point / __c.__size;
        index = index->next;
    }
    __valid = true;
}

bool Cluster::Centroid::equal(const Point& p) const
{
    for(unsigned int i = 0; i < p.getDims(); i++ )
        if(__p[i] != p[i])
            return false;
    return true;
}

void Cluster::Centroid::toInfinity()
{
    for(unsigned int i = 0; i < __p.getDims(); i++)
        __p[i] = std::numeric_limits<double>::max();
}
//Move

Cluster::Move::Move(const Point&p, Cluster &from, Cluster &to) :
        __p(p),
        __from(from),
        __to(to)
{}

void Cluster::Move::perform()
{
    __to.add(__p);
    __from.remove(__p);
}

namespace Clustering //friends
{

    std::ostream& operator<<(std::ostream& os, const Cluster& c)
    {
        LNode* output = c.__points;
        for(unsigned int i = 0; i < c.__size; i++)
        {
            os << output->point << ' ' << Cluster::POINT_CLUSTER_ID_DELIM << ' ' << c.__id << std::endl;
            output = output->next;
        }
        return os;
    }

    std::istream& operator>>(std::istream& is, Cluster& c)
    {
        std::string str;
        while( getline(is, str))
        {
            std::stringstream ss(str);

            try
            {
                Point * p = new Point(c.__dimensionality);//I would make the new point here
                ss >> *p;
                c.add(*p);
            }
            catch (DimensionalityMismatchEx& ex)
            {
                std::cerr << "Caught exception: " << ex << std::endl;
                Point::rewindIdGen();
            }
        }
        return is;
    }

    bool operator==(const Cluster& c, const Cluster& d)
    {
        if(c.__dimensionality != d.__dimensionality)
            throw DimensionalityMismatchEx(c.__dimensionality, d.__dimensionality);
        if(c.__size != d.__size)
            return false;

        LNode* lhs = c.__points, *rhs = d.__points;
        for(unsigned int i = 0; i < c.__size; i++)
        {
            if(lhs->point != rhs->point)
                return false;
            lhs = lhs->next;
            rhs = rhs->next;
        }
        return true;
    }

    bool operator!=(const Cluster& c, const Cluster& c1)
    {
        return !(c == c1);
    }

    const Cluster operator+(const Cluster& c, const Cluster& c1)
    {
        Cluster newc(c);
        newc+=c1;
        return newc;
    }

    const Cluster operator-(const Cluster& c, const Cluster& c1)
    {
        Cluster newc(c);
        newc-= c1;
        return newc;
    }


    const Cluster operator+(const Cluster& c, const Point& p)
    {
        Cluster newc(c);
        newc += p;
        return newc;
    }


    const Cluster operator-(const Cluster& c, const Point& p)
    {
        Cluster newc(c);
        newc-=p;
        return newc;
    }

}