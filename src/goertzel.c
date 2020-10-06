#include <stdint.h>
#include <math.h>

#include "debug.h"
#include "goertzel.h"

void goertzel_init(GOERTZEL_STATE *gp, uint32_t N, double k) {
    // TO BE IMPLEMENTED
	(*gp).N = N;
	(*gp).k = k;
	(*gp).A = (2 *M_PI * (k / N));
	(*gp).B = (2 * cos((*gp).A));
	(*gp).s0 = 0;
	(*gp).s1 = 0;
	(*gp).s2 = 0;
}

void goertzel_step(GOERTZEL_STATE *gp, double x) {
    // TO BE IMPLEMENTED
    double s0 = (x + ( (*gp).B * (*gp).s1)) - (*gp).s2;
    (*gp).s0 = s0;
    (*gp).s2 = (*gp).s1;
    (*gp).s1 = (*gp).s0;
}

double goertzel_strength(GOERTZEL_STATE *gp, double x) {
    // TO BE IMPLEMENTED
    double s0 = x + ( (*gp).B * (*gp).s1) - (*gp).s2;
    double realC = cos( (*gp).A);
    double imagC = sin( (*gp).A);
    double realY = s0 - ( (*gp).s1 * realC);
    double imagY = (-1 * (*gp).s1) * imagC;
    double realD = cos( 2 * M_PI * (*gp).k / (*gp).N * ( (*gp).N) -1 );
    double imagD = -1 * sin( 2 * M_PI * (*gp).k / (*gp).N * ( (*gp).N) -1 );

    double imagY1 = (imagY * realD) + (realY * imagD);
    double realY1 = (realY * realD) - (imagY * imagD);

    imagY = fabs(imagY1);
    realY = fabs(realY1);
    double finalValY = 2 * ( (realY1 * realY1) + (imagY1 * imagY1)) / ( (*gp).N * (*gp).N);
    debug("%f\n", finalValY);
    return finalValY;
}
