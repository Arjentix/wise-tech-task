/**
 * Copyright 2020, Daniil Polyakov, All rights reserved
 */

#include "satellite/satellite.h"

#include <fstream>
#include <sstream>

#include "satellite/satellite_parse_exception/satellite_parse_exception.h"

std::pair<satellite::SatelliteArray, std::size_t> satellite::ReadFromFile(
    const std::string& file_path) {
  satellite::SatelliteArray satellites;
  std::ifstream file(file_path);
  if (!file) {
    throw std::runtime_error("Can't open file");
  }

  std::size_t time_counter = 1;
  while (!file.eof()) {
    std::string line;
    std::getline(file, line);

    if (line != "") {
      std::istringstream iss(line);
      int sat_num;
      SatelliteData sat_data;

      iss >> sat_num >> sat_data;
      if (iss.fail()) {
        throw satellite::SatelliteParseException(
          "Error in parsing satellite data");
      }

      if (sat_num > 0 && sat_num <= kSatelliteArraySize) {
        satellites[sat_num - 1].time_to_data[time_counter] = sat_data;
      }

    } else {
      ++time_counter;
    }
  }

  return {satellites, time_counter};
}

std::ostream& operator<<(std::ostream& os,
                         const satellite::Satellite& satellite) {
  os << '{';

  bool first = true;
  for (const auto& [time, data] : satellite.time_to_data) {
    if (!first) {
      os << ",\n";
    }
    os << time << ": " << data;

    first = false;
  }

  os << '}';
  return os;
}

std::ostream& operator<<(std::ostream& os,
                         const satellite::SatelliteArray& satellites) {
  os << "[\n";

  bool first = true;
  for (satellite::Satellite sat : satellites) {
    if (!first) {
      os << ",\n";
    }
    os << sat;

    first = false;
  }

  os << ']';
  return os;
}
