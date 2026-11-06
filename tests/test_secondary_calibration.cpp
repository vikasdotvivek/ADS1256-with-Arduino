#include "../SecondaryCalibration.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace {
int assertions = 0;

void check(bool condition, const char* name)
{
    ++assertions;
    if (!condition) {
        std::cerr << "FAIL: " << name << '\n';
        std::exit(EXIT_FAILURE);
    }
}

void near(float actual, double expected, const char* name)
{
    const double tolerance = 1.0e-6 * (1.0 + std::fabs(expected));
    check(std::isfinite(actual) && std::fabs(actual - expected) <= tolerance, name);
}
}

int main()
{
    using ads1256_calibration::secondaryChannel;
    near(secondaryChannel(0.0f), 0.0, "zero endpoint");
    near(secondaryChannel(-4.9f), 2.5, "calibrated endpoint");
    near(secondaryChannel(-2.45f), 1.25, "fractional midpoint");
    near(secondaryChannel(-0.049f), 0.025, "small fractional reading");
    near(secondaryChannel(4.9f), -2.5, "opposite sign is preserved");
    near(secondaryChannel(-9.8f), 5.0, "existing extrapolation is preserved");
    near(secondaryChannel(-490000.0f), 250000.0, "100000x endpoint");
    near(secondaryChannel(-245000.0f) / 100000.0f, 1.25,
         "existing serial output scaling");

    for (int i = -1000; i <= 1000; ++i) {
        const float input = static_cast<float>(i) * 0.0049f;
        near(secondaryChannel(input), static_cast<double>(input) * (2.5 / -4.9),
             "sweep matches the intended floating-point gain");
    }

    // Invalid readings must not be silently converted into a plausible zero.
    check(std::isnan(secondaryChannel(std::numeric_limits<float>::quiet_NaN())),
          "NaN remains invalid");
    check(std::isinf(secondaryChannel(std::numeric_limits<float>::infinity())),
          "infinity remains invalid");

    std::cout << "PASS: " << assertions << " calibration assertions\n";
    return EXIT_SUCCESS;
}
