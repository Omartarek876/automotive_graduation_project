from ultralytics import YOLO

# Load a YOLO11n PyTorch model
model = YOLO("yolov8n.pt")

# Export the model to NCNN format
model.export(format="ncnn", imgsz=480)  # creates 'yolov11n-pose_ncnn_model