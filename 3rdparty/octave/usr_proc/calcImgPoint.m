function P = calcImgPoint(cloud_pt, x0, y0, fs, cc, rm)
% Calculates image point
% based on cloud point
% and parameters

	_C = cloud_pt;
	rmi = rm^-1;

%	l = -fs / ( rmi(3,1)*(_C(1)-cc(1)) + rmi(3,2)*(_C(2)-cc(2)) + rmi(3,3)*(_C(3)-cc(3)) );

%	P(1) = l * ( rmi(1,1)*(_C(1)-cc(1)) + rmi(1,2)*(_C(2)-cc(2)) + rmi(1,3)*(_C(3)-cc(3)) );
%	P(2) = l * ( rmi(2,1)*(_C(1)-cc(1)) + rmi(2,2)*(_C(2)-cc(2)) + rmi(2,3)*(_C(3)-cc(3)) );

	P(1) = -fs* ( rmi(1,1)*(_C(1)-cc(1)) + rmi(2,1)*(_C(2)-cc(2)) + rmi(3,1)*(_C(3)-cc(3)) );
	P(1) /= rmi(1,3)*(_C(1)-cc(1)) + rmi(2,3)*(_C(2)-cc(2)) + rmi(3,3)*(_C(3)-cc(3));

	P(2) = -fs* ( rmi(1,2)*(_C(1)-cc(1)) + rmi(2,2)*(_C(2)-cc(2)) + rmi(3,2)*(_C(3)-cc(3)) );
	P(2) /= rmi(1,3)*(_C(1)-cc(1)) + rmi(2,3)*(_C(2)-cc(2)) + rmi(3,3)*(_C(3)-cc(3));

end
