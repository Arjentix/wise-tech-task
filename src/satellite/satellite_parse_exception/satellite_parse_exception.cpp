/**
 * Copyright 2020, Daniil Polyakov, All rights reserved
 */

#include "satellite/satellite_parse_exception/satellite_parse_exception.h"

satellite::SatelliteParseException::SatelliteParseException(
    const std::string& message)
    : std::runtime_error(message) {}
