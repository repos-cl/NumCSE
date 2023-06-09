function x = simpnewton(x,F,DF,rtol,atol)
% MATLAB template for simplified Newton method
[L,U] = lu(DF(x)); % one LU-decomposition
s = U\(L\F(x)); x = x-s;
% termination based on relative and absolute tolerance
ns = norm(s); nx = norm(x);
while ((ns > rtol*nx) && (ns > atol))
  s = U\(L\F(x)); x = x-s;
end
