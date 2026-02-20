# Lutron Lighting Competition | 1st Place in Class Winner

A real-time audio spectrum visualizer that utilizes Fast Fourier Transforms (FFT) to process ambient sound and animate a custom-built 70-LED matrix. This project was awarded 1st place in a class-wide engineering competition.

##  Technical Highlights

### 1. Digital Signal Processing (DSP)
* **FFT Implementation:** Utilizes the `arduinoFFT` library to convert time-domain microphone samples into frequency-domain magnitudes.
* **Signal Conditioning:** Implemented a **Hamming Window** to reduce spectral leakage and used frequency bin mapping to isolate 7 distinct audio bands for the visualization.
* **Real-time Processing:** Optimized sampling rates to ensure low-latency response between audio input and LED output.

### 2. Algorithmic Matrix Mapping (Serpentine Logic)
To minimize wiring complexity, the LED strip was laid out in a **serpentine (zig-zag)** pattern. Due to wiring constraints every other column is physically inverted, so I developed a custom mapping algorithm to translate a standard (x, y) coordinate system into the correct LED index:

```cpp
if (band % 2 == 0) {
    // Even columns: Top-to-Bottom
    ledIndex = (10 * (band + 1)) - row - 1;
} else {
    // Odd columns: Bottom-to-Top
    ledIndex = (10 * band) + row;
}
