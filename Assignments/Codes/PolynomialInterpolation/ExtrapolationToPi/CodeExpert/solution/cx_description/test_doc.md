
# Tests of extrapolpi.hpp

> We test `extrapolate_to_pi()` for $k=5$ and $k=15$. The outputs should be
$3.12532$ and $3.14159$ respectively.
***
> `gram_schmidt(const MatrixXd & A)`: We test this function on the matrix A defined by
```
1 1 1 1
0 1 1 1
0 0 1 1
0 0 0 1
```
which the Gram-Schmidt process transforms into the identity matrix.
The test is passed if the norm( gram_schmidt(A) - Id ) < 10^{-9}.

***

> `orthogonality_test()`: This function is written by the student. It should create a random matrix A, calculate Q = gram_schmidt(A) and return err = norm(Q^TQ - Id).
The test is passed if err < 10^{-9}.