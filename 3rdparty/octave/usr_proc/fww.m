function [x0 y0 fs center_cast rotation_matrix] = fww(im, cl)
% Performs "fotogrametryczne wcięcie wstecz", whatever that means
% Input - (image_keypoints, cloud_keypoints)
% Output: [x0 y0 fs center_cast rotation_matrix]
% x0, y0 - center of image
% fs - ?
% center_cast - center of cast
% rotation_matrix - rotation of image from XY plane to specific place in 3D space

	A=[];
	for i=1:size(cl,1)
		X=cl(i,1);
		Y=cl(i,2);
		Z=cl(i,3);
		x=im(i,1);
		y=im(i,2);
		A=[A; X Y Z 1 0 0 0 0 -x*X -x*Y -x*Z];
		A=[A; 0 0 0 0 X Y Z 1 -y*X -y*Y -y*Z];
	end
	l=[];
	for i=1:size(im,1)
		l=[l; im(i,1); im(i,2)];
	end
	L=(A'*A)^-1*A'*l;

% values of L from the Osada book
% uncomment to test code
%	L2=[2938.1148;-201.9398;-2.3448;8990.4129;408.3923;592.5297;-2534.5797;5692.8448;0.3242;0.4981;-0.0124];
%	L=L2;

	x0 = ( L(1)*L(9) + L(2)*L(10) + L(3)*L(11) ) / ( L(9)^2 + L(10)^2 + L(11)^2 );
	y0 = ( L(5)*L(9) + L(6)*L(10) + L(7)*L(11) ) / ( L(9)^2 + L(10)^2 + L(11)^2 );
	fs = sqrt( ( (x0*L(9)-L(1))^2 + (x0*L(10)-L(2))^2 + (x0*L(11)-L(3))^2 + (y0*L(9)-L(5))^2 + (y0*L(10)-L(6))^2 + (y0*L(11)-L(7))^2 ) / ( 2* ( L(9)^2 + L(10)^2 + L(11)^2 ) ) );
	center_cast = - [L(1) L(2) L(3) ; L(5) L(6) L(7) ; L(9) L(10) L(11)]^-1 * [L(4);L(8);1];
	_D = -1 / sqrt(L(9)^2 + L(10)^2 + L(11)^2);
	_r = [(x0*L(9)-L(1))/fs (x0*L(10)-L(2))/fs (x0*L(11)-L(3))/fs ; (y0*L(9)-L(5))/fs (y0*L(10)-L(6))/fs (y0*L(11)-L(7))/fs ; L(9) L(10) L(11) ];
	rotation_matrix = _D * _r;

end
