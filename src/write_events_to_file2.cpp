#include <metavision/sdk/driver/camera.h>
#include <metavision/sdk/base/events/event_cd.h>
#include "metavision/sdk/core/utils/cd_frame_generator.h"
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// #include "acd/arc_star_detector.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
// #include <time.h>

typedef std::chrono::high_resolution_clock Clock;

struct Event
{
  double t;
  int x, y;
  bool p;
};

// using namespace acd; // Asynchronous Corner Detector

// this class will be used to analyze the events
class EventAnalyzer
{
public:
  // class variables to store global information
  std::vector<Event> event_vec;
  const size_t n_event = 30e6;
  const size_t expected_num_event = 30e6;

  EventAnalyzer()
  {
    event_vec.clear();                     // Event vec must be empty
    event_vec.reserve(expected_num_event); // Preallocate vectors
  }

  // this function will be associated to the camera callback
  // it is used to compute statistics on the received events
  void analyze_events(const Metavision::EventCD *begin, const Metavision::EventCD *end)
  {
    // record events
    for (const Metavision::EventCD *ev = begin; ev != end; ++ev)
    {
      Event event = {(double)ev->t, ev->x, ev->y, ev->p};
      event_vec.push_back(event);
    }
  }

  int get_event_vec_size()
  {
    event_vec.shrink_to_fit(); // Free memory if possible
    return event_vec.size();
  }

  std::vector<Event> get_event_vec()
  {
    event_vec.shrink_to_fit(); // Free memory if possible
    return event_vec;
  }
};

template <class T>
int WriteContainerToFile(const std::string &file_path, const T &container)
{
  std::ofstream file(file_path);

  if (file.fail())
  {
    std::cout << "Error creating/opening file" << std::endl
              << std::flush;
    return -1;
  }

  for (Event item : container)
  {
    file << item.t << " " << item.x << " " << item.y << " " << item.p << "\n";
  }
  file.close();
}

// main loop
int main(int argc, char *argv[])
{
  Metavision::Camera cam;       // create the camera
  EventAnalyzer event_analyzer; // create the event analyzer

  // open the first available camera
  cam = Metavision::Camera::from_first_available();

  // get camera resolution
  int camera_width = cam.geometry().width();
  int camera_height = cam.geometry().height();
  std::cout << camera_width << camera_height << std::endl;

  // prepare event vector
  const size_t n_event = 30e6;
  const size_t expected_num_event = 30e6;
  std::vector<Event> event_vec;
  event_vec.clear();                     // Event vec must be empty
  event_vec.reserve(expected_num_event); // Preallocate vectors

  // add the event callback. This callback will be called periodically to provide access to the most recent events
  cam.cd().add_callback([&event_analyzer](const Metavision::EventCD *ev_begin, const Metavision::EventCD *ev_end)
                        { event_analyzer.analyze_events(ev_begin, ev_end); });

  // create a frame generator for visualization
  // this will get the events from the callback and accumulate them in a cv::Mat
  Metavision::CDFrameGenerator cd_frame_generator(camera_width, camera_height);

  const int fps = 25;                                                   // event-based cameras do not have a frame rate, but we need one for visualization
  const int wait_time = static_cast<int>(std::round(1.f / fps * 1000)); // how much we should wait between two frames
  cv::Mat cd_frame;                                                     // frame where events will be accumulated
  const std::string window_name = "Metavision SDK Get Started";

  // this function is used to tell the frame generator what to do with the frame and how often to generate it
  cd_frame_generator.start(
      fps, [&cd_frame](const Metavision::timestamp &ts, const cv::Mat &frame)
      { frame.copyTo(cd_frame); });

  cv::namedWindow(window_name, CV_GUI_EXPANDED);
  cv::resizeWindow(window_name, camera_width, camera_height);

  // start the camera
  cam.start();

  // keep running while the camera is on or the video is not finished
  std::cout << "Recording ... " << std::endl;
  // keep running while the camera is on or the video is not finished
  while (cam.is_running())
  {
    // display the frame if it's not empty
    if (!cd_frame.empty())
    {
      cv::imshow(window_name, cd_frame);
    }

    // if the user presses the `q` key, quit the loop
    if ((cv::waitKey(wait_time) & 0xff) == 'q')
    {
      break;
    }
  }

  cam.stop();

  std::cout << "Flush here. " << std::endl;
  std::cout << "Events size: " << std::endl;
  std::cout << event_analyzer.get_event_vec_size() << std::endl;

  if (argc == 2)
  {
    std::string corner_file_path = argv[1];
    std::cout << "Writing corner classification to file..." << std::flush;
    WriteContainerToFile(corner_file_path, event_analyzer.get_event_vec());
    std::cout << "OK" << std::endl;
  }
}