# Problems

# TripletToCRS (2-13)
- ported to CodeExpert
- no tests

# Sylvester (2-18)
- Didn't change much, just basic cleaup.
- Latex links not checked yet.

# SparseCCS (2-14)
- could change type of row_ind and col_ptr to integer vectors
- changed tests to test against Eigen and not the mastersolution

# Ellpack (2-15)
- changed Triplet_new type to Triplet
- some cosmetics
- template throws exception because vector is accessed even though not having a size; **should main handle this exception?**

# GridFunction (2-16)
- some cosmetics

# MatMatCOO (2-17)
- cosmetics, more comments
- replaced random matrix generation
- added plotting using matplotlibcpp in main
- for runtime reasons this should not be timed for such big matrix sizes

# Sylvester (2-19)
- some cosmetics