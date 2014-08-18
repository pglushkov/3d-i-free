
% template<typename T> MySquareMatrix<T, 4> CreateAxisRotation(T x, T y, T z, float angle)
% {
        % float rad = my_utils::DegToRad(angle);
        % float COS = cos(rad);
        % float SIN = sin(rad);

        % MySquareMatrix<T, 4> r;

% r[0][0] = COS + (1.0f - COS)*x*x;       r[0][1] = (1.0f - COS)*x*y - SIN*z;     r[0][2] = (1.0f - COS)*x*z + SIN*y;
% r[1][0] = (1.0f - COS)*y*x + SIN*z;     r[1][1] = COS + (1.0f - COS)*y*y;       r[1][2] = (1.0f - COS)*y*z - SIN*x;
% r[2][0] = (1.0f - COS)*z*x - SIN*y;     r[2][1] = (1.0f - COS)*z*y + SIN*x;     r[2][2] = COS + (1.0f - COS)*z*z;

        % return r;
% }

function res = create_axis_rotation(x, y, z, angle)

        res = zeros(3,3);
        COS = cos(angle);
        SIN = sin(angle);

        res(1,1) = COS + (1.0 - COS)*x*x;
        res(1,2) = (1.0 - COS)*x*y - SIN*z;
        res(1,3) = (1.0 - COS)*x*z + SIN*y;

        res(2,1) = (1.0 - COS)*y*x + SIN*z;
        res(2,2) = COS + (1.0 - COS)*y*y;
        res(2,3) = (1.0 - COS)*y*z - SIN*x;

        res(3,1) = (1.0 - COS)*z*x - SIN*y;
        res(3,2) = (1.0 - COS)*z*y + SIN*x;
        res(3,3) = COS + (1.0 - COS)*z*z;

endfunction

