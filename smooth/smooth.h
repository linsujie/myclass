#ifndef _SMOOTH_H
#define _SMOOTH_H
#include "spectrum.h"
#include "interp.h"

// Smooth a spectrum or an interp with a extended Savitzky-Golay method introduced by Peter Gorry.
// [Anal. Chem. 1990, 62, 570-573]
spectrum smooth(const spectrum& spec, int filter_length = 5, int polynomial_order = 4);
spectrum smooth(const interp& intp, double step_size, int filter_length = 5, int polynomial_order = 4);
#endif // for #ifndef _SMOOTH_H
