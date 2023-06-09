function [x,cvg] = dampnewton(x,F,DF,rtol,atol, LMIN)

[L,U] = lu(DF(x));
s = U\(L\F(x));

xn = x-s;
lambda = 1; cvg = 0;

f = F(xn);
st = U\(L\f);
stn = norm(st);

while ((stn>rtol*norm(xn)) && (stn > atol))
  while (norm(st) > (1-lambda/2)*norm(s))
    lambda = lambda/2;
    if (lambda < LMIN), cvg = -1; return; end
    xn = x-lambda*s; f = F(xn); 
    st =  U\(L\f);
  end
  x = xn; [L,U] = lu(DF(x));
  s = U\(L\f);
  lambda = min(2*lambda,1);
  xn = x-lambda*s; f = F(xn);
  st = U\(L\f);
end
x = xn;
