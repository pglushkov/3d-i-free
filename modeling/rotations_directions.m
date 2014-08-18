
clear all;

V = [1 0 0];

angle = pi/4;
C = cos(angle);
S = sin(angle);

Rx = [1 0 0; 0 C -S; 0 S C];
Ry = [C 0 S; 0 1 0; -S 0 C];
Rz = [C -S 0; S C 0; 0 0 1];

Tr = Rx*Ry*Rz;

Ax = asin(Tr(3,2));
Ay = asin(Tr(1,3));
Az = asin(Tr(2,1));

ROT1 = Rz * Ry

new_y = [0 1 0] * inv(Rz)
ROT2 = Rz * create_axis_rotation(new_y(1), new_y(2), new_y(3), pi/4)
% ROT = create_axis_rotation(0, 0, 1, pi/4)

V * ROT1
V * ROT2
