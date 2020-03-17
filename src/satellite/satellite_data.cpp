/**
 * Copyright 2020, Daniil Polyakov, All rights reserved
 */

#include "satellite/satellite_data.h"

bool satellite::SatelliteData::operator!=(const SatelliteData& other) const {
  return !(x == other.x && y == other.y && z == other.z &&
           distance == other.distance);
}

std::ostream& operator<<(std::ostream& os,
                         const satellite::SatelliteData& sat_data) {
  os << "x = " << sat_data.x << ", y = " << sat_data.y
     << ", z = " << sat_data.z << ", dist = " << sat_data.distance;

  return os;
}

std::istream& operator>>(std::istream& is, satellite::SatelliteData& sat_data) {
  is >> sat_data.x;
  is >> sat_data.y;
  is >> sat_data.z;
  is >> sat_data.distance;

  return is;
}
