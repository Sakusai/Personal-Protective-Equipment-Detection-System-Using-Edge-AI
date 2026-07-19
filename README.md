# Personal Protective Equipment Detection System Using Edge AI

**MATON Erwann**

Edge AI system to detect the use of personal protective equipment (PPE) — helmet, safety vest, glasses — in workplaces or industrial environments. Runs locally on a **XIAO ESP32S3** + **Grove Vision AI Module V2** (Himax WiseEye2 NPU), no cloud required.

## Hardware

- Seeed Studio XIAO ESP32S3
- Grove Vision AI Module V2
- CSI camera (already connected to the module)

## Progress

### Step 1 — Module tested with SenseCraft AI (no-code)

Connected the Grove Vision AI V2 alone to SenseCraft AI (https://sensecraft.seeed.cc/ai/model), deployed a pretrained face detection model, and confirmed live detection with bounding boxes in the browser. This validated the camera and module before adding the microcontroller.

### Step 2 — ESP32 software setup (done)

Stack: **C/C++ with PlatformIO (VSCode)**, Arduino framework — required because the `Seeed_Arduino_SSCMA` library (handles I2C communication with the module) only supports Arduino, not MicroPython.

- [x] PlatformIO project set up (`seeed_xiao_esp32s3`, framework `arduino`)
- [x] Dependencies added: `Seeed_Arduino_SSCMA` + `ArduinoJson`
- [x] First test sketch written: reads detections (boxes, scores, coordinates) and prints them to serial
- [x] Build successful
- [x] Upload to the XIAO working (bootloader mode via BOOT button)
- [x] Live detections confirmed over serial (face detection, ~50ms inference time, 70%+ confidence)

### Step 3 — Real-time web dashboard (done)

Stack: `ESPAsyncWebServer` + `AsyncTCP` (async, non-blocking, so the web server doesn't interfere with the detection loop), WebSocket for real-time push (no polling).

Since the available network is Ethernet-only with a captive portal (no usable open Wi-Fi for the XIAO), the XIAO runs in **Wi-Fi Access Point mode**: it creates its own Wi-Fi network, and any device connects directly to it — no internet or router needed.

- [x] XIAO set up as Wi-Fi Access Point (`WiFi.softAP`)
- [x] Async web server serving a single-page dashboard (HTML/CSS/JS embedded in the firmware)
- [x] WebSocket channel (`/ws`) pushing detection results (class, score, coordinates) live to the browser
- [x] Camera snapshot streaming: images captured by the Grove Vision AI V2 are base64-encoded and sent over WebSocket, displayed live in the dashboard
- [x] Verified working end-to-end: detections and camera images both display correctly in the browser in real time

### Next steps

- [ ] Map raw class indices to readable labels (currently shown as numeric `target` values)
- [ ] Train a custom model (Edge Impulse) for actual PPE: helmet, vest, glasses
- [ ] Logic to combine "person" + "PPE" detections into a compliant/non-compliant status
- [ ] Event history / logging on the dashboard
- [ ] System configuration options via the web interface

## Notes

- To enter bootloader mode on the XIAO: hold the **B (BOOT)** button while plugging in the USB-C cable, release once connected. Press **R (RESET)** after flashing to restart normally.
- To access the dashboard: connect a device to the **"PPE-Detection"** Wi-Fi network created by the XIAO, then open `http://192.168.4.1/` in a browser.