## SHT31 Sensor Modeling

The **SHT31** is a digital sensor designed to measure **temperature** and **relative humidity**. In this project, it is modeled as a stable environmental sensing device whose output changes gradually over time, in contrast to the **MAX30102**, which captures an optical PPG waveform with faster dynamics and greater susceptibility to noise. For that reason, the simulated SHT31 uses a smaller noise standard deviation, such as `0.01`, which better reflects the higher stability of temperature measurements compared with optical pulse signals.

### Temperature Drift Representation

The temperature model is defined as a **slow sinusoidal drift** around a nominal baseline of **36.5°C**, with a small amplitude and additive Gaussian noise. This approach captures the fact that temperature does not remain perfectly constant, but instead varies smoothly due to factors such as sensor self-heating, environmental changes, airflow, or gradual physiological regulation.

A representative model is:

```text
temperature(t) = 36.5 + A * sin(2πft) + noise
```

In this expression:

- 36.5 is the baseline temperature,

- A is the drift amplitude,

- f is the drift frequency,

- noise represents small random measurement variation.

### Why Elapsed Time Is Used

In the PPG model, the variable `t` represents the position within a single heartbeat cycle. Because it is derived using fmod, it resets to 0.0 at the beginning of each beat, which is what makes the waveform periodic and repeatable.

That behavior is appropriate for pulse simulation, but not for temperature. Temperature does not “restart” at fixed intervals like a heartbeat; instead, it evolves continuously and slowly over time. For that reason, the temperature model should use raw elapsed time, such as elapsed_ms, rather than the cycle-based variable `t`. This preserves continuity and avoids introducing an unrealistic repeating reset into the signal.

### Why a 300-Second Cycle Is Realistic

A period of 300 seconds corresponds to a 5-minute cycle. This is a reasonable timescale for thermal drift because temperature changes are much slower than cardiac signals. In practice, a realistic drift cycle for body or near-body temperature can be modeled within a range of 300 to 600 seconds, or 5 to 10 minutes, depending on how quickly the environment or measurement conditions are expected to vary.

### Why a 0.3°C Amplitude Is Realistic

An amplitude of 0.3°C means the simulated temperature can vary by approximately ±0.3°C around the baseline. This is a practical engineering choice because it produces visible but still plausible drift, without making the sensor output appear unstable or exaggerated. It is large enough to illustrate gradual thermal variation in the simulation, while remaining consistent with realistic short-term body or skin temperature fluctuation.

### Frequency and Period Relationship

The drift frequency is computed from the period using:

```
f = 1.0 / period
```

This works because frequency is defined as the number of cycles per second, while period is the duration of one cycle in seconds. Since these quantities are mathematical reciprocals, dividing 1.0 by the period gives the number of complete cycles that occur each second.

For example, if:

```
period = 300 s
```

then:
```
f = 1.0 / 300 = 0.00333 Hz
```

This means the signal completes one full oscillation every 300 seconds, or every 5 minutes.

### Summary

The SHT31 simulation is therefore modeled as a continuous, low-noise, slowly drifting signal rather than a repeating physiological waveform. Using elapsed_ms instead of cycle-relative time preserves continuity, the sinusoidal drift captures realistic gradual variation, and the small noise term reflects the stability expected from a temperature sensor. This makes the model both physically plausible and consistent with the overall engineering approach used throughout the project.