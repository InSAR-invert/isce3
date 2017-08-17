//
// Author: Joshua Cohen
// Copyright 2017
//

#ifndef __ISCE_CORE_ELLIPSOID_H__
#define __ISCE_CORE_ELLIPSOID_H__

#include <cmath>
#include <vector>

namespace isce::core {
    struct Ellipsoid {
        double a;   // Major semi-axis
        double e2;  // Eccentricity squared

        Ellipsoid(double maj, double ecc) : a(maj), e2(ecc) {}  // Value constructor
        Ellipsoid() : Ellipsoid(0.,0.) {}                       // Default constructor (delegated)
        Ellipsoid(const Ellipsoid &e) : a(e.a), e2(e.e2) {}     // Copy constructor
        inline Ellipsoid& operator=(const Ellipsoid&);          // Copy-assignment operator

        inline double rEast(double);
        inline double rNorth(double);
        inline double rDir(double,double);
        void latLon(std::vector<double>&,std::vector<double>&,int);
        void getAngs(std::vector<double>&,std::vector<double>&,std::vector<double>&,double&,double&);
        void getTCN_TCvec(std::vector<double>&,std::vector<double>&,std::vector<double>&,std::vector<double>&);
        void TCNbasis(std::vector<double>&,std::vector<double>&,std::vector<double>&,std::vector<double>&,std::vector<double>&);
    };

    inline Ellipsoid& Ellipsoid::operator=(const Ellipsoid &rhs) {
        a = rhs.a;
        e2 = rhs.e2;
        return *this;
    }

    inline double Ellipsoid::rEast(double lat) { return a / sqrt(1. - (e2 * pow(sin(lat), 2))); }

    inline double Ellipsoid::rNorth(double lat) { return (a * (1. - e2)) / pow((1. - (e2 * pow(lat, 2))), 1.5); }

    inline double Ellipsoid::rDir(double hdg, double lat) {
        auto re = rEast(lat);
        auto rn = rNorth(lat);
        return (re * rn) / ((re * pow(cos(hdg), 2)) + (rn * pow(sin(hdg), 2)));
    }
}

#endif