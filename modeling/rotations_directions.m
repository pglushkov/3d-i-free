
clear all;

V = [1 0 0];

angle = pi/3;
C = cos(angle);
S = sin(angle);

Rx = [1 0 0; 0 C -S; 0 S C];
Ry = [C 0 -S; 0 1 0; 0 S C];
Rz = [C -S 0; S C 0; 0 0 1];

Res = V*Rx*Ry*Rz

Tr = Rx*Ry*Rz;

Ax = asin(Tr(3,2));
Ay = asin(Tr(1,3));
Az = asin(Tr(2,1));

len = 1.0; # suppose that our vector has unit-length...

