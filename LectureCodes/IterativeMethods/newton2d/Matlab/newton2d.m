F=@(x) [x(1)^2-x(2)^4; x(1)-x(2)^3];
DF=@(x) [2*x(1),-4*x(2)^3;1,-3*x(2)^2];
x=[0.7;0.7]; x_ast=[1;1]; tol=1E-10;

res=[0,x',norm(x-x_ast)];
s = DF(x)\F(x); x = x-s;
res = [res; 1,x',norm(x-x_ast)]; k=2;
while (norm(s) > tol*norm(x))
  s = DF(x)\F(x); x = x-s;
  res = [res; k,x',norm(x-x_ast)];
  k = k+1;
end

ld = diff(log(res(:,4))); %\label{newt:14}
rates = ld(2:end)./ld(1:end-1); %\label{newt:15}
