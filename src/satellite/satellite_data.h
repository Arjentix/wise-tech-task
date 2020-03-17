/**
 * Copyright 2020, Daniil Polyakov, All rights reserved
 */

#pragma once

#include <istream>
#include <ostream>

namespace satellite {

struct SatelliteData {
  double x;
  double y;
  double z;
  double distance;

  bool operator!=(const SatelliteData& other) const;
};

}  // namespace satellite

std::ostream& operator<<(std::ostream& os,
                         const satellite::SatelliteData& sat_data);
std::istream& operator>>(std::istream& is, satellite::SatelliteData& sat_data);
