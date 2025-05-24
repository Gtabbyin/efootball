#include "roboteq_driver/controller.h"
#include "roboteq_driver/channel.h"

#include "roboteq_msgs/Status.h"
#include "serial/serial.h"

#include <boost/bind.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <unistd.h>
#include <iostream>
#include <sstream>

// Link to generated source from Microbasic script file.
extern const char* script_lines[];
extern const int script_ver = 28;

namespace roboteq {

const std::string eol("\r");
const size_t max_line_length(128);

Controller::Controller(const char *port, int baud)
  : nh_("~"), port_(port), baud_(baud), connected_(false), receiving_script_messages_(false),
    version_(""), start_script_attempts_(0), serial_(NULL),
    command("!", this), query("?", this), param("^", this)
{
  pub_status_ = nh_.advertise<roboteq_msgs::Status>("status", 1);
}

Controller::~Controller() {
}

void Controller::addChannel(Channel* channel) {
  channels_.push_back(channel);
}
