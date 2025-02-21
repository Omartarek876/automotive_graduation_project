import cv2
from ultralytics import YOLO
import numpy as np
import mediapipe as mp
import time
import math
import pygame 
from pygame import mixer

mixer.init()
voice_left = mixer.Sound('left.wav')
voice_right = mixer.Sound('Right.wav')
voice_down = mixer.Sound('down.wav')
eyes_blink= mixer.Sound('eyes_blink.wav')
counter_right=0
counter_down=0
counter_left=0
FONTS =cv2.FONT_HERSHEY_COMPLEX
RIGHT_EYE=[ 33, 7, 163, 144, 145, 153, 154, 155, 133, 173, 157, 158, 159, 160, 161 , 246 ] 
LEFT_EYE =[ 362, 382, 381, 380, 374, 373, 390, 249, 263, 466, 388, 387, 386, 385,384, 398 ]
LOWER_LIPS =[61, 146, 91, 181, 84, 17, 314, 405, 321, 375, 291, 308, 324, 318, 402, 317, 14, 87, 178, 88, 95]
UPPER_LIPS=[ 185, 40, 39, 37,0 ,267 ,269 ,270 ,409, 415, 310, 311, 312, 13, 82, 81, 42, 183, 78] 
BLACK = (0,0,0)
WHITE = (255,255,255)
LIGHTBLUE = (180,130,70)
BLUE = (255,0,0)
RED = (0,0,255)
CYAN = (255,255,0)
YELLOW =(0,255,255)
MAGENTA = (255,0,255)
GRAY = (128,128,128)
GREEN = (0,255,0)
PURPLE = (128,0,128)
ORANGE = (0,165,255)
PINK = (147,20,255)


# Load models
model2 = YOLO("best.pt")  # Glasses detection model
model1 = YOLO("Mask.pt")  # Fase_Mask detection model

mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(min_detection_confidence=0.2, min_tracking_confidence=0.2)

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

def detect_head_pose(image):
    start = time.time()

    image = cv2.cvtColor(cv2.flip(image,1),cv2.COLOR_BGR2RGB) #flipped for selfie view

    image.flags.writeable = False

    results = face_mesh.process(image)

    image.flags.writeable = True

    image = cv2.cvtColor(image,cv2.COLOR_RGB2BGR)

    img_h , img_w, img_c = image.shape
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


            #Get 2d Coord
            face_2d = np.array(face_2d,dtype=np.float64)

            face_3d = np.array(face_3d,dtype=np.float64)

            focal_length = 1 * img_w

            cam_matrix = np.array([[focal_length,0,img_h/2],
                                  [0,focal_length,img_w/2],
                                  [0,0,1]])
            distortion_matrix = np.zeros((4,1),dtype=np.float64)

            success,rotation_vec,translation_vec = cv2.solvePnP(face_3d,face_2d,cam_matrix,distortion_matrix)


            #getting rotational of face
            rmat,jac = cv2.Rodrigues(rotation_vec)

            angles,mtxR,mtxQ,Qx,Qy,Qz = cv2.RQDecomp3x3(rmat)

            x = angles[0] * 360
            y = angles[1] * 360
            z = angles[2] * 360

            #here based on axis rot angle is calculated
            if y < -10:
                text="Looking Left"
                if pygame.mixer.get_busy()==0:
                  voice_left.play()
            elif y > 10:
                text="Looking Right"
                if pygame.mixer.get_busy()==0:
                  voice_right.play()
            elif x < -4:
                text="Looking Down"
                if pygame.mixer.get_busy()==0:
                  voice_down.play()
            elif x > 10:
                text="Looking Up"
            else:
                text="Forward"

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

        cv2.putText(image,f'FPS: {int(fps)}',(20,450),cv2.FONT_HERSHEY_SIMPLEX,1.5,(0,255,0),2)

        return image  # Return modified image and text

    return image, "No Face Detected" 

def landmarksDetection(img, results, draw=False):
    img_height, img_width= img.shape[:2]
    mesh_coord = [(int(point.x * img_width), int(point.y * img_height)) for point in results.multi_face_landmarks[0].landmark]
    if draw :
        [cv2.circle(img, p, 2, (0,255,0), -1) for p in mesh_coord]
    return mesh_coord

def euclaideanDistance(point, point1):
    x, y = point
    x1, y1 = point1
    distance = math.sqrt((x1 - x)**2 + (y1 - y)**2)
    return distance

def blinkRatio(img, landmarks, right_indices, left_indices):
    # Right eyes 
    # horizontal line 
    rh_right = landmarks[right_indices[0]]
    rh_left = landmarks[right_indices[8]]
    # vertical line 
    rv_top = landmarks[right_indices[12]]
    rv_bottom = landmarks[right_indices[4]]
    # draw lines on right eyes 
    #cv2.line(img, rh_right, rh_left, GREEN, 2)
    #cv2.line(img, rv_top, rv_bottom, WHITE, 2)

    # LEFT_EYE 
    # horizontal line 
    lh_right = landmarks[left_indices[0]]
    lh_left = landmarks[left_indices[8]]

    # vertical line 
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

def colorBackgroundText(img, text, font, fontScale, textPos, textThickness=1,textColor=(0,255,0), bgColor=(0,0,0), pad_x=3, pad_y=3):
  
    (t_w, t_h), _= cv2.getTextSize(text, font, fontScale, textThickness) # getting the text size
    x, y = textPos
    cv2.rectangle(img, (x-pad_x, y+ pad_y), (x+t_w+pad_x, y-t_h-pad_y), bgColor,-1) # draw rectangle 
    cv2.putText(img,text, textPos,font, fontScale, textColor,textThickness ) # draw in text

    return img



def Head_Eye_detect (image):
    
    
    Threshold_Frame = [200,350,450]
    counter = 0
    counter_eye = 0
    Counter_right=0
    Counter_down=0
    Counter_left=0
    counter_left=0
    counter_right=0
    counter_down=0
    # Flip the image horizontally for a later selfie-view display
    # Also convert the color space from BGR to RGB
    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)

    # To improve performance
    image.flags.writeable = False
    
    # Get the result
    results = face_mesh.process(image)
    
    # To improve performance
    image.flags.writeable = True
    
    # Convert the color space from RGB to BGR
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

                    # Get the 2D Coordinates
                    face_2d.append([x, y])

                    # Get the 3D Coordinates
                    face_3d.append([x, y, lm.z])       
            
            # Convert it to the NumPy array
            face_2d = np.array(face_2d, dtype=np.float64)

            # Convert it to the NumPy array
            face_3d = np.array(face_3d, dtype=np.float64)

            # The camera matrix
            focal_length = 1 * img_w

            cam_matrix = np.array([ [focal_length, 0, img_h / 2],
                                    [0, focal_length, img_w / 2],
                                    [0, 0, 1]])

            # The Distance Matrix
            dist_matrix = np.zeros((4, 1), dtype=np.float64)

            # Solve PnP
            success, rot_vec, trans_vec = cv2.solvePnP(face_3d, face_2d, cam_matrix, dist_matrix)

            # Get rotational matrix
            rmat, jac = cv2.Rodrigues(rot_vec)

            # Get angles
            angles, mtxR, mtxQ, Qx, Qy, Qz = cv2.RQDecomp3x3(rmat)

            # Get the y rotation degree
            x = angles[0] * 360
            y = angles[1] * 360

            ratio = blinkRatio(image, landmarksDetection(image, results, False), RIGHT_EYE, LEFT_EYE)
            if y < -10:
                text="Looking Left"
            elif y > 10:
                text="Looking Right"
            elif x < -4:
                text="Looking Down"
            elif x > 10:
                text="Looking Up"
            elif ratio > 4.0:
                text="Closed Eyes"    
            else:
                text="Forward"
             
            if y< -10:
                Counter_right=0
                Counter_down=0
                Counter_forward=0
                counter_down=0
                counter_right=0
                if Counter_left % Threshold_Frame[counter_left] == 0  and pygame.mixer.get_busy()==0:
                    counter_left +=1
                    counter_left = counter_left % 3
                    if counter_left == 0:
                        Counter_left = 0
                    
                    voice_left.play()
            
            if  y > 10:
                Counter_left=0
                Counter_down=0
                Counter_forward=0
                counter_left=0
                counter_down=0
                if Counter_right % Threshold_Frame[counter_right] == 0 and pygame.mixer.get_busy()==0:
                    
                    counter_right +=1
                    counter_right = counter_right % 3  
                    
                    if counter_right == 0:
                        Counter_right = 0
                    
                    voice_right.play()
                    
       
            if x < -4:
                Counter_right=0
                Counter_left=0
                Counter_forward=0
                if Counter_down % Threshold_Frame[counter_down] == 0 and pygame.mixer.get_busy()==0:

                    counter_down +=1
                    counter_down = counter_down % 3  
                    
                    if counter_down == 0:
                        Counter_down = 0                    
                    voice_down.play()
            
            
            
            frame_height, frame_width= image.shape[:2] 
            
            if results.multi_face_landmarks:
                mesh_coords = landmarksDetection(image, results, False)
                ratio = blinkRatio(image, mesh_coords, RIGHT_EYE, LEFT_EYE)
                
                
                print (ratio)
                if ratio > 4.0:
                    counter_eye += 1
                    if counter_eye < 30 and pygame.mixer.get_busy()==0:
                        eyes_blink.play()
                        counter_eye = 0
                else: 
                    counter_eye=0 
                p1 = (int(nose_2d[0]),int(nose_2d[1]))
                p2 = (int(nose_2d[0] + y*10), int(nose_2d[1] -x *10))

                cv2.line(image,p1,p2,(255,0,0),3)
                cv2.putText(image,text,(20,50),cv2.FONT_HERSHEY_SIMPLEX,2,(0,255,0),2)
                
                
        return image  # Return modified image and text

    return image, "No Face Detected"        

def main():
    
    cap = cv2.VideoCapture(0)  # Uncomment for webcam


    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

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
        
        if case == "Face detected":
            frame = Head_Eye_detect (frame)
        elif case == "Face with glasses detected":
            frame = detect_head_pose(frame)

        # Display classification result
        cv2.putText(frame, case, (10, 350), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

        cv2.imshow("Face Detection", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
