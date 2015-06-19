function [A B C D] = calc_plane(p1, p2, p3)
% Calculates plane coefficients Ax + By + Cz + D = 0
% based on 3 points in 3D space

	v1 = p1 - p2;
	v2 = p3 - p2;
	n = cross(v1, v2);
	normal = n/norm(n);
	A = normal(1);
	B = normal(2);
	C = normal(3);
	D = -A*p1(1) -B*p1(2) -C*p1(3);

end
