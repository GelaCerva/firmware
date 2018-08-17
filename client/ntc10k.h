#include <avr/pgmspace.h>
#include <math.h>


double thermistor2Temp(int raw_adc) {
    double temp;
    temp = log(10000.0*((1024.0/raw_adc-1)));
    //   =log(10000.0/(1024.0/RawADC-1)) // for pull-up configuration
    temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
    temp = temp - 273.15; // Convert Kelvin to Celcius
    //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    return temp;
}
