# Code improvement review

Review date: 2026-11-06. Scope: the tracked sketch `vikas_ads1256_8channel.ino` and repository layout. This is a source review, not a hardware qualification. No calibration or acquisition behavior is changed by this commit.

## Findings

1. **Fractional mapping is truncated.** The three `map(sensorN, -4.9, 0, 2.5, 0)` calls use Arduino's integer `long map(long, ...)` API. The fractional endpoints and readings are converted to integers. Replace with a tested floating-point interpolation helper only after confirming the intended input/output units. Verify endpoint, midpoint, negative, and out-of-range cases. Reference: https://github.com/arduino/reference-en/blob/master/Language/Functions/Math/map.adoc
2. **Scaling is implicit.** `vRef = 2.5 * 100000` is passed to a constructor whose comment describes volts, and output is subsequently divided by 100000. Mapping uses -4.9 and 2.5 before that division. Document and test one consistent unit conversion; changing only the map function does not establish physically correct calibration.
3. **Conversion/channel association needs a fixture.** Each `setChannel()` precedes a read annotated as belonging to the previous channel. Pin the exact ADS1256 library and test this pipeline against known input voltages; do not reorder calls based only on the comments.
4. **Unbounded readiness waits.** `waitDRDY()` occurs four times in every loop. Check the actual dependency's timeout behavior and define an observable acquisition fault for a disconnected ADC rather than allowing an indefinite wait.
5. **Make the example build reproducibly.** Add the target board/FQBN, exact ADS1256 dependency/version, wiring, and a compile command. The repository presently supplies the sketch and README rather than a complete dependency manifest.

## Acceptance tests

Use a host-side test for interpolation, then compile with the declared Arduino core and library. On the bench verify all four differential inputs, open/disconnected DRDY, negative full-scale input, output units, and achievable 9600-baud sample throughput. Do not describe the code as deployment-ready until those tests pass.
