/**
 * Copyright 2020, Daniil Polyakov, All rights reserved
 */

#pragma once

#include <map>
#include <array>
#include <vector>
#include <string>
#include <ostream>
#include <istream>
#include <utility>

#include "satellite/satellite_data.h"

namespace satellite {

const unsigned kSatelliteArraySize = 25;

struct Satellite {
  std::map<int, SatelliteData> time_to_data;
};

using SatelliteArray = std::array<Satellite, kSatelliteArraySize>;

/**
 * ReadFromFile() - constructs SatelliteArray from file.
 * 
 * @param file_path - path to the file.
 * 
 * @return pair of SatelliteArray and counter of time measurement.
 */
std::pair<SatelliteArray, std::size_t> ReadFromFile(
    const std::string& file_path);

}  // namespace satellite

std::ostream& operator<<(std::ostream& os,
                         const satellite::Satellite& satellite);

std::ostream& operator<<(std::ostream& os,
                         const satellite::SatelliteArray& satellites);
