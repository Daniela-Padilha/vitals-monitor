## PPG Signal Overview

A photoplethysmography (PPG) signal is an optical measurement of how blood volume changes in tissue over time, usually obtained by shining light into the skin and measuring how much is absorbed or reflected. Its pulsatile shape is driven by the cardiac cycle: the waveform rises during systole as arterial blood volume increases, then falls during diastole as the pressure wave dissipates. Secondary features, such as the dicrotic notch, appear because of vascular dynamics and wave reflections associated with aortic valve closure and the elastic response of the arterial system.

## Sampling Requirements for PPG Acquisition

A PPG signal reflects cyclical blood-volume changes in peripheral tissue caused by cardiac activity. Even under extreme exertion, human heart rate rarely exceeds about 200 BPM, or roughly 3–4 beats per second; however, diagnostically relevant waveform features such as the dicrotic notch occur on much shorter time scales within each pulse. For that reason, reliable morphology capture requires a sampling rate substantially higher than the minimum Nyquist limit, which is why devices such as the MAX30102 are typically configured in the 50–400 Hz range depending on the acquisition mode and fidelity requirements.

## MAX30102 Operating Modes

The MAX30102 supports three primary operating modes: **Heart Rate mode** (red LED only), **SpO2 mode** (red and infrared LEDs), and **Multi-LED mode**. Each mode changes both the sensing objective and the underlying hardware behavior, since the device controls different LED activation sequences and acquires different optical channels depending on the selected configuration. Understanding when to use each mode is important for system design: Heart Rate mode is suitable for basic pulse monitoring, SpO2 mode enables oxygen-saturation estimation through dual-wavelength measurement, and Multi-LED mode is used when more flexible or extended optical sampling is required.

## Deterministic and Portable Simulation Design

In the simulator, using `srand(42)` guarantees deterministic behavior by producing the same noise sequence on every execution. This becomes especially important once scenario-based testing is introduced through a `--scenario` flag, because reproducible noise allows outputs to be compared consistently across runs. As a result, the simulation becomes easier to validate, debug, and benchmark in a controlled engineering workflow.

In production-grade C code, fixed-width integer types such as `int64_t` from `<stdint.h>` should be preferred to platform-dependent types like `long`. This guarantees consistent integer size across systems, which is essential for portability and correctness, since `long` may be 32-bit on some platforms and 64-bit on others.

## Pulse Waveform Modeling

The systolic peak can be modeled as:

```
systolic_peak = e^(-k_rise * (t - t_peak)^2)
```

This Gaussian formulation provides independent control over both the peak position and the pulse width, which makes it more suitable than a simple sine wave when asymmetric waveform shaping is required.

The dicrotic bump can be modeled as:

```
dicrotic_bump = A_d * sin(2*pi * f_d * (t - t_notch)) * e^(-d * (t - t_notch))
```

This damped sine formulation captures the small oscillatory feature that follows the systolic phase while allowing its amplitude and decay rate to be tuned independently.

The slow return to baseline can be modeled as:

```
baseline_decay = e^(-k_fall * t)
```

This exponential decay term represents the gradual relaxation of the waveform back to baseline after the main pulse components.

| Constant  | What it controls                     | Starting value |
|-----------|--------------------------------------|----------------|
| `k_rise`  | Steepness of systolic rise           | `30.0`         |
| `t_peak`  | Where in the cycle the peak sits     | `0.15`         |
| `A_d`     | Height of dicrotic bump              | `0.15`         |
| `t_notch` | Where the dicrotic notch sits        | `0.45`         |
| `f_d`     | Frequency of the dicrotic bump       | `2.5`          |
| `d`       | How fast the dicrotic bump damps     | `10.0`         |
| `k_fall`  | Speed of diastolic decay             | `3.0`          |

## Separation Between Raw PPG Signal and Derived BPM

In the MAX30102 simulation path, `out->value` stores the **raw optical signal amplitude** produced by `ppg_model`, rather than the heart rate in BPM. This raw value represents the normalized photoplethysmography waveform itself, typically varying between `0.0` and `1.0` as each heartbeat changes the measured blood-volume-related light absorption. In other words, `ppg_model` generates the sensor-like optical signal, not the interpreted physiological metric.

The **BPM value is derived later** by the signal-processing stage in `filter.c`, where a peak detection algorithm analyzes the waveform over time to estimate heart rate. This separation is an important design decision because it keeps **signal acquisition** independent from **signal interpretation**. By modeling the raw sensor output first and extracting BPM afterward, the architecture more closely reflects how a real MAX30102-based system works: the sensor provides optical samples, and higher-level software is responsible for filtering, peak detection, and heart-rate estimation.

Architecturally, this separation matters because it enforces a cleaner division of responsibility between system layers. The sensor model is responsible only for generating realistic acquisition data, while the filtering layer is responsible for transforming that data into clinically meaningful metrics. This makes the system easier to maintain, because changes to the BPM estimation algorithm do not require modifications to the acquisition model, and changes to the waveform model do not force redesign of the downstream analysis pipeline.

It also improves **modularity** and **testability**. The raw waveform can be validated independently to confirm that the simulator produces realistic optical samples, while the peak-detection algorithm can be tested separately using known inputs and expected BPM outputs. This layered design is especially valuable in embedded systems, where acquisition, filtering, and feature extraction are often developed and verified as distinct components.

Finally, separating raw signal generation from BPM derivation increases **realism** and **extensibility**. Real optical sensors do not output BPM directly; they output sampled light-intensity data, and heart rate must be inferred through processing. Preserving that distinction makes the simulation more faithful to actual hardware behavior and allows additional downstream algorithms—such as smoothing, motion-artifact rejection, arrhythmia detection, or SpO2 estimation—to be added later without changing the acquisition interface.