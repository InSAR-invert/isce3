// -*- C++ -*-
// -*- coding: utf-8 -*-
//
// Author: Joshua Cohen, Bryan Riel
// Copyright 2017-2018
//

#ifndef ISCE_IMAGE_RESAMPSLC_H
#define ISCE_IMAGE_RESAMPSLC_H
#pragma once

#include "forward.h"

#include <cstdint>
#include <cstdio>
#include <complex>
#include <valarray>

// isce3::core
#include <isce3/core/Interpolator.h>
#include <isce3/core/Poly2d.h>

// isce3::io
#include <isce3/io/forward.h>

// isce3::product
#include <isce3/product/Product.h>
#include <isce3/product/RadarGridParameters.h>

// Definition
class isce3::image::ResampSlc {

    // Public data members
    public:
        typedef Tile<std::complex<float>> Tile_t;

    // Meta-methods
    public:
        /** Constructor from an isce3::product::Product (no flattening) */
        inline ResampSlc(const isce3::product::Product & product,
                         char frequency = 'A');

        /** Constructor from an isce3::product::Product and reference product (flattening) */
        inline ResampSlc(const isce3::product::Product & product,
                         const isce3::product::Product & refProduct,
                         char frequency = 'A');

        /** Constructor from an isce3::product::Swath (no flattening) */
        inline ResampSlc(const isce3::product::Swath & swath);

        /** Constructor from an isce3::product::Swath and reference swath (flattening) */
        inline ResampSlc(const isce3::product::Swath & swath,
                         const isce3::product::Swath & refSwath);

        /** Constructor from an isce3::product::RadarGridParameters (no flattening) */
        inline ResampSlc(const isce3::product::RadarGridParameters & rdr_grid,
                         const isce3::core::LUT2d<double> & doppler,
                         double wvl);

        /** Constructor from an isce3::product::RadarGridParameters and reference radar grid (flattening) */
        inline ResampSlc(const isce3::product::RadarGridParameters & rdr_grid,
                         const isce3::product::RadarGridParameters & ref_rdr_grid,
                         const isce3::core::LUT2d<double> & doppler,
                         double wvl, double ref_wvl);

        /** Constructor from individual components (no flattening) */
        inline ResampSlc(const isce3::core::LUT2d<double> & doppler,
                         double startingRange, double rangePixelSpacing,
                         double sensingStart, double prf, double wvl);

        /** Constructor from individual components (flattening) */
        inline ResampSlc(const isce3::core::LUT2d<double> & doppler,
                         double startingRange, double rangePixelSpacing,
                         double sensingStart, double prf, double wvl,
                         double refStartingRange, double refRangePixelSpacing,
                         double refWvl);

        /** Destructor */
        inline ~ResampSlc() {};

        /** Get parameters related to radar grids */
        inline double startingRange() const { return _startingRange; }
        inline double rangePixelSpacing() const { return _rangePixelSpacing; }
        inline double sensingStart() const { return _sensingStart; }
        inline double prf() const { return _prf; }
        inline double wavelength() const { return _wavelength; }
        inline double refStartingRange() const { return _refStartingRange; }
        inline double refRangePixelSpacing() const { return _refRangePixelSpacing; }
        inline double refWavelength() const { return _refWavelength; }

        // Poly2d and LUT getters
        inline isce3::core::Poly2d rgCarrier() const;
        inline isce3::core::Poly2d azCarrier() const;

        // Poly2d and LUT setters
        inline void rgCarrier(const isce3::core::Poly2d &);
        inline void azCarrier(const isce3::core::Poly2d &);

        /** Get read-only reference to Doppler LUT2d */
        inline const isce3::core::LUT2d<double> & doppler() const;

        /** Get reference to Doppler LUT2d */
        inline isce3::core::LUT2d<double> & doppler();

        /** Set Doppler LUT2d */
        inline void doppler(const isce3::core::LUT2d<double> &);

        // Set reference product for flattening
        inline void referenceProduct(const isce3::product::Product & product,
                                     char frequency = 'A');

        // Get/set number of lines per processing tile
        inline size_t linesPerTile() const;
        inline void linesPerTile(size_t);

        /** Get flag for reference data */
        inline bool haveRefData() const { return _haveRefData; }
                
        // Convenience functions
        inline void declare(int, int, int, int) const;

        // Generic resamp entry point from externally created rasters
        void resamp(isce3::io::Raster & inputSlc, isce3::io::Raster & outputSlc,
                    isce3::io::Raster & rgOffsetRaster, isce3::io::Raster & azOffsetRaster,
                    int inputBand=1, bool flatten=false, bool isComplex=true, int rowBuffer=40,
                    int chipSize=isce3::core::SINC_ONE);

        // Generic resamp entry point: use filenames to create rasters
        void resamp(const std::string & inputFilename, const std::string & outputFilename,
                    const std::string & rgOffsetFilename, const std::string & azOffsetFilename,
                    int inputBand=1, bool flatten=false, bool isComplex=true, int rowBuffer=40,
                    int chipSize=isce3::core::SINC_ONE);
        
    // Data members
    protected:
        // Number of lines per tile
        size_t _linesPerTile = 1000;
        // Band number
        int _inputBand;
        // Filename of the input product
        std::string _filename;
        // Flag indicating if we have a reference data (for flattening)
        bool _haveRefData;
        // Interpolator pointer
        isce3::core::Interpolator<std::complex<float>> * _interp;

        // Polynomials and LUTs
        isce3::core::Poly2d _rgCarrier;            // range carrier polynomial
        isce3::core::Poly2d _azCarrier;            // azimuth carrier polynomial
        isce3::core::LUT2d<double> _dopplerLUT;

        // Variables ingested from a Swath
        double _startingRange;
        double _rangePixelSpacing;
        double _sensingStart;
        double _prf;
        double _wavelength;
        double _refStartingRange;
        double _refRangePixelSpacing;
        double _refWavelength;

    // Protected functions
    protected:
        
        // Tile initialization for input offsets
        void _initializeOffsetTiles(Tile_t &, isce3::io::Raster &, isce3::io::Raster &,
                                    isce3::image::Tile<float> &,
                                    isce3::image::Tile<float> &, int);

        // Tile initialization for input SLC data
        void _initializeTile(Tile_t &, isce3::io::Raster &,
                             const isce3::image::Tile<float> &,
                             int, int, int);

        // Tile transformation
        void _transformTile(Tile_t & tile,
                            isce3::io::Raster & outputSlc,
                            const isce3::image::Tile<float> & rgOffTile,
                            const isce3::image::Tile<float> & azOffTile,
                            int inLength, bool flatten,
                            int chipSize);

        // Convenience functions
        inline int _computeNumberOfTiles(int, int);

        // Initialize interpolator pointer
        void _prepareInterpMethods(isce3::core::dataInterpMethod, int);

        // Set radar parameters from an isce3::product::Swath
        inline void _setDataFromSwath(const isce3::product::Swath & swath);

        // Set reference radar parameters from an isce3::product::Swath (for flattening)
        inline void _setRefDataFromSwath(const isce3::product::Swath & swath);
};

// Get inline implementations for ResampSlc
#define ISCE_IMAGE_RESAMPSLC_ICC
#include "ResampSlc.icc"
#undef ISCE_IMAGE_RESAMPSLC_ICC

#endif

// end of file
