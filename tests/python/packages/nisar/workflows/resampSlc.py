#!/usr/bin/env python3
#
# Author: Liang Yu
# Copyright 2019-

import numpy as np
import iscetest
from nisar.workflows import resampSlc

class ResampOpts:
    '''
    class to emulate argparse terminal input
    member values set to test basic functionality
    values can be adjusted to meet test requirements
    '''
    product = iscetest.data + 'envisat.h5'
    frequency = 'A'
    polarization = 'HH'
    offsetdir = iscetest.data + 'offsets'
    outFilePath = 'resampSlc/resampCpuSlc.slc'
    linesPerTile = 249

    gpu = False


def testCpuResamp():
    '''
    run resample SLC without flattening and compare output against golden data
    '''
    # init inputs
    opts = ResampOpts()

    # run resamp
    resampSlc.main(opts)

    # load generated resampled SLC and avoid edges
    test_slc = np.fromfile(opts.outFilePath, dtype=np.complex64).reshape(500,500)[20:-20,20:-20]

    # load reference SLC and avoid edges
    ref_slc = np.fromfile(iscetest.data+'warped_envisat.slc', dtype=np.complex64).reshape(500,500)[20:-20,20:-20]
    
    # get normalized error
    abs_error = np.abs(np.sum(test_slc - ref_slc) / test_slc.size)

    # check error
    assert (abs_error < 1e-6), f'NISAR CPU resamp error {abs_error} > 1e-6'


if __name__ == '__main__':
    testCpuResamp()

# end of file
