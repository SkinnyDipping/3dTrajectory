function rotationMatrix = angles2RotationMatrix(f, o, k)
% Builds [3x3] rotation matrix
% based on 3 angles

	A=[];
	A(:,1) = [cos(f)*cos(k) -cos(f)*sin(k) sin(f)];
	A(:,2) = [cos(o)*sin(k)+sin(o)*sin(f)*cos(k) cos(o)*cos(k)-sin(o)*sin(f)*cos(k) -sin(o)*cos(f)];
	A(:,3) = [sin(o)*sin(k)+cos(o)*sin(f)*cos(k) sin(o)*cos(k)+cos(o)*sin(f)*sin(k) cos(o)*cos(f)];

	rotationMatrix = A;

end
