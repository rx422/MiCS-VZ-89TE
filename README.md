# 🌫️ MiCS-VZ-89TE – Indoor Air Quality Sensor Driver

[![License: GPL v3](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![PlatformIO Library](https://img.shields.io/badge/PlatformIO-Ready-orange?logo=platformio)](https://platformio.org/lib/show)

Cross-platform C driver for the **MiCS-VZ-89TE** indoor air quality sensor. Supports VOC (ppb), CO₂ (ppm), raw resistance measurements (Ω), R₀ calibration, and revision information via I²C.

---

## 🚀 Features

- ✅ Fully cross-platform C implementation
- 🔄 Communicates via I²C with CRC integrity checking
- 🔬 Access to:
  - VOC and CO₂ equivalent readings
  - Raw sensor resistance
  - Factory R₀ calibration value
  - Sensor date/revision metadata
- 🧩 Optional Arduino-compatible wrapper using `Wire.h`
- 🧪 Ready-to-run PlatformIO demo with `main.cpp`

---

## 📦 Installation

#### PlatformIO Registry (recommended)

Add to `platformio.ini`:

```ini
lib_deps = https://github.com/rx422/MiCS-VZ-89TE.git#v0.1.0
```

## 🛠️ Manual Install

Clone or download the repo and place it in your project's `lib/` directory:

```ini
git clone https://github.com/rx422/MiCS-VZ-89TE.git
```

---

## 🧪 Example: Arduino UNO

Available in [`examples/app/src/main.cpp`](./examples/app/src/main.cpp)

---

## 📁 Project Structure

```ini
MiCS-VZ-89TE/
├── include/               ← Public driver header
├── src/                   ← C driver implementation
├── examples/
│   └── app/
│       ├── platformio.ini
│       └── src/
│           ├── main.cpp
│           ├── VZ89TE_wrapper.h
│           └── VZ89TE_wrapper.cpp
├── library.json           ← PlatformIO manifest
├── LICENSE                ← GNU GPL v3 license
└── README.md              ← Project overview and usage guide
```

---

## 📜 License

This project is licensed under the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.html). 
© 2025 **rx422** · [ad.birnaz@gmail.com](mailto:ad.birnaz@gmail.com)

---

## 🙌 Contributions

Found a bug? Have an idea?  
📬 [Open an issue](../../issues) or submit a pull request — contributions are welcome!

---

## 🛠️ Future Plans

- ✅ Initial release (v0.1.0)
- 🔧 Add support for other output types (e.g., PWM) 
  - [ ] Define and implement a PWM API
- 📘 Doxygen‐style documentation

---
