#ifndef ADS1256_SECONDARY_CALIBRATION_H
#define ADS1256_SECONDARY_CALIBRATION_H

namespace ads1256_calibration {

// The sketch's zero-offset calibration is -4.9 input -> 2.5 output.
// Keep the gain floating-point: Arduino map() truncates its arguments to long.
// This multiplicative mapping also preserves the sketch's existing 100000x
// internal scaling. It deliberately does not clamp readings or hide NaN/Inf.
// Sensor validity and physical range checks belong to the acquisition layer.
inline float secondaryChannel(float reading)
{
    return reading * (2.5f / -4.9f);
}

} // namespace ads1256_calibration

#endif
