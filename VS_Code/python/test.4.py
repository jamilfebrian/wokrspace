import cv2
import numpy as np
import tensorflow as tf

# Load the COCO-SSD model
model = tf.keras.models.load_model('models/efficientdet_d0_coco17_tpu-32/saved_model')

# Set the class names
class_names = ['background', 'person', 'bicycle', 'car', 'motorcycle',
               'airplane', 'bus', 'train', 'truck', 'boat', 'traffic light',
               'fire hydrant', 'stop sign', 'parking meter', 'bench', 'bird',
               'cat', 'dog', 'horse', 'sheep', 'cow', 'elephant', 'bear',
               'zebra', 'giraffe', 'backpack', 'umbrella', 'handbag', 'tie',
               'suitcase', 'frisbee', 'skis', 'snowboard', 'sports ball',
               'kite', 'baseball bat', 'baseball glove', 'skateboard',
               'surfboard', 'tennis racket', 'bottle', 'wine glass', 'cup',
               'fork', 'knife', 'spoon', 'bowl', 'banana', 'apple',
               'sandwich', 'orange', 'broccoli', 'carrot', 'hot dog', 'pizza',
               'donut', 'cake', 'chair', 'couch', 'potted plant', 'bed',
               'dining table', 'toilet', 'tv', 'laptop', 'mouse', 'remote',
               'keyboard', 'cell phone', 'microwave', 'oven', 'toaster',
               'sink', 'refrigerator', 'book', 'clock', 'vase', 'scissors',
               'teddy bear', 'hair drier', 'toothbrush']

# Open the video capture device
cap = cv2.VideoCapture(0)

while True:
    # Read a frame from the video capture device
    ret, frame = cap.read()

    # Convert the frame to RGB
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # Resize the frame
    input_tensor = tf.convert_to_tensor(frame_rgb)
    input_tensor = tf.image.resize(input_tensor, (320, 320))
    input_tensor = input_tensor[tf.newaxis, ...]

    # Detect objects in the frame
    detections = model(input_tensor)

    # Loop through each detection and draw a rectangle around it
    for i in range(len(detections['detection_scores'][0])):
        score = detections['detection_scores'][0][i]
        class_id = int(detections['detection_classes'][0][i])
        bbox = detections['detection_boxes'][0][i]
        if score > 0.7 and class_id == 1:
            h, w, _ = frame.shape
            ymin, xmin, ymax, xmax = bbox.numpy()
            xmin = int(xmin * w)
            xmax = int(xmax * w)
            ymin = int(ymin * h)
            ymax = int(ymax * h)
            cv2.rectangle(frame, (xmin, ymin), (xmax, ymax), (0, 255, 0), 2)
            cv2.putText(frame, class_names[class_id], (xmin, ymin - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    # Show the frame
    cv2.imshow('Body Detection', frame)

    # Press 'q' to quit
    if cv2.waitKey(1) == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
