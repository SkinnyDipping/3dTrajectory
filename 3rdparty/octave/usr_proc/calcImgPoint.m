function P = calcImgPoint(cloud_pt, x0, y0, fs, cc, rm)
% Calculates image point
% based on cloud point
% and parameters

	_C = cloud_pt;
	rm = rm^-1;
	P(1) = -fs* ( rm(1,1)*(_C(1)-cc(1)) + rm(2,1)*(_C(2)-cc(2)) + rm(3,1)*(_C(3)-cc(3)) )
	P(1) /= rm(1,3)*(_C(1)-cc(1)) + rm(2,3)*(_C(2)-cc(2)) + rm(3,3)*(_C(3)-cc(3))

	P(2) = -fs* ( rm(1,2)*(_C(1)-cc(1)) + rm(2,2)*(_C(2)-cc(2)) + rm(3,2)*(_C(3)-cc(3)) )
	P(2) /= rm(1,3)*(_C(1)-cc(1)) + rm(2,3)*(_C(2)-cc(2)) + rm(3,3)*(_C(3)-cc(3))

end
