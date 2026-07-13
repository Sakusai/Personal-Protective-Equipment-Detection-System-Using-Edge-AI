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

### Step 2 — ESP32 software setup (in progress)

Stack: **C/C++ with PlatformIO (VSCode)**, Arduino framework — required because the `Seeed_Arduino_SSCMA` library (handles I2C communication with the module) only supports Arduino, not MicroPython.

- [x] PlatformIO project set up (`seeed_xiao_esp32s3`, framework `arduino`)
- [x] Dependencies added: `Seeed_Arduino_SSCMA` + `ArduinoJson`
- [x] First test sketch written: reads detections (boxes, scores, coordinates) and prints them to serial
- [x] Build successful
- [ ] Upload to the XIAO — currently debugging (bootloader mode issue)
- [ ] Confirm live detections (face/hand) over serial

### Next steps

- [ ] Get upload working, validate real-time detection
- [ ] Train a custom model (Edge Impulse) for actual PPE: helmet, vest, glasses
- [ ] Logic to combine "person" + "PPE" detections into a compliant/non-compliant status
- [ ] Add Wi-Fi + web server on the XIAO for live status, event history, and configuration
