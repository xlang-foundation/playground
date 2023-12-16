import time
from xlang_image import Factory

@jit
def calc_mandelbrot_set(m:int,n:int,max_iter:int,output_set:any)->bool:
    double re_min = -2.0, re_max = 2.0, im_min = -2, im_max = 2;

    auto mandelbrot = [](double real, double imag, int max_iter) -> int {
        double z_real = real, z_imag = imag;
        for (int n = 0; n < max_iter; ++n) {
            double z_real2 = z_real * z_real, z_imag2 = z_imag * z_imag;
            if (z_real2 + z_imag2 > 4.0) return n;
            z_imag = 2 * z_real * z_imag + imag;
            z_real = z_real2 - z_imag2 + real;
        }
        return max_iter;
    };
    X::Tensor output_tenser = output_set;
    unsigned char* output_data = (unsigned char*)output_tenser->GetData();
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            double real = re_min + (re_max - re_min) * j / n;
            double imag = im_min + (im_max - im_min) * i / m;
            output_data[i*n+j] = (unsigned char)mandelbrot(real, imag, max_iter);
        }
    }
    return true;
w = 800
h = 800
out_tensor = tensor(dtype=tensor.uint8,shape=[h,w])
t1 = time.time()
calc_mandelbrot_set(w,h,100,out_tensor)
t2 = time.time()
t2 = (t2-t1)
print("(${w},${h})->time spend:",t2)
output_img =  Factory.Image("mandelbrot_set_101.jpg")
output_img.from_tensor(out_tensor)
output_img.save()
print("done")
