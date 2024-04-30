#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>

void addTexture() {

  //  VideoCapture cap(0);  // Change to 1 or 2 if you have multiple webcams

  //  if (!cap.isOpened()) {
  // 	 cout << "Error: Failed to open the webcam." << endl;
  // 	 return;
  //  }

  //  // Set the desired resolution (e.g., 1920x1080)
  //  cap.set(CAP_PROP_FRAME_WIDTH, 1920);
  //  cap.set(CAP_PROP_FRAME_HEIGHT, 1080);

  //  // Check if the webcam successfully set the resolution
  //  int width = cap.get(CAP_PROP_FRAME_WIDTH);
  //  int height = cap.get(CAP_PROP_FRAME_HEIGHT);
  //  cout << "Resolution set to: " << width << "x" << height << endl;

  cv::Mat textureImage = cv::imread("canvas.jpg", cv::IMREAD_GRAYSCALE);

  // Mat memImg; //memory image aka afterimage
  cv::Mat img = cv::imread("inputImage.jpg", cv::IMREAD_COLOR); // 3 channel bgr

  //  cap.read(img);
  if (!(textureImage.cols >= img.cols) || !(textureImage.rows >= img.rows)) {
    std::cout << " IMAGE IS TOO BIG FOR TEXTURE IMAGE" << std::endl;
    return;
  }

  cv::Rect matchRoi(0, 0, img.cols, img.rows);
  cv::Mat tempTexture = textureImage(matchRoi);

  cv::Mat texture;
  tempTexture.convertTo(texture, CV_32F, 1.0 / 255.0);

  // Normalize the texture image to the range [0, 255]
  double minVal, maxVal;
  minMaxLoc(texture, &minVal,
            &maxVal); // Find min and max intensity values in the texture image

  cv::Mat normalizedTexture;
  texture.convertTo(normalizedTexture, CV_8U, 255.0 / (maxVal - minVal),
                    -minVal * 255.0 / (maxVal - minVal));
  // cv::imshow("normalized texture", normalizedTexture);

  // std::cout << "x "<< img.cols <<"y "<< img.rows << std::endl;
  /* DISCLAIMER: CHANGING THIS TO TRUE CREATES AN INFINITE LOOP */
  bool again = false;
  bool saveImage = false;
  //  float satMargin = 0.1; // not used
  float valMargin = 0.4;
  float textureMidpoint = 0.7;

  // cv::imshow("picture", img);
  // cv::imshow("texture", texture);
  // cv::waitKey(0);

  do {
    //  cap.read(img);
    cv::Mat hsvImage;
    cvtColor(img, hsvImage, cv::COLOR_BGR2HSV);
    // Mat scaledSaturation, scaledValue;
    cv::Mat scaledSaturation(img.size(), CV_32F);
    cv::Mat scaledValue(img.size(), CV_32F);
    cv::Mat outputImage = img.clone();

    // Iterate over each pixel in the normalized texture and adjust image
    // brightness based on texture intensity
    for (int y = 0; y < normalizedTexture.rows; y++) {
      for (int x = 0; x < normalizedTexture.cols; x++) {
        // Retrieve the normalized texture intensity [0, 1]
        float texIntensityNormalized =
            static_cast<float>(normalizedTexture.at<uchar>(y, x)) / 255.0;

        // Calculate the brightness adjustment based on texture intensity
        float brightnessChange = (texIntensityNormalized - textureMidpoint) *
                                 valMargin; // Calculate change from midpoint

        // Apply brightness adjustment to the corresponding pixel in the image
        cv::Vec3b &hsvPixel = hsvImage.at<cv::Vec3b>(y, x);

        hsvPixel[2] = cv::saturate_cast<uchar>(
            (hsvPixel[2] > 10 ? hsvPixel[2] : 10) +
            brightnessChange * 255.0f); // Modify value component
      }
    }

    // Convert HSV image back to BGR color space
    cv::cvtColor(hsvImage, outputImage, cv::COLOR_HSV2BGR);

    if (saveImage) {
      // Save the processed image to a file
      std::string outputPath = "output.jpg";
      bool result = cv::imwrite(outputPath, outputImage);

      if (result) {
        std::cout << "Image saved successfully as: " << outputPath << std::endl;
      } else {
        std::cerr << "Error: Failed to save image." << std::endl;
        return;
      }
    }
    cv::imshow("final", outputImage);
    cv::waitKey(0); // wait for key press to continue to next frame

  } while (again); // please note this is an infinite loop
}
int main() {

  addTexture();
  return 0;
}
