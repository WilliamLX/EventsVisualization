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

  for (auto item : container)
  {
    file << item << std::endl;
  }
  file.close();
}

// using namespace acd; // Asynchronous Corner Detector

// this class will be used to analyze the events
class EventAnalyzer
{
public:
  // class variables to store global information
  std::vector<Event> event_vec;
  std::string output_dir;
  std::ofstream file;

  EventAnalyzer()
  {
  }

  EventAnalyzer(std::string file_path)
  {
    std::cout << "Output file dir: " << file_path << std::endl;
    file = std::ofstream(file_path);
  }

  // this function will be associated to the camera callback
  // it is used to compute statistics on the received events
  void analyze_events(const Metavision::EventCD *begin, const Metavision::EventCD *end)
  {
    // counting analysis
    for (const Metavision::EventCD *ev = begin; ev != end; ++ev)
    {
      // Event event = {(double)ev->t, ev->x, ev->y, ev->p};
      // event_vec.push_back(event);
      this->file << ev->t << " " << ev->x << " " << ev->y << " " << ev->p << std::endl;
    }
  }
};

// main loop
int main(int argc, char *argv[])
{
  Metavision::Camera cam; // create the camera

  EventAnalyzer event_analyzer; // create the event analyzer
  if (argc == 2)
  {
    event_analyzer = EventAnalyzer((std::string)argv[1]);
  }

  // open the first available camera
  cam = Metavision::Camera::from_first_available();

  // get camera resolution
  int camera_width = cam.geometry().width();
  int camera_height = cam.geometry().height();

  std::cout << camera_width << camera_height << std::endl;

  // add the event callback. This callback will be called periodically to provide access to the most recent events
  cam.cd().add_callback([&event_analyzer](const Metavision::EventCD *ev_begin, const Metavision::EventCD *ev_end)
                        { event_analyzer.analyze_events(ev_begin, ev_end); });

  // start the camera
  cam.start();

  // keep running while the camera is on or the video is not finished
  std::cout << "Recording ... " << std::endl;
  while (cam.is_running())
  {
  }

  cam.stop();
}