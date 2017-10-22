## High Quality Signal Generator
To test the signal acquisition pipeline we must generate signals that we can input to check for distortions. The signals we generate must include amplitudes of -70mV to 100mV and frequencies of 1Hz to 40Hz.[1]
To create these very small voltages we will be producing larger voltages and then using resistors to step the voltage down to the desired range. To test frequency performance of our pipeline, we'll need to generate clean single tone frequencies. This is harder to do as simply low pass filtering a PWM squarewave leaves unwanted harmonics in the pipeline. Therefore we'll be using an arduino with better filtering to generate high quality signals. We will be following this guide:

[ARduino DDS Sinewave Generator](http://interface.khm.de/index.php/lab/interfaces-advanced/arduino-dds-sinewave-generator/)

The Arduino code and modifications can be found in  [Arduino_Signal_Generator](./SignalGenerator/Arduino_Signal_Generator)

## References
[1] [Electroencephalography](http://www.bem.fi/book/13/13.htm)
