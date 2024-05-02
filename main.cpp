#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <vector>

// FORWARD DECLARATION OF UNCHANGEABLE CODE (below Main) AKA DONE LIST
void display(cv::Mat &mat, std::string title);
void save(cv::Mat &mat, std::string imageName);
cv::Mat normalize(cv::Mat &texture);

// START OF DOCUMENT

// Only translate the Hue value from one image to another
void transferColorOnly(cv::Mat &img, cv::Mat &textureImage) {

  float arbitrary_crossover_point = 0.8;
  /* in this function, we aim to color the texture, as opposed to texture the
  input image. the idea is simple, we use HSV color space keeping in mind H is
  an integer value 0 - 179 s is float 0-1, and v is float 0-1. we keep the H
  unscathed. we can change the saturation slightly,however the only reason I'd
  do this is to accentuate "white" the V  value is basically the darkness.

  Considerations:
  1.) Saturation Changes during lightening (decrease)
  if the input image as very bright highlights we can slightly
  take from the saturation but this washes out the color, so to combat this,
  we'll only take away from saturation once the V is maxed out - think of it as
  being linearly interpolated with "anything where crosses over 0.8" the
  increase then will reflect in both v and s.
  2.) Saturation changes, during darkening (increase)
  When it comes to darkness, the
  reason we'll increase saturation again is to preserve the original color going
  into it. so far I do not think it is necessary, apart from light colors, like
  clouds in the sky, to accentuate texture.

  */

  cv::Mat hsvImg;
  // cv::Mat hsvTexture;
  if (img.channels() == 3) // expected for img (and bgra for screenshots)
    cvtColor(img, hsvImg, cv::COLOR_BGR2HSV);
  else if (img.channels() == 4) // expected for img (and bgra for screenshots)
  {
    /// TODO: extract Alpha, to preserve it before conversion
    cvtColor(img, hsvImg, cv::COLOR_BGR2HSV);
  } else
    return; // not even going to need this function here, use the two grayscale
            // images.

  if (textureImage.channels() !=
      1) // if the texture is no longer grayscale - then something is not right.
    return;

  // use the value of the texture image at any given point divided by 255 to
  // ensure that it is the same as the V of HSV (between 0 and 1)

  // takes the hue value transfer
}

// ADDS TEXTURE MAT to input mat, with additional Blending.
void addTexture(cv::Mat &img, cv::Mat &textureImage, float valMargin = 0.3,
                float textureMidpoint = 0.7,float blendBalance =0.3) {
                  

  // LAZY QUICK EXIT TO AVOID HANDLING THE PROBLEM just match sizes.
  if ((textureImage.cols < img.cols) || (textureImage.rows < img.rows)) {
    std::cout << " IMAGE IS TOO BIG FOR TEXTURE IMAGE" << std::endl;
    return;
    cv::Mat resizedImage;
    // Resize the source image to the target dimensions using linear
    // interpolation
    cv::resize(textureImage, resizedImage, cv::Size(img.cols, img.rows));
    textureImage = resizedImage.clone();
  }
  // // // Crop the texture to match the input image's size.
  cv::Rect matchRoi(0, 0, img.cols, img.rows);
  cv::Mat tempTexture = textureImage(matchRoi);

  cv::Mat texture;
  tempTexture.convertTo(texture, CV_32F, 1.0 / 255.0);

  cv::Mat normalizedTexture = normalize(texture);

  cv::Mat hsvImage;
  cvtColor(img, hsvImage, cv::COLOR_BGR2HSV);
  cv::Mat outputImage = img.clone();

  // added step: blend the normalized image with the input, then do the next step: 
  cv::Mat matchingNormTexture;
  // matchingNormTexture=normalizedTexture.clone();
  cv::cvtColor(normalizedTexture,matchingNormTexture, cv::COLOR_GRAY2BGR);
  cv::addWeighted(img, 1.0-blendBalance,matchingNormTexture, blendBalance,0.0, img);

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
      
      // recolor the image from the regular one
    }
  }

  // Convert HSV image back to BGR color space
  cv::cvtColor(hsvImage, outputImage, cv::COLOR_HSV2BGR);
  img = outputImage.clone();
}

///  IDEA: have the program run this on all images in a folder. 
/// void batchRun() { 
//   // for each image in input, apply each texture in texture, and save in
//   output
//   // under [inputName]_textured.jpg
// }
int main() {
  std::string name = "outputImage";
  // change here as you see fit
  cv::Mat image = cv::imread("inputImage.jpg", cv::IMREAD_COLOR);
  cv::Mat texture = cv::imread("canvas.jpg", cv::IMREAD_GRAYSCALE);



  addTexture(image, texture);
  display(image,"debug");
  // save(image, name);
  return 0;
}

// START OF DONE STUFF

void save(cv::Mat &mat, std::string imageName) {
  // Save the image to a file

  std::string outputPath = "output/" + imageName + ".jpg";
  // cv::imshow(outputPath,mat);
  // cv::waitKey(0);

  bool result = cv::imwrite(outputPath, mat);

  if (result) {
    std::cout << "Image saved successfully as: " << outputPath << std::endl;
  } else {
    std::cerr << "Error: Failed to save image." << std::endl;
    return;
  }
}
void display(cv::Mat &mat, std::string title="PRESS SPACE TO CONTINUE") {
  cv::imshow(title, mat);
  cv::waitKey(0);
}
cv::Mat normalize(cv::Mat &texture) {
  // Normalize the texture image to the range [0, 255]
  double minVal, maxVal;
  minMaxLoc(texture, &minVal, &maxVal);
  // Find min and max intensity values in the texture image
  cv::Mat normalizedTexture;
  texture.convertTo(normalizedTexture, CV_8U, 255.0 / (maxVal - minVal),
                    -minVal * 255.0 / (maxVal - minVal));
  return normalizedTexture;
}