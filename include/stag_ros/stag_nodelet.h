/**
MIT License

Copyright (c) 2020 Michail Kalaitzakis, Brennan Cain (Unmanned Systems and
Robotics Lab, University of South Carolina, USA)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

#include <nodelet/nodelet.h>

// ROS includes
#include "ros/ros.h"
#include "cv_bridge/cv_bridge.h"
#include "image_transport/image_transport.h"
#include "camera_info_manager/camera_info_manager.h"

// ROS msgs
#include "sensor_msgs/image_encodings.h"

// Stag includes
#include "../../lib/stag_core/Stag.h"
#include "stag_ros/structures.hpp"
#include "stag_ros/tagJsonLoader.hpp"

namespace stag_ros {

class StagNodelet : public nodelet::Nodelet {
 public:
  virtual ~StagNodelet();
  virtual void onInit();

 private:
  // Callbacks
  void imageCallback(const sensor_msgs::ImageConstPtr &msg);
  void cameraInfoCallback(const sensor_msgs::CameraInfoConstPtr &msg);

  // Functions
  void loadParameters(const ros::NodeHandle &nh);
  bool getBundleIndex(const int id, int &bundle_index, int &tag_index);
  bool getTagIndex(const int id, int &tag_index);
  void solvePnp(const std::vector<cv::Point2d> &img,
      const std::vector<cv::Point3d> &world,
      cv::Mat &output
      );
  // ROS Subcribers
  image_transport::Subscriber imageSub;
  ros::Subscriber cameraInfoSub;

  // ROS Publishers
  ros::Publisher markersPub;
  image_transport::Publisher imageDebugPub;

  // STag handle
  Stag *stag;
  int stagLib;
  int errorC;
  double tagSize;
  std::vector<cv::Point3d> tagCorners;

  // Data
  cv::Mat cameraMatrix;
  cv::Mat distortionMat;
  cv::Mat rectificationMat;
  cv::Mat projectionMat;
  bool gotCamInfo;
  bool debugI;
  bool isCompressed;
  std::string imageTopic;
  std::string cameraInfoTopic;
  std::string tag_tf_prefix;

  // Tag and bundle info
  std::vector<Bundle> bundles;
  std::vector<Tag> tags;

};

}  // namespace stag_ros