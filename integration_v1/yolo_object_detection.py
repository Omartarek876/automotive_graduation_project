import cv2
from ultralytics import YOLO


model = YOLO("yolov8n_ncnn_model") 


cap = cv2.VideoCapture(0) 

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    
    results = model(frame,imgsz=480)

    
    annotated_frame = results[0].plot()

   
    cv2.imshow("Object Detection", annotated_frame)

  
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()