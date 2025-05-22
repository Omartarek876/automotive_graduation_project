import cv2
from ultralytics import YOLO
import serial
import numpy as np
import mediapipe as mp
import time
import math
import pyttsx3
import threading
import pygame 
from pygame import mixer

# Load models
DMS_model1 = YOLO("Mask.pt")  
DMS_model2 = YOLO("best.pt") 
nvs_model1 = YOLO("yolov8n.pt")
nvs_model2 = YOLO("TSRTL.pt")
eye_model = YOLO("closed.pt")  # Eye status detection model for masked faces


#Sound
speakable_labels = {
    "Left Turn", "No Entry", "No Overtaking", "No Stopping",
    "Pedestrian Crossing", "Right Turn", "Road Works",
    "Speed Limit 100", "Speed Limit 120", "Speed Limit 60",
    "Speed Limit 80", "Speed Limit 90", "s-Bump" , "Curve Ahead"
}

FLAG_DELAY = 10  
COMMON_ALERT = 7 
uart_start_time = {"Looking Left": 0, "Looking Right": 0, "Looking Down": 0, "Looking Up": 0, "Closed Eyes": 0, "no face": 0}
flag_sent = {"Looking Left": False, "Looking Right": False, "Looking Down": False, "Looking Up": False, "Closed Eyes": False, "no face": False}
ser = serial.Serial('COM3', 9600)

# Global counters for tracking states
looking_left_frames = 0
looking_right_frames = 0
looking_down_frames = 0
looking_up_frames = 0
closed_eyes_frames = 0

# Time-based tracking variables
start_time_left = 0
start_time_right = 0
start_time_down = 0
start_time_up = 0
start_time_closed_eyes = 0
start_time_no = 0


# Time thresholds (in seconds)
TIME_THRESHOLD_LEFT = 3.0  # 3 seconds looking left
TIME_THRESHOLD_RIGHT = 3.0  # 3 seconds looking right
TIME_THRESHOLD_DOWN = 3.0  # 3 seconds looking down
TIME_THRESHOLD_UP = 3.0  # 3 seconds looking up
TIME_THRESHOLD_CLOSED_EYES = 3.0  # 3 seconds with eyes closed
TIME_THRESHOLD_No_Face = 3.0 

# State tracking variables
current_state = "Forward"
previous_state = "Forward"

# Define thresholds
AUDIO_THRESHOLD = 30
ESCALATION_THRESHOLD = 90

# Audio Initialization
mixer.init()
voice_left = mixer.Sound('left.wav')
voice_right = mixer.Sound('Right.wav')
voice_down = mixer.Sound('down.wav')
eyes_blink= mixer.Sound('eyes_blink.wav')
warning_sound = mixer.Sound('Warningg.mp3')

# Color Definitions
BLUE = (255,0,0)
RED = (0,0,255)
GREEN = (0,255,0)

# Landmark Indices
RIGHT_EYE = [ 33, 7, 163, 144, 145, 153, 154, 155, 133, 173, 157, 158, 159, 160, 161 , 246 ] 
LEFT_EYE = [ 362, 382, 381, 380, 374, 373, 390, 249, 263, 466, 388, 387, 386, 385,384, 398 ]
LOWER_LIPS = [61, 146, 91, 181, 84, 17, 314, 405, 321, 375, 291, 308, 324, 318, 402, 317, 14, 87, 178, 88, 95]
UPPER_LIPS = [ 185, 40, 39, 37,0 ,267 ,269 ,270 ,409, 415, 310, 311, 312, 13, 82, 81, 42, 183, 78] 

# Initialize MediaPipe Face Mesh
mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(min_detection_confidence=0.2, min_tracking_confidence=0.2)


def send_uart_flag(flag):
    try:
        ser.write(f"{flag}".encode())  
        print(f"UART SENT -> {flag}")  
    except Exception as e:
        print(f"UART ERROR: {e}")

# Time-based action tracking
def track_time_based_actions(current_pose):
    global start_time_left, start_time_right, start_time_down, start_time_up, start_time_closed_eyes,start_time_no
    global uart_start_time, flag_sent, sound_played, current_state, previous_state

    current_time = time.time()

    # Reset timers if state changes  
    if current_state != previous_state:
        start_time_left = start_time_right = start_time_down = start_time_up = start_time_closed_eyes = start_time_no = current_time
        uart_start_time = {key: 0 for key in uart_start_time}
        flag_sent = {key: False for key in flag_sent}
        previous_state = current_state

    if current_pose == "Looking Left":
        if uart_start_time["Looking Left"] == 0:  
            uart_start_time["Looking Left"] = current_time  

        if current_time - start_time_left >= TIME_THRESHOLD_LEFT:
            print("WARNING: Looking Left for too long!")
            if pygame.mixer.get_busy()==0:
             voice_left.play()
            start_time_left = current_time

        if current_time - uart_start_time["Looking Left"] >= FLAG_DELAY and not flag_sent["Looking Left"]:
            send_uart_flag(COMMON_ALERT)  # Edited: Using I2C to send the common flag
            flag_sent["Looking Left"] = True  

    elif current_pose == "Looking Right":
        if uart_start_time["Looking Right"] == 0:
            uart_start_time["Looking Right"] = current_time  

        if current_time - start_time_right >= TIME_THRESHOLD_LEFT:
            print("WARNING: Looking Right for too long!")
            if pygame.mixer.get_busy()==0:
             voice_right.play()
            start_time_right = current_time

        if current_time - uart_start_time["Looking Right"] >= FLAG_DELAY and not flag_sent["Looking Right"]:
            send_uart_flag(COMMON_ALERT)  # Edited: Using I2C to send the common flag
            flag_sent["Looking Right"] = True  

    elif current_pose == "Looking Down":
        if uart_start_time["Looking Down"] == 0:
            uart_start_time["Looking Down"] = current_time  

        if current_time - start_time_down >= TIME_THRESHOLD_LEFT:
            print("WARNING: Looking Down for too long!")
            if pygame.mixer.get_busy()==0:
             voice_down.play()
            start_time_down = current_time

        if current_time - uart_start_time["Looking Down"] >= FLAG_DELAY and not flag_sent["Looking Down"]:
            send_uart_flag(COMMON_ALERT)  # Edited: Using I2C to send the common flag
            flag_sent["Looking Down"] = True  

    elif current_pose == "Looking Up":
        if uart_start_time["Looking Up"] == 0:
            uart_start_time["Looking Up"] = current_time  

        if current_time - start_time_up >= TIME_THRESHOLD_LEFT:
            print("WARNING: Looking Up for too long!")
            if pygame.mixer.get_busy()==0:
             warning_sound.play()
            start_time_up = current_time

        if current_time - uart_start_time["Looking Up"] >= FLAG_DELAY and not flag_sent["Looking Up"]:
            send_uart_flag(COMMON_ALERT)  # Edited: Using I2C to send the common flag
            flag_sent["Looking Up"] = True  

    elif current_pose == "Closed Eyes":
        if uart_start_time["Closed Eyes"] == 0:
            uart_start_time["Closed Eyes"] = current_time  

        if current_time - start_time_closed_eyes >= TIME_THRESHOLD_LEFT:
            print("WARNING: Eyes Closed for too long!")
            if pygame.mixer.get_busy()==0:
             eyes_blink.play()
            start_time_closed_eyes = current_time

        if current_time - uart_start_time["Closed Eyes"] >= FLAG_DELAY and not flag_sent["Closed Eyes"]:
            send_uart_flag(COMMON_ALERT)  # Edited: Using I2C to send the common flag
            flag_sent["Closed Eyes"] = True 

    elif current_pose == "No Face Detected":
        if uart_start_time["no face"] == 0:
            uart_start_time["no face"] = current_time  

        if current_time - start_time_no >= TIME_THRESHOLD_No_Face:
            print("WARNING!!!")
            if pygame.mixer.get_busy()==0:
             warning_sound.play()
            start_time_no = current_time

        if current_time - uart_start_time["no face"] >= FLAG_DELAY and not flag_sent["no face"]:
            send_uart_flag(COMMON_ALERT)  # Edited: Using I2C to send the common flag
            flag_sent["no face"] = True       


def check_conditions(results1, results2):
    classes1 = [int(cls) for cls in results1[0].boxes.cls] if results1[0].boxes else []
    classes2 = [int(cls) for cls in results2[0].boxes.cls] if results2[0].boxes else []

    face_detected = 0 in classes1
    mask_detected = 1 in classes1
    glass_detected = 0 in classes2

    if face_detected and not mask_detected and not glass_detected:
        case = "Face detected"
    elif face_detected and not mask_detected and glass_detected:
        case = "Face with glasses detected"
    elif not face_detected and mask_detected and not glass_detected:
        case = "Face with mask detected"
    elif not face_detected and mask_detected and glass_detected:
        case = "Face with mask and glasses detected"
    else:
        case = "No face detected"
    
    print(case)
    return case

def landmarksDetection(img, results, draw=False):
    img_height, img_width = img.shape[:2]
    mesh_coord = [(int(point.x * img_width), int(point.y * img_height)) for point in results.multi_face_landmarks[0].landmark]
    if draw:
        [cv2.circle(img, p, 2, (0,255,0), -1) for p in mesh_coord]
    return mesh_coord

def euclaideanDistance(point, point1):
    x, y = point
    x1, y1 = point1
    distance = math.sqrt((x1 - x)**2 + (y1 - y)**2)
    return distance

def blinkRatio(img, landmarks, right_indices, left_indices):
    # Right eyes 
    rh_right = landmarks[right_indices[0]]
    rh_left = landmarks[right_indices[8]]
    rv_top = landmarks[right_indices[12]]
    rv_bottom = landmarks[right_indices[4]]

    # LEFT_EYE 
    lh_right = landmarks[left_indices[0]]
    lh_left = landmarks[left_indices[8]]
    lv_top = landmarks[left_indices[12]]
    lv_bottom = landmarks[left_indices[4]]
    rhDistance = euclaideanDistance(rh_right, rh_left)
    rvDistance = euclaideanDistance(rv_top, rv_bottom)

    lvDistance = euclaideanDistance(lv_top, lv_bottom)
    lhDistance = euclaideanDistance(lh_right, lh_left)
    
    if lvDistance != 0 and lhDistance !=0:
        reRatio = rhDistance/rvDistance
        leRatio = lhDistance/lvDistance
    
    ratio = (reRatio+leRatio)/2
    return ratio   

def detect_head_pose(image):
    global current_state, previous_state
    
    start = time.time()

    image = cv2.cvtColor(cv2.flip(image,1),cv2.COLOR_BGR2RGB)
    image.flags.writeable = False
    results = face_mesh.process(image)
    image.flags.writeable = True
    image = cv2.cvtColor(image,cv2.COLOR_RGB2BGR)

    img_h, img_w, img_c = image.shape
    face_2d = []
    face_3d = []

    if results.multi_face_landmarks:
        for face_landmarks in results.multi_face_landmarks:
            for idx, lm in enumerate(face_landmarks.landmark):
                if idx == 33 or idx == 263 or idx ==1 or idx == 61 or idx == 291 or idx==199 or idx==10:
                    if idx ==10:
                        nose_2d = (lm.x * img_w,lm.y * img_h)
                        nose_3d = (lm.x * img_w,lm.y * img_h,lm.z * 3000)
                    x,y = int(lm.x * img_w),int(lm.y * img_h)

                    face_2d.append([x,y])
                    face_3d.append(([x,y,lm.z]))

            face_2d = np.array(face_2d,dtype=np.float64)
            face_3d = np.array(face_3d,dtype=np.float64)

            focal_length = 1 * img_w
            cam_matrix = np.array([[focal_length,0,img_h/2],
                                  [0,focal_length,img_w/2],
                                  [0,0,1]])
            distortion_matrix = np.zeros((4,1),dtype=np.float64)

            success,rotation_vec,translation_vec = cv2.solvePnP(face_3d,face_2d,cam_matrix,distortion_matrix)

            rmat,jac = cv2.Rodrigues(rotation_vec)
            angles,mtxR,mtxQ,Qx,Qy,Qz = cv2.RQDecomp3x3(rmat)

            x = angles[0] * 360
            y = angles[1] * 360
            z = angles[2] * 360

            if y < -10:
                text = "Looking Left"
                current_state = "Looking Left"
            elif y > 10:
                text = "Looking Right"
                current_state = "Looking Right"
            elif x < -4:
                text = "Looking Down"
                current_state = "Looking Down"
            elif x > 10:
                text = "Looking Up"
                current_state = "Looking Up"
            else:
                text = "Forward"
                current_state = "Forward"

            # Track time-based actions
            track_time_based_actions(current_state)

            nose_3d_projection,jacobian = cv2.projectPoints(nose_3d,rotation_vec,translation_vec,cam_matrix,distortion_matrix)

            p1 = (int(nose_2d[0]),int(nose_2d[1]))
            p2 = (int(nose_2d[0] + y*10), int(nose_2d[1] -x *10))

            cv2.line(image,p1,p2,(255,0,0),3)

            cv2.putText(image,text,(20,50),cv2.FONT_HERSHEY_SIMPLEX,2,(0,255,0),2)
            cv2.putText(image,"x: " + str(np.round(x,2)),(500,50),cv2.FONT_HERSHEY_SIMPLEX,1,(0,0,255),2)
            cv2.putText(image,"y: "+ str(np.round(y,2)),(500,100),cv2.FONT_HERSHEY_SIMPLEX,1,(0,0,255),2)
            cv2.putText(image,"z: "+ str(np.round(z, 2)), (500, 150), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

        end = time.time()
        totalTime = end-start
        fps = 1/totalTime
        print("FPS: ",fps)

        #cv2.putText(image,f'FPS: {int(fps)}',(20,450),cv2.FONT_HERSHEY_SIMPLEX,1.5,(0,255,0),2)

        return image

    return image

def Head_Eye_detect(image):
    global current_state, previous_state
    
    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
    image.flags.writeable = False
    results = face_mesh.process(image)
    image.flags.writeable = True
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    img_h, img_w, img_c = image.shape
    face_3d = []
    face_2d = []
   
    if results.multi_face_landmarks:
        for face_landmarks in results.multi_face_landmarks:
            for idx, lm in enumerate(face_landmarks.landmark):
                if idx == 33 or idx == 263 or idx == 1 or idx == 61 or idx == 291 or idx == 199 or idx==10:
                    if idx == 10:
                        nose_2d = (lm.x * img_w, lm.y * img_h)
                        nose_3d = (lm.x * img_w, lm.y * img_h, lm.z * 8000)

                    x, y = int(lm.x * img_w), int(lm.y * img_h)

                    face_2d.append([x, y])
                    face_3d.append([x, y, lm.z])       
            
            face_2d = np.array(face_2d, dtype=np.float64)
            face_3d = np.array(face_3d, dtype=np.float64)

            focal_length = 1 * img_w

            cam_matrix = np.array([ [focal_length, 0, img_h / 2],
                                    [0, focal_length, img_w / 2],
                                    [0, 0, 1]])

            dist_matrix = np.zeros((4, 1), dtype=np.float64)

            success, rot_vec, trans_vec = cv2.solvePnP(face_3d, face_2d, cam_matrix, dist_matrix)

            rmat, jac = cv2.Rodrigues(rot_vec)

            angles, mtxR, mtxQ, Qx, Qy, Qz = cv2.RQDecomp3x3(rmat)
            x = angles[0] * 360
            y = angles[1] * 360

            # Detect eye blink
            mesh_coords = landmarksDetection(image, results, False)
            ratio = blinkRatio(image, mesh_coords, RIGHT_EYE, LEFT_EYE)

            if y < -10:
                text = "Looking Left"
                current_state = "Looking Left"
            elif y > 10:
                text = "Looking Right"
                current_state = "Looking Right"
            elif x < -4:
                text = "Looking Down"
                current_state = "Looking Down"
            elif x > 10:
                text = "Looking Up"
                current_state = "Looking Up"
            elif ratio > 4.3:
                text = "Closed Eyes"
                current_state = "Closed Eyes"
            else:
                text = "Forward"
                current_state = "Forward"

            # Track time-based actions
            track_time_based_actions(current_state)

            nose_3d_projection, jacobian = cv2.projectPoints(
                nose_3d, rot_vec, trans_vec, cam_matrix, dist_matrix
            )

            p1 = (int(nose_2d[0]), int(nose_2d[1]))
            p2 = (int(nose_2d[0] + y*10), int(nose_2d[1] - x*10))

            cv2.line(image, p1, p2, (255, 0, 0), 3)
            cv2.putText(image, text, (20, 50), 
                        cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 255, 0), 2)
            
            # Display additional tracking information
            print(ratio)

        return image

    return image

def Eye_Status_detection(image):
    
    global eye_closed_start_time, eye_closed_duration
    
    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
    image.flags.writeable = False
    
    # Get face regions from mask detection model
    results = eye_model(image)
    
    image.flags.writeable = True
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
    
    # Extract classes and confidence scores
    if results[0].boxes:
        classes = [int(cls) for cls in results[0].boxes.cls]
        confs = [float(conf) for conf in results[0].boxes.conf]
        boxes = results[0].boxes.xyxy.cpu().numpy()
        
        # Draw each box with its confidence
        for i, (box, cls, conf) in enumerate(zip(boxes, classes, confs)):
            x1, y1, x2, y2 = map(int, box)
            
            # Set color based on class (0=closed, 1=open)
            color = (0, 0, 255) if cls == 0 else (0, 255, 0)
            
            # Draw bounding box
            cv2.rectangle(image, (x1, y1), (x2, y2), color, 2)
            
            # Display class name and confidence
            label = f"{'Closed' if cls == 0 else 'Open'}: {conf:.2f}"
            cv2.putText(image, label, (x1, y1-10), 
                       cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)
    
    # Determine overall eye status from all detections
    eyes_opened = 1 in classes if results[0].boxes else False
    eyes_closed = 0 in classes if results[0].boxes else False
    
    if eyes_closed:
        text = "Closed Eyes"
        current_state = "Closed Eyes"
    else:
        text = "Forward"
        current_state = "Forward"
    
    # Track time-based actions
    track_time_based_actions(current_state)
    cv2.putText(image, text, (20, 50), 
                cv2.FONT_HERSHEY_SIMPLEX, 2, GREEN, 2)
    
    return image


def No_Face(image):
    global current_state, previous_state
    
    
    text = "Warning !!!"
    current_state = "No Face Detected"
           

    # Track time-based actions
    track_time_based_actions(current_state)

            

            
    cv2.putText(image, text, (20, 50), 
                cv2.FONT_HERSHEY_SIMPLEX, 2, RED, 2)
            
           

    return image

def check_traffic_signal(label):
    # Map each traffic signal label to a numeric flag
    signal_codes = {
        "red": 8,
        "green": 9,
        "stop": 10
    }
    
    if label.lower() in signal_codes:
        flag = signal_codes[label]
        print(f"Detected traffic signal: {label} (code: {flag})")
        try:
            # Use the send_uart_flag function to send the numeric flag over UART
            send_uart_flag(flag)
        except Exception as e:
            print(f"Error sending data: {e}")


def speak_label(text):
    threading.Thread(target=run_speech, args=(text,), daemon=True).start()

def run_speech(text):
    engine = pyttsx3.init()
    engine.say(text)
    engine.runAndWait()


def main():
    dms_cap = cv2.VideoCapture(0)  # DMS stream
    nvs_cap = cv2.VideoCapture(1)  # NVS stream

    # Check if the cameras opened properly
    if not dms_cap.isOpened():
        print("Error: Couldn't open DMS camera.")
        return
    if not nvs_cap.isOpened():
        print("Error: Couldn't open NVS camera.")
        return

    prev_time = time.time()
    fps = 0
    frame_count = 0

    prev_nvs_labels = set()

    while True:
        # Capture frame for DMS
        ret, frame = dms_cap.read()
        if not ret:
            print("Error: Couldn't read frame from DMS camera.")
            break

        # Capture frame for NVS
        ret, nvs_frame = nvs_cap.read()
        if not ret:
            print("Error: Couldn't read frame from NVS camera.")
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
            results1 = DMS_model1(frame)
            results2 = DMS_model2(frame)
            case = str(check_conditions(results1, results2))

            for result in [results1, results2]:
                for box in result[0].boxes:
                    x1, y1, x2, y2 = map(int, box.xyxy[0])
                    cls = int(box.cls)
                    conf = float(box.conf)
                    cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 2)

            if len(frame.shape) == 2:
                frame = cv2.cvtColor(frame, cv2.COLOR_GRAY2BGR)

            if case == "Face detected":
                frame = Head_Eye_detect(frame)
            elif case == "Face with glasses detected":
                frame = detect_head_pose(frame)
            elif case == "Face with mask detected":
                frame = Eye_Status_detection(frame)
            elif case == "Face with mask and glasses detected":
                frame = detect_head_pose(frame)
            elif case == "No face detected":
                frame = No_Face(frame)

            cv2.putText(frame, case, (20, 300), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

            # === Night Vision Processing (modular) ===
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
                    cv2.putText(enhanced_nvs, label, (x1, y1 - 10),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
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

            # Play new NVS sounds
            new_nvs_labels = current_nvs_labels - prev_nvs_labels
            for label in new_nvs_labels:
                if label in speakable_labels:
                    speak_label_once(label)
            prev_nvs_labels = current_nvs_labels.copy()

            # Display
            cv2.imshow("Driver Monitoring System", frame)
            cv2.imshow("Night Vision System", enhanced_nvs)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        except Exception as e:
            print(f"Error processing frame: {e}")
            continue

    dms_cap.release()
    nvs_cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Critical error: {e}")
