# Problems

## GaussianQuadrature (8-1)
- integrate() function changed to be implemented by student as it is part of the problem (derived from statement in solution); is optional
- small comment changes

## ImproperIntegrals (8-3)
- small comment changes

## NestedQuad (8-4)
- small comment changes
- made some variables const/constexpr

## WeightedGaussQuadrature (8-6)
- ported to CodeExpert and matplotlibcpp
- there was a small error in how tabulating was done (indexing)

## GaussPts (8-8)
- ported to CodeExpert
- **Someone should check if recursive formula for p from b is correctly implemented.** In my eyes, there is a minus sign missing in the exponent.

## EffQuadSingInt (8-9)
- ported to CodeExpert
- made more comments
- subproblem (e) changed to also plot, visual convergence study is better than just observing convergence on a table **CHANGE IN LATEX**

## ZerosLegendre (8-10)
- ported to CodeExpert
- Latex needs to be updated.
- currently implemented for n >= 2, added asserts
- There was an error in the regula falsi implementation: if statement used no brackets which was wrong because of two preceeding statements.
