# 🚗 Driver Monitoring System (DMS) with YOLOv8 and MediaPipe

This project is an **Driver Monitoring System** that uses real-time face detection, eye tracking, head pose estimation, and facial classification to ensure the driver's alertness and attention. It detects risky situations such as drowsiness, distractions, or absence and generates appropriate audio alerts.

---

## 📌 Features

- ✅ **YOLOv8 Face Detection**
- ✅ **Glasses Detection**
- ✅ **Mask Detection**
- ✅ **Eye Closure Detection (Blinking & Drowsiness)**
- ✅ **Head Pose Estimation (left, right, up, down)**
- ✅ **Audio Alerts** for:
  - Looking away too long (left/right/up/down)
  - Eyes closed more than 1.5 seconds
  - Face not detected

---

## 🧠 Models Used

This system uses **YOLOv8** with custom-trained models:

| Model       | Purpose                 |
|-------------|--------------------------|
| `best.pt`   | Glasses Detection         |
| `Mask.pt`   | Mask Detection            |
| `closed.pt` | Eye Status (Open/Closed)  |

---

## 🛠️ Requirements

Make sure you have Python 3.8+ installed. Install required libraries using:

```bash
pip install opencv-python ultralytics mediapipe pygame numpy
