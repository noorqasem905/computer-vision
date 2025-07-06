# shapes_detection

---

## 🛠️ How it Works

### 1. 🔍 Image Processing

**Function:** `image_process(cv::Mat &img)`

This function prepares the image for shape detection:

- Resizes the image to 50% of its original size
- Converts the image to grayscale
- Applies Gaussian blur to reduce noise
- Applies Canny edge detection
- Dilates the edges to strengthen the shape outlines

---

### 2. 📐 Contour Detection & Shape Classification

**Function:** `getContours(cv::Mat imgDil, cv::Mat img)`

This function does the heavy lifting for detecting and classifying shapes:

- Uses `cv::findContours` to detect contours twice:
  - One with `CHAIN_APPROX_SIMPLE` to simplify points (used for drawing)
  - One with `CHAIN_APPROX_NONE` to preserve all edge points (used for circle detection)
- Uses `cv::approxPolyDP` to simplify contours and identify corners
- Uses `cv::boundingRect` to get bounding boxes for shape labeling
- Calls `which_shape()` to determine the name/type of the shape
- Draws:
  - Purple contours around the shape
  - Green bounding boxes
  - Shape names above each shape using `cv::putText`

---

### 3. 🔁 Shape Classification Logic

**Function:** `which_shape(...)`

This function classifies shapes based on their corner count:

| Corner Count | Shape      |
|--------------|------------|
| 3            | Triangle   |
| 4            | Square or Rectangle (based on aspect ratio) |
| 5            | Pentagon   |
| 6            | Hexagon    |
| 7            | Heptagon   |
| >7           | Circle or Polygon (checked with heuristic) |

It uses `is_Square()` and `is_circle()` for more detailed analysis.

---

### 4. 🎯 Circle Detection Heuristic

**Function:** `is_circle(...)`

This function uses a heuristic to check if a shape is a circle:

- Calculates the center using image moments
- Compares total points in `CHAIN_APPROX_NONE` vs `CHAIN_APPROX_SIMPLE`
- If the shape has many points that aren't corners, it's likely a smooth circle

Returns:
- `1`: Contour is invalid
- `3`: Likely a circle
- `0`: Not a circle

---

## 🖼️ Output

- All detected shapes are outlined and labeled in the result image.
- Contours are shown in **purple**
- Bounding boxes are shown in **green**
- Text labels indicate the type of shape

---

## 📦 Dependencies

- OpenCV (tested with OpenCV 4.x)
- C++11 or higher

Make sure OpenCV is installed and `pkg-config` is available.

---

## ▶️ How to Run

1. Clone the repository and make sure your image file (e.g., `shape5.png`) is in the project folder.

2. Compile the program:

```bash
g++ main.cpp -o shape_detection `pkg-config --cflags --libs opencv4`

 # Basic usage (no resizing):
./shape_detection image.png

# Resize both width and height to 0.5 (50%):
./shape_detection image.png 0.5

# Resize width to 0.7 (70%) and height to 0.3 (30%):
./shape_detection image.png 0.7 0.3
