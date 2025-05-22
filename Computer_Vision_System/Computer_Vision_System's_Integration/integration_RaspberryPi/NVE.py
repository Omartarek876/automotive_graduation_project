import cv2
import numpy as np
import torch
from ultralytics import YOLO
from i2c_utils import send_i2c_flag
import pygame
import threading
import os

# Initialize Pygame mixer
pygame.mixer.init()

# Load YOLO models
model1 = YOLO("/home/mahmoud/integration_final/yolov8n_ncnn_model")
model2 = YOLO("/home/mahmoud/integration_final/TSRTL_ncnn_model")

# Speakable labels
speakable_labels = {
    "No Entry", "No Overtaking", "No Stopping",
    "Pedestrian Crossing", "Road Works",
    "Speed Limit 100", "Speed Limit 120", "Speed Limit 60",
    "Speed Limit 80", "Speed Limit 90", "s-Bump", "Curve Ahead"
}


# ── Rate‑limiter state ─────────────────────────────────────────────
_last_sent = {}
COOLDOWN = 5.0

def _should_send(label: str) -> bool:
    now = time.time()
    last = _last_sent.get(label, 0.0)
    if now - last >= COOLDOWN:
        _last_sent[label] = now
        return True
    return False

def check_traffic_signal(label: str):
    # define the mapping here, so it's only in this scope
    signal_codes = {
        "red": 8,
        "green": 9,
        "stop sign": 10
    }
    code = signal_codes.get(label.lower())
    if code is not None and _should_send(label.lower()):
        print(f"Detected traffic signal: {label} (code: {code}), sending I²C flag")
        try:
            send_i2c_flag(code)
        except Exception as e:
            print(f"[i2c ERROR] {e}")


# Sound playback
def speak_label_once(label):
    threading.Thread(target=play_sound_once, args=(label,), daemon=True).start()

def play_sound_once(label):
    sound_path = os.path.join("sounds", f"{label}.mp3")
    if os.path.exists(sound_path):
        try:
            pygame.mixer.music.load(sound_path)
            pygame.mixer.music.play()
            while pygame.mixer.music.get_busy():
                pygame.time.Clock().tick(10)
        except Exception as e:
            print(f"[SOUND ERROR] {e}")
    else:
        print(f"[MISSING SOUND] No audio file for: {label}")

# Image enhancement
def adjust_gamma(image, gamma=1.0):
    invGamma = 1.0 / gamma
    table = np.array([((i / 255.0) ** invGamma) * 255 for i in np.arange(256)]).astype("uint8")
    return cv2.LUT(image, table)


def unsharp_mask(image, kernel_size=(5, 5), sigma=1.0, amount=2.0, threshold=0):
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
    if brightness < 100:
        blurred_img = cv2.GaussianBlur(frame, (3, 3), 0)
        img2hsv = cv2.cvtColor(blurred_img, cv2.COLOR_BGR2HSV)
        hsv_v = img2hsv[:, :, 2]

        clahe = cv2.createCLAHE(clipLimit=3.0, tileGridSize=(8, 8))
        cl1 = clahe.apply(hsv_v)
        cl1 = adjust_gamma(cl1, gamma=1.1)
        enhanced_v = np.clip(cl1 * 1.2, 0, 255)
        img2hsv[:, :, 2] = enhanced_v

        improved_image = cv2.cvtColor(img2hsv, cv2.COLOR_HSV2BGR)
        return unsharp_mask(improved_image)
    else:
        return frame.copy()

# Determine traffic light class id for model1
model1_class_names = model1.names
traffic_light_class_id = [k for k, v in model1_class_names.items() if v == "traffic light"][0]

# Main function
def main():

    cap = cv2.VideoCapture(1)
    if not cap.isOpened():
        print("Failed to open the camera")
        return

    previous_labels = set()
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Failed to read the frames from the camera!")
            break

        enhanced_frame = enhance_image(frame)
        results1 = model1(enhanced_frame, conf=0.3)
        results2 = model2(enhanced_frame, conf=0.3)

        current_labels = set()

        for r1 in results1:
            for box in r1.boxes:
                cls_id = int(box.cls[0])
                if cls_id == traffic_light_class_id:
                    continue
                label = model1_class_names[cls_id]
                current_labels.add(label)
                check_traffic_signal(label)

                x1, y1, x2, y2 = map(int, box.xyxy[0])
                cv2.rectangle(enhanced_frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(enhanced_frame, label, (x1, y1 - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

        for r2 in results2:
            for box in r2.boxes:
                label = r2.names[int(box.cls[0])]
                current_labels.add(label)
                check_traffic_signal(label)

                x1, y1, x2, y2 = map(int, box.xyxy[0])
                color = (0, 0, 255) if label == "red" else (0, 255, 0) if label == "green" else (0, 255, 255)
                cv2.rectangle(enhanced_frame, (x1, y1), (x2, y2), color, 2)
                cv2.putText(enhanced_frame, label, (x1, y1 - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

        new_labels = current_labels - previous_labels
        for label in new_labels:
            if label in speakable_labels:
                speak_label_once(label)

        previous_labels = current_labels.copy()
        cv2.imshow("Night Vision System", enhanced_frame)
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

    cap.release()
    cv2.destroyAllWindows()


# end of module


# main.py
if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Critical error: {e}")
