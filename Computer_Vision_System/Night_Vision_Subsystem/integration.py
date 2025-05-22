import cv2
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

#Sound
speakable_labels = {
    "Left Turn", "No Entry", "No Overtaking", "No Stopping",
    "Pedestrian Crossing", "Right Turn", "Road Works",
    "Speed Limit 100", "Speed Limit 120", "Speed Limit 60",
    "Speed Limit 80", "Speed Limit 90", "s-Bump" , "Curve Ahead"
}

#Flags
def check_traffic_signal(label):
    if label.lower() in ["red", "green", "stop"]:
        print(f"🚦 Current traffic light: {label}")

def speak_label(text):
    threading.Thread(target=run_speech, args=(text,), daemon=True).start()

def run_speech(text):
    engine = pyttsx3.init()
    engine.say(text)
    engine.runAndWait()

class_names1 = model1.names
traffic_light_class_id = [k for k, v in class_names1.items() if v == "traffic light"][0]

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        print("Failed to read the frames from the camera!")
        break

    results1 = model1(frame, conf=0.3)
    results2 = model2(frame, conf=0.3)

    for r1 in results1:
        for box in r1.boxes:
            cls_id = int(box.cls[0])
            if cls_id == traffic_light_class_id:
                continue

            x1, y1, x2, y2 = map(int, box.xyxy[0])
            label = class_names1[cls_id]
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, label, (x1, y1 - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    for r2 in results2:
        for box in r2.boxes:
            x1, y1, x2, y2 = map(int, box.xyxy[0])
            label = r2.names[int(box.cls[0])]
            color = (0, 0, 255) if label == "red" else (0, 255, 0) if label == "green" else (0, 255, 255)

            cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)
            cv2.putText(frame, label, (x1, y1 - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

            check_traffic_signal(label)

            if label in speakable_labels:
                speak_label(label)

    cv2.imshow("Output", frame)
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
