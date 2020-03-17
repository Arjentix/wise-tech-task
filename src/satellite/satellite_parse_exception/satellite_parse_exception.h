/**
 * Copyright 2020, Daniil Polyakov, All rights reserved
 */

#pragma once

#include <stdexcept>
#include <string>

namespace satellite {

class SatelliteParseException : public std::runtime_error {
 public:
  explicit SatelliteParseException(const std::string& message);
};

}  // namespace satellite
