import cv2
from ultralytics import YOLO
import numpy as np
import mediapipe as mp
import time
import math
import pygame 
from pygame import mixer

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

# Time thresholds (in seconds)
TIME_THRESHOLD_LEFT = 2.0  # 2 seconds looking left
TIME_THRESHOLD_RIGHT = 2.0  # 2 seconds looking right
TIME_THRESHOLD_DOWN = 2.0  # 2 seconds looking down
TIME_THRESHOLD_UP = 2.0  # 2 seconds looking up
TIME_THRESHOLD_CLOSED_EYES = 1.5  # 1.5 seconds with eyes closed

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
eyes_blink = mixer.Sound('eyes_blink.wav')

# Color Definitions

BLUE = (255,0,0)
RED = (0,0,255)
GREEN = (0,255,0)

# Landmark Indices
RIGHT_EYE = [ 33, 7, 163, 144, 145, 153, 154, 155, 133, 173, 157, 158, 159, 160, 161 , 246 ] 
LEFT_EYE = [ 362, 382, 381, 380, 374, 373, 390, 249, 263, 466, 388, 387, 386, 385,384, 398 ]
LOWER_LIPS = [61, 146, 91, 181, 84, 17, 314, 405, 321, 375, 291, 308, 324, 318, 402, 317, 14, 87, 178, 88, 95]
UPPER_LIPS = [ 185, 40, 39, 37,0 ,267 ,269 ,270 ,409, 415, 310, 311, 312, 13, 82, 81, 42, 183, 78] 

# Load models
model2 = YOLO("best.pt")  # Glasses detection model
model1 = YOLO("Mask.pt")  # Face_Mask detection model

# Initialize MediaPipe Face Mesh
mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(min_detection_confidence=0.2, min_tracking_confidence=0.2)

# Logging functionality
def log_driver_behavior(event):
    """
    Log driver behavior events to a file
    """
    with open('driver_behavior_log.txt', 'a') as log_file:
        log_file.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - {event}\n")

# Time-based action tracking
def track_time_based_actions(current_pose):
    global start_time_left, start_time_right, start_time_down, start_time_up, start_time_closed_eyes
    global current_state, previous_state

    current_time = time.time()

    # Reset timers if state changes
    if current_state != previous_state:
        start_time_left = start_time_right = start_time_down = start_time_up = start_time_closed_eyes = current_time
        previous_state = current_state

    # Time-based action tracking
    if current_pose == "Looking Left":
        if current_time - start_time_left >= TIME_THRESHOLD_LEFT:
            print("WARNING: Looking Left for too long!")
            log_driver_behavior("Prolonged Looking Left")
            voice_left.play()
            start_time_left = current_time

    elif current_pose == "Looking Right":
        if current_time - start_time_right >= TIME_THRESHOLD_RIGHT:
            print("WARNING: Looking Right for too long!")
            log_driver_behavior("Prolonged Looking Right")
            voice_right.play()
            start_time_right = current_time

    elif current_pose == "Looking Down":
        if current_time - start_time_down >= TIME_THRESHOLD_DOWN:
            print("WARNING: Looking Down for too long!")
            log_driver_behavior("Prolonged Looking Down")
            voice_down.play()
            start_time_down = current_time

    elif current_pose == "Looking Up":
        if current_time - start_time_up >= TIME_THRESHOLD_UP:
            print("WARNING: Looking Up for too long!")
            log_driver_behavior("Prolonged Looking Up")
            start_time_up = current_time

    elif current_pose == "Closed Eyes":
        if current_time - start_time_closed_eyes >= TIME_THRESHOLD_CLOSED_EYES:
            print("WARNING: Eyes Closed for too long!")
            log_driver_behavior("Prolonged Eyes Closed")
            eyes_blink.play()
            start_time_closed_eyes = current_time

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

    #image = cv2.cvtColor(cv2.flip(image,1),cv2.COLOR_BGR2RGB)
    image = cv2.cvtColor(image,cv2.COLOR_BGR2RGB)
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

def main():
    cap = cv2.VideoCapture(0)
    
    # FPS calculation variables
    prev_time = time.time()
    fps = 0
    frame_count = 0
    
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break
        
        # Calculate FPS
        current_time = time.time()
        frame_count += 1
        if current_time - prev_time >= 1.0:  # Update every second
            fps = frame_count
            frame_count = 0
            prev_time = current_time
        
        try:
            # Perform detections
            results1 = model1(frame)  # Mask detection
            results2 = model2(frame)  # Face detection
            case = str(check_conditions(results1, results2))

            # Draw YOLO detections manually to avoid errors
            for result in [results1, results2]:
                for box in result[0].boxes:
                    x1, y1, x2, y2 = map(int, box.xyxy[0])  # Convert bounding box to int
                    cls = int(box.cls)
                    conf = float(box.conf)
                    label = f"{cls}: {conf:.2f}"
                    
                    cv2.rectangle(frame, (x1, y1), (x2, y2), (0,0,255), 2)
            
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

            # Display FPS
           # cv2.putText(frame, f"FPS: {fps}", (10, 30), 
            #            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
            
            # Display classification result
            cv2.putText(frame, case, (20, 450), 
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

            cv2.imshow("Face Detection", frame)

            # Exit condition
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        
        except Exception as e:
            print(f"Error processing frame: {e}")
            continue
    
    # Cleanup
    cap.release()
    cv2.destroyAllWindows()

# Initialization and Entry Point
if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Critical error: {e}")
        log_driver_behavior(f"Application Crashed: {e}")