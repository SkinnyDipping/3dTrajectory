function P = castPoint(image, cloud, x0, y0, fs, cc, rm)
% Casts point on plane of image based
% on parameters from "fww"
% returns Point coordinates

	T_0 = [x0 y0];
	T_CC = cc;
	Rot = [rm [0;0;0];[0 0 0 1]];

	% Laying img
	Frame = [];
	for i=1:size(image,1)
		Frame(i,:) = image(i,:) - T_0;
	end

	% Distance in Z
	for i=1:size(Frame,1)
		Frame(i,3) = -fs;
		Frame(i,4) = 1;
	end

	% Rotation
	for i=1:size(Frame,1)
		Frame(i,:) = (Rot*Frame(i,:)')';
	end

	% Translation to O
	for i=1:size(Frame,1)
		Frame(i,1:3) = Frame(i,1:3)+T_CC';
	end
	
	disp(Frame(1,1:3));

	% Calc plane coefficients
	[A B C D] = calc_plane(Frame(1,1:3), Frame(2,1:3), Frame(3,1:3));

	% Making a line
	% k - directional vector
	% ptA - point on line
	k = Frame(1,1:3) - cc';
	k /= norm(k);
	p = cc;
	_t = -(A*p(1) + B*p(2) + C*p(3) + D) / (A*k(1) + B*k(2) + C*k(3));
	P = [p(1)+k(1)*_t p(2)+k(2)*_t p(3)+k(3)*_t];

	% Reverse transform
	P -= T_CC';
	P = Rot(1:3,1:3)*P'
	P += [T_0 fs]'


end
