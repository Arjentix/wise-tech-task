/**
 * Copyright 2020, Daniil Polyakov, All rights reserved
 */

#include <iostream>

#include <cstdlib>

#include "satellite/satellite.h"
#include "les/les.h"

using std::cout;
using std::cerr;
using std::endl;

/**
 * DEBUG_PRINT() - macro for easier debuging, prints variable name and value
 */
#define DEBUG_PRINT(x) std::cout << #x << " = " << x << std::endl

/**
 * ConcatenateMatrices() - verticaly concatenates two matrices.
 * Vector is also a matrix, but with one column.
 * 
 * @param upper - the upper matrix.
 * 
 * @param lower - the lower matrix.
 * 
 * @return - concatenated matrix.
 */
Eigen::MatrixXd ConcatenateMatrices(Eigen::MatrixXd&& upper,
                                    Eigen::MatrixXd&& lower);

/**
 * Check() - checks provided coords and prints results.
 * It computes Δ (measurement error) for every time point and calculates
 * difference between every two. Then it provides average value of this
 * difference. It computes that for every satellite and calculates general
 * average value. The closer to zero the better!
 * 
 * Complexity: O(m*n^2), where m - number of satellites, n - number of time
 * points.
 * 
 * @param satellites - array of satellites with all input data.
 * 
 * @param coords - founded x, y, z coordinates.
 */
void Check(const satellite::SatelliteArray& satellites,
           const Eigen::Vector3d& coords);

/**
 * PrintCoordinates() - prints coordinates x, y, z with additional informantion
 * 
 * @param coords - x, y, z coordinates.
 */
void PrintCoordinates(const Eigen::Vector3d& coords);

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <filename>" << endl;
    return -1;
  }

  // Speeding up input/output
  std::ios_base::sync_with_stdio(false);

  Eigen::Vector3d coords;
  try {
    // Storing information from input file
    auto [satellites, time_counter] = satellite::ReadFromFile(argv[1]);

    for (int i = 3; i <= time_counter; ++i) {
      cout << "<--- " << i << " --->" << endl;

      // Constructing matrix of coefficients and vector of values for LSM
      auto [coef_mat, value_vec] = les::ConstructCoefficientsAndValues(
          satellites, i);

      coords = les::ApplyLeastSquares(coef_mat, value_vec);
      PrintCoordinates(coords);
    }

    // Running checking
    // This is a very expensive function
    cout << "<--- Checking for last result --->" << endl;
    Check(satellites, coords);
  }
  catch (std::exception& ex) {
    cerr << ex.what() << endl;
    return -2;
  }

  return 0;
}

bool CheckIfOnEarth(const Eigen::Vector3d& coords) {
  for (auto& coord : coords) {
    if (std::abs(coord) > 6'400'000) {
      return false;
    }
  }

  return true;
}

void PrintCoordinates(const Eigen::Vector3d& coords) {
  cout << "Coordinates =\n" << coords << "\nOn Earth: " << std::boolalpha
       << CheckIfOnEarth(coords) << "\n" << endl;
}

double ComputeDistance(const Eigen::Vector3d& coords,
                       const satellite::SatelliteData& sat_data) {
  return std::sqrt(std::pow(coords[0] - sat_data.x, 2) +
                   std::pow(coords[1] - sat_data.y, 2) +
                   std::pow(coords[2] - sat_data.z, 2));
}

void Check(const satellite::SatelliteArray& satellites,
           const Eigen::Vector3d& coords) {
  double general_avg = 0;
  double general_counter = 0;

  // For every satellite compute average difference between every pair of
  // estimated measurement errors
  for (int i = 0; i < satellite::kSatelliteArraySize; ++i) {
    double avg = 0;
    int counter = 0;

    if (!satellites[i].time_to_data.empty()) {
      for (auto& [time, data] : satellites[i].time_to_data) {
        for (auto it = satellites[i].time_to_data.begin();
            it != satellites[i].time_to_data.end();
            ++it) {
          if (data != it->second) {
            ++counter;
            double estimate_error_1 = data.distance -
                                      ComputeDistance(coords, data);
            double estimate_error_2 = it->second.distance -
                                      ComputeDistance(coords, it->second);
            avg = std::abs(estimate_error_1 - estimate_error_2);
          }
        }
      }
      avg /= counter;
      general_avg += avg;
      ++general_counter;
      cout << "Average difference between estimated errors for satellite "
           << i + 1 << " = " << avg << endl;
    }
  }

  general_avg /= general_counter;
  cout << "General average difference between esitmated errors = "
       << general_avg << "\nThe closer to zero the better" << endl;
}
