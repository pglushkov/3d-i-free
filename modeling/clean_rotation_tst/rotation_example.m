
x = [1 0 0];
xc = [1; 0; 0;];

y = [0 1 0];
yc = [0; 1; 0;];

z = [0 0 1];
zc = [0; 0; 1;];

angle = 90; % deg
angle_rad = angle / 180 * pi;

Rx = generate_rotation_x(angle_rad, 'ROW');
Ry = generate_rotation_y(angle_rad, 'ROW');
Rz = generate_rotation_z(angle_rad, 'ROW');

Rxc = generate_rotation_x(angle_rad, 'COL');
Ryc = generate_rotation_y(angle_rad, 'COL');
Rzc = generate_rotation_z(angle_rad, 'COL');

fprintf('\n\n\n\n*************************************************\n');
fprintf('\n x-column rotated by %f degrees around z : [%f,%f,%f]\n', angle, Rz*xc);
fprintf('\n x-column rotated by %f degrees around y : [%f,%f,%f]\n', angle, Ry*xc);
fprintf('\n z-column rotated by %f degrees around x : [%f,%f,%f]\n', angle, Rx*zc);
fprintf('\n');
fprintf('\n (TWISTED) x-row rotated by %f degrees around z : [%f,%f,%f]\n', angle, x*Rz);
fprintf('\n (TWISTED) x-row rotated by %f degrees around y : [%f,%f,%f]\n', angle, x*Ry);
fprintf('\n (TWISTED) z-row rotated by %f degrees around x : [%f,%f,%f]\n', angle, z*Rx);
fprintf('\n');
fprintf('\n');
fprintf('\n');
fprintf('\n x-row rotated by %f degrees around z : [%f,%f,%f]\n', angle, x*Rzc);
fprintf('\n x-row rotated by %f degrees around y : [%f,%f,%f]\n', angle, x*Ryc);
fprintf('\n z-row rotated by %f degrees around x : [%f,%f,%f]\n', angle, z*Rxc);
fprintf('\n');
fprintf('\n (TWISTED) x-column rotated by %f degrees around z : [%f,%f,%f]\n', angle, Rzc*xc);
fprintf('\n (TWISTED) x-column rotated by %f degrees around y : [%f,%f,%f]\n', angle, Ryc*xc);
fprintf('\n (TWISTED) z-column rotated by %f degrees around x : [%f,%f,%f]\n', angle, Rxc*zc);
fprintf('\n*************************************************\n\n\n\n');
