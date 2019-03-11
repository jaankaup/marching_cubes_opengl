u = rand (32, 32);
t = linspace (-1, 1, 32);
[x, y] = meshgrid (t, t);

a = 4;
z = exp (- a .* x .^ 2) .* exp (- a .* y .^ 2);
b = 128;
k = exp (- b .* x .^ 2) .* exp (- b .* y .^ 2);
w = conv2 (u, k, 'same') .* z;

figure (1); clf (); surf (x, y, z);
figure (2); clf (); contour (w); colorbar ();
figure (3); clf (); imshow (abs (w) > median (median (w)));
