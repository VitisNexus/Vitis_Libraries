.. 
   .. Copyright © 2019–2023 Advanced Micro Devices, Inc

`Terms and Conditions <https://www.amd.com/en/corporate/copyright>`_.

.. meta::
   :keywords: Vitis Sparse Matrix Library, primitive
   :description: The primitives that provide the basic building blocks for CSC format sparse matrix and dense vector multiplication.

.. _L1_intro:

************************************
Primitive Overview
************************************

The L1 primitives provide a range of hardware modules for implementing the multiplication function between a CSC format sparse matrix and a dense vector. The C++ implementation of those modules can be found in the ``include`` directory of the AMD Vitis |trade| sparse library.

.. toctree::
      :maxdepth: 1

1. Scatter-gather logic 
------------------------

The Scatter-gather logic for selecting input dense vector entries is implmented by the L1 primitive ``xBarCol``. For more information, see :ref:`L1_xBarCol`.

2. Row-wise accumulator
-----------------------

The row-wise accumulator is implemened by the L1 primitive ``cscRow``. This primitive basically multiplies the values of multiple NNZ entries with their correponding dense column vector values, and accumulates the results according to the row indices. The basic functions used by this primitive include ``xBarRow``, ``rowMemAcc``, and ``rowAgg``. The ``xBarRow`` primitive includes ``formRowEntry`` logic for multiplying the NNZ values with the corresponding input column vector entries and the ``split``, ``merge`` logic for distributing the multiplication results to the corresponding row banks.  The ``rowMemAcc`` primitives accumulate the intermediate results in on-chip memories. Multiple on-chip memory buffers are provided to remove the floating pointer accumulation bubbles. The ``rowAgg`` primitive collects the results from all accumulators and outputs the results in sequence.

For more information, see :ref:`L1_cscRow`.

3. Buffer and distribute input column vector entries and the column pointers of NNZs
----------------------------------------------------------------------------

The CSC format sparse matrix information is stored in three arrays, namely the array of the NNZs' values, the array of the row indices of NNZs, and the column pointers of the NNZs. To maximize the performance, the storage of the values and row indices of the NNZs can be partitioned into blocks and stored in multiple HBM channels. This storage scheme allows multiple sparse matrix blocks being processed in parallel. The buffering and transmission logic implemented in ``dispCol`` and ``dispNnzCol`` are used to move column vector and pointer blocks to allow multiple sparse matrix blocks being processed in parallel. ``dispColVec`` is the basic component of dispCol.

For more information, see :ref:`L1_dispCol`.

.. |trade|  unicode:: U+02122 .. TRADEMARK SIGN
   :ltrim:
.. |reg|    unicode:: U+000AE .. REGISTERED TRADEMARK SIGN
   :ltrim: