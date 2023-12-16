import numpy as np
import matplotlib.pyplot as plt
import time

def mandelbrot_iter(c_real, c_imag, max_iter):
    z_real, z_imag = 0, 0
    for i in range(max_iter):
        z_real_temp = z_real * z_real - z_imag * z_imag + c_real
        z_imag = 2 * z_real * z_imag + c_imag
        z_real = z_real_temp
        if z_real * z_real + z_imag * z_imag > 4:
            return i
    return max_iter

def mandelbrot_set(m, n, max_iter=100):
    mandelbrot = np.zeros((m, n))
    re_min, re_max = -2, 2
    im_min, im_max = -2, 2

    # Lambda function to call the iterative function
    calculate = lambda c_real, c_imag: mandelbrot_iter(c_real, c_imag, max_iter)

    for x in range(n):
        for y in range(m):
            real = re_min + (x / n) * (re_max - re_min)
            imag = im_min + (y / m) * (im_max - im_min)

            mandelbrot[y, x] = calculate(real, imag)

    return mandelbrot

m, n = 800, 800
t1 = time.time()
mandelbrot_set_array = mandelbrot_set(m, n)
t2 = time.time()
t2 = (t2-t1)
print("(",m,",",n,")","->time spend:",t2)

plt.imshow(mandelbrot_set_array, cmap='hot', extent=[-2, 2, -2, 2])
plt.colorbar()
plt.title("Mandelbrot Set")
plt.xlabel("Re")
plt.ylabel("Im")
plt.show()
print("Done!")