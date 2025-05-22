from ultralytics import YOLO
import cv2

model = YOLO("D:\\Documents\\GP_DATA\\Night_Vision_System_Implemetation\\Datasets\\Used_Datasets\\objects_at_the_road\\NIGHT\\USED\\Greyscale\\models\\nano_version4\\best.pt")  

video_path = "Cairo 4K - Night Drive - Driving Downtown(360P).mp4"
cap = cv2.VideoCapture(video_path)  

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        print("Finished processing video.")
        break

    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    gray_frame = cv2.merge([gray_frame, gray_frame, gray_frame])  

    results = model(gray_frame, conf=0.25, device='cpu')

    for r in results:
        gray_frame = r.plot()

    cv2.imshow("YOLOv8 Video Detection (Greyscale)", gray_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
