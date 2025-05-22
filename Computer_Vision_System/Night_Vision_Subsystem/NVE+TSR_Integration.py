import cv2
import numpy as np
import torch
from ultralytics import YOLO
import pyttsx3
import threading

# YOLO models
model1 = YOLO("yolov8n.pt")
model2 = YOLO("TSRTL.pt")

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("failed to open the camera")
    exit()

# Sound
speakable_labels = {
    "No Entry", "No Overtaking", "No Stopping",
    "Pedestrian Crossing", "Road Works",
    "Speed Limit 100", "Speed Limit 120", "Speed Limit 60",
    "Speed Limit 80", "Speed Limit 90", "s-Bump", "Curve Ahead"
}

# Flags and voice functions
def check_traffic_signal(label):
    if label.lower() in ["red", "green", "stop"]:
        print(f"🚦 Current traffic light: {label}")

def speak_label(text):
    threading.Thread(target=run_speech, args=(text,), daemon=True).start()

def run_speech(text):
    engine = pyttsx3.init()
    engine.say(text)
    engine.runAndWait()

# Helper functions for image enhancement
def adjust_gamma(image, gamma=1.0):
    invGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** invGamma) * 255 for i in np.arange(256)]).astype("uint8")
    return cv2.LUT(image, table)

def unsharp_mask(image, kernel_size=(5, 5), sigma=1.0, amount=1.5, threshold=0):
    blurred = cv2.GaussianBlur(image, kernel_size, sigma)
    sharpened = float(amount + 1) * image - float(amount) * blurred
    sharpened = np.clip(sharpened, 0, 255).astype(np.uint8)
    if threshold > 0:
        low_contrast_mask = np.abs(image - blurred) < threshold
        np.copyto(sharpened, image, where=low_contrast_mask)
    return sharpened

def enhance_image(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    brightness = np.mean(hsv[:, :, 2])

    if brightness < 80:  # Adjust threshold as needed
        blurred_img = cv2.GaussianBlur(frame, (3, 3), 0)
        img2hsv = cv2.cvtColor(blurred_img, cv2.COLOR_BGR2HSV)
        hsv_v = img2hsv[:, :, 2]
        
        # Apply CLAHE
        clahe = cv2.createCLAHE(clipLimit=3.0, tileGridSize=(8, 8))
        cl1 = clahe.apply(hsv_v)
        
        # Apply Gamma correction
        cl1 = adjust_gamma(cl1, gamma=1.1)
        
        # Enhance the brightness by increasing the V channel value
        enhanced_v = np.clip(cl1 * 1.2, 0, 255)  # Increase V by 20%
        img2hsv[:, :, 2] = enhanced_v
        
        improved_image = cv2.cvtColor(img2hsv, cv2.COLOR_HSV2BGR)
        
        # Apply Unsharp Masking
        return unsharp_mask(improved_image)
    else:
        return frame.copy()

# Load YOLO models for detection
model1_class_names = model1.names
traffic_light_class_id = [k for k, v in model1_class_names.items() if v == "traffic light"][0]

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        print("Failed to read the frames from the camera!")
        break

    # Enhance image if necessary
    enhanced_frame = enhance_image(frame)

    # Detect objects using YOLO models
    results1 = model1(enhanced_frame, conf=0.3)
    results2 = model2(enhanced_frame, conf=0.3)

    # Process results from first model (traffic light detection)
    for r1 in results1:
        for box in r1.boxes:
            cls_id = int(box.cls[0])
            if cls_id == traffic_light_class_id:
                continue

            x1, y1, x2, y2 = map(int, box.xyxy[0])
            label = model1_class_names[cls_id]
            cv2.rectangle(enhanced_frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(enhanced_frame, label, (x1, y1 - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    # Process results from second model (traffic signal detection)
    for r2 in results2:
        for box in r2.boxes:
            x1, y1, x2, y2 = map(int, box.xyxy[0])
            label = r2.names[int(box.cls[0])]
            color = (0, 0, 255) if label == "red" else (0, 255, 0) if label == "green" else (0, 255, 255)

            cv2.rectangle(enhanced_frame, (x1, y1), (x2, y2), color, 2)
            cv2.putText(enhanced_frame, label, (x1, y1 - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

            check_traffic_signal(label)

            if label in speakable_labels:
                speak_label(label)

    # Show the enhanced frame with detections
    cv2.imshow("Enhanced Output", enhanced_frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
