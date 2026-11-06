# Verified improvement: retain fractional calibration

## Source finding

The sketch passed `-4.9` and `2.5` to Arduino's integer `map()` for sensor channels 2, 3, and 4. The standard signature takes `long` arguments, truncating those endpoints to -4 and 2. It also discards fractional input/output information.

Reference: Arduino's official language reference, https://docs.arduino.cc/language-reference/en/functions/math/map/ .

## Change

Replace those three calls with `ads1256_calibration::secondaryChannel()`. The original intended mapping has zero at both input and output, so the floating-point linear map reduces to `reading * (2.5f / -4.9f)`.

The existing 100000x internal scaling, ADC multiplexer order, sensor 1, UART baud rate, output delimiters and five-decimal serial formatting are unchanged. There is no new clamping, timing, motor control or fault-recovery policy. NaN/infinite inputs remain invalid rather than being disguised as zero.

## Verification

The exact helper was compiled with host GCC, C++11, `-Wall -Wextra -Werror -pedantic` and the undefined-behavior sanitizer. All 2011 assertions passed: endpoints, fractional midpoint, small signals, extrapolation, sign, the existing 100000x scale, a 2001-point sweep and invalid-input propagation.

A read-only GitHub Actions workflow repeats the host test and checks that the sketch calls the tested helper. It does not compile the complete sketch or execute ADC/hardware code.

## Still required

Build against the intended Arduino board/core and exact ADS1256 library. Confirm the deployed calibration and channel ordering using reference signals before use. The unusual scaled `vRef`, hardware acquisition timing and physical calibration accuracy were not changed or qualified by this arithmetic test. Do not treat this branch as a hardware-certified release.
