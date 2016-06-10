template<typename MatType>
void blockAccess(Eigen::MatrixBase<MatType> &M)
{
  using index_t = typename Eigen::MatrixBase<MatType>::Index;
  using entry_t = typename Eigen::MatrixBase<MatType>::Scalar;
  const index_t nrows(M.rows()); // No. of rows 
  const index_t ncols(M.cols()); // No. of columns
  
  cout << "Matrix M = " << endl << M << endl; // Print matrix
  // Block size half the size of the matrix
  index_t p = nrows/2,q = ncols/2; 
  // Output submatrix with left upper entry at position {\sf (i,i)}
  for(index_t i=0; i < min(p,q); i++)
    cout << "Block (" << i << ',' << i << ',' << p << ',' << q 
         << ") = " << M.block(i,i,p,q) <<  endl;
  // l-value access: modify sub-matrix by adding a constant
  M.block(1,1,p,q) += Eigen::MatrixBase<MatType>::Constant(p,q,1.0);
  cout << "M = " << endl << M << endl;
  // r-value access: extract sub-matrix
  MatrixXd B = M.block(1,1,p,q);
  cout << "Isolated modified block = " << endl << B << endl;
  // Special sub-matrices
  cout << p << " top rows of m = " << M.topRows(p) << endl;
  cout << p << " bottom rows of m = " << M.bottomRows(p) << endl;
  cout << q << " left cols of m = " << M.leftCols(q) << endl;
  cout << q << " right cols of m = " << M.rightCols(p) << endl;
  // r-value access to upper triangular part
  const MatrixXd T = M.template triangularView<Upper>(); // \Label{ba:l}
  cout << "Upper triangular part = " << endl << T << endl; 
  // l-value access to upper triangular part
  M.template triangularView<Lower>() *= -1.5; // \Label{ba:2}
  cout << "Matrix M = " << endl << M << endl;
}