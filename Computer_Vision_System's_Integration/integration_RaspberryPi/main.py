import i2c_utils
import cv2
import time
from picamera2 import Picamera2

# DMS imports
from dms_f import (
    DMS_model1,DMS_model2,
    check_conditions,Head_Eye_detect,
    detect_head_pose,No_Face
)

# NVS imports
from NVE import (
    enhance_image,
    model1 as nvs_model1,
    model2 as nvs_model2,traffic_light_class_id,
    check_traffic_signal,speakable_labels,
    _should_send,speak_label_once
)



def main():
   
    # Initialize PiCamera2 for DMS
    picam2 = Picamera2()
    picam2.configure(picam2.create_preview_configuration(main={"format": "RGB888", "size": (640, 480)}))
    picam2.start()

    # Open NVS camera 
    nvs_cap = cv2.VideoCapture(1)
    
    prev_time = time.time()
    frame_count = 0
    fps = 0
    prev_nvs_labels = set()

    while True:
        # Capture DMS frame via PiCamera2
        frame = picam2.capture_array()

        # Capture NVS frame via OpenCV
        ret, nvs_frame = nvs_cap.read()
        if not ret:
            print("Error: couldn't read frame from NVS camera.")
            break

        # Calculate FPS
        current_time = time.time()
        frame_count += 1
        if current_time - prev_time >= 1.0:
            fps = frame_count
            frame_count = 0
            prev_time = current_time

        try:
            # === DMS Processing ===
            results1 = DMS_model1(frame,imgsz=320)
            results2 = DMS_model2(frame,imgsz=320)
            case = str(check_conditions(results1, results2))

            f# Draw YOLO detections manually to avoid errors
            for result in [results1, results2]:
                for box in result[0].boxes:
                    x1, y1, x2, y2 = map(int, box.xyxy[0])  # Convert bounding box to int
                    cls = int(box.cls)
                    conf = float(box.conf)
                    label = f"{cls}: {conf:.2f}"

                    cv2.rectangle(frame, (x1, y1), (x2, y2), RED, 2)

            # Ensure frame is in the correct format
            if frame is None:
                print("Error: Frame is None")
                continue

            if len(frame.shape) == 2:  # If grayscale, convert to BGR
                frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2BGR)

            # Process frame based on detection case
            if case == "Face detected":
                frame = Head_Eye_detect(frame)
            elif case == "Face with glasses detected":
                frame = detect_head_pose(frame)
            elif case == "Face with mask detected":
                frame = detect_head_pose(frame)
            elif case == "Face with mask and glasses detected":
                frame = detect_head_pose(frame)
            elif case == "No face detected":
                frame = No_Face(frame)

            cv2.putText(frame, case, (20, 300), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

            # === Night Vision Processing ===
            enhanced_nvs = enhance_image(nvs_frame)
            nvs_results1 = nvs_model1(enhanced_nvs, conf=0.3)
            nvs_results2 = nvs_model2(enhanced_nvs, conf=0.3)

            current_nvs_labels = set()
            for r1 in nvs_results1:
                for box in r1.boxes:
                    cls_id = int(box.cls[0])
                    if cls_id == traffic_light_class_id:
                        continue
                    label = nvs_model1.names[cls_id]
                    current_nvs_labels.add(label)
                    x1, y1, x2, y2 = map(int, box.xyxy[0])
                    cv2.rectangle(enhanced_nvs, (x1, y1), (x2, y2), (0, 255, 0), 2)
                    cv2.putText(enhanced_nvs, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                    check_traffic_signal(label)

            for r2 in nvs_results2:
                for box in r2.boxes:
                    label = nvs_model2.names[int(box.cls[0])]
                    current_nvs_labels.add(label)
                    x1, y1, x2, y2 = map(int, box.xyxy[0])
                    color = (0, 0, 255) if label == "red" else (0, 255, 0) if label == "green" else (0, 255, 255)
                    cv2.rectangle(enhanced_nvs, (x1, y1), (x2, y2), color, 2)
                    cv2.putText(enhanced_nvs, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)
                    check_traffic_signal(label)

            new_labels = current_nvs_labels - prev_nvs_labels
            for label in new_labels:
                if label in speakable_labels:
                    speak_label_once(label)
            prev_nvs_labels = current_nvs_labels.copy()

            # Display outputs
            cv2.imshow("Driver Monitoring System", frame)
            cv2.imshow("Night Vision System", enhanced_nvs)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        except Exception as e:
            print(f"Error processing frame: {e}")
            continue

    # Cleanup
    picam2.stop()
    nvs_cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()