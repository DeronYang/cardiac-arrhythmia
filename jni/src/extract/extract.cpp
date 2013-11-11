
//remove power-line and high-frequency noise with low pass filter
//12-tap low pass filter with cut-off frequency 35Hz/180Hz
//created by matlab
double fir_coeff[12] = {-0.0011, 0.0047, 0.0307, 0.0882, 0.1623, 0.2152,
                        0.2152, 0.1623, 0.0882, 0.0307, 0.0047, -0.0011};
