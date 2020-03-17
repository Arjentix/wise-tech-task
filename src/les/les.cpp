/**
 * Copyright 2020, Daniil Polyakov, All rights reserved
 */

#include "les/les.h"

#include <cmath>

#include <array>
#include <iostream>

using les::CoefficientMatrix;
using les::ValuesVector;
using AvailableDataset = std::array<satellite::SatelliteData, 3>;

#define DEBUG_PRINT(x) std::cout << #x << " = " << x << std::endl

/**
 * GetAvailableData() - searches for 3 measurement for given satellite from 1 to
 * time_point times.
 * 
 * @return pair of array of 3 consecutive SatelliteData and true in success and
 * false if not.
 */
std::pair<AvailableDataset, bool> GetAvailableDataset(
    const satellite::Satellite& sat, std::size_t time_point) {
  AvailableDataset available_dataset;
  int counter = 0;

  for (std::size_t j = time_point; j > 0 && counter < 3; --j) {
    // If there is data for j time point
    if (sat.time_to_data.count(j)) {
      available_dataset[2 - counter++] = sat.time_to_data.at(j);
    }
  }

  if (counter < 3) {
    return {available_dataset, false};
  }

  return {available_dataset, true};
}

/**
 * ConstructCoefficientsRow() - calculates row of matrix of coefficients.
 * 
 * @return 3-dimensional vector wit coefficients.
 */
Eigen::Vector3d ConstructCoefficientsRow(
    const AvailableDataset& available_dataset) {
  Eigen::Vector3d row(0, 0, 0);
  double d_1_2 = available_dataset[1].distance - available_dataset[0].distance;
  // DEBUG_PRINT(d_1_2);
  double d_2_3 = available_dataset[2].distance - available_dataset[1].distance;
  // DEBUG_PRINT(d_2_3);

  // DEBUG_PRINT(available_dataset[0]);
  // DEBUG_PRINT(available_dataset[1]);
  // DEBUG_PRINT(available_dataset[2]);
  row[0] = (available_dataset[1].x - available_dataset[2].x) / d_2_3 -
           (available_dataset[0].x - available_dataset[1].x) / d_1_2;
  // DEBUG_PRINT(row[0]);

  row[1] = (available_dataset[1].y - available_dataset[2].y) / d_2_3 -
           (available_dataset[0].y - available_dataset[1].y) / d_1_2;
  // DEBUG_PRINT(row[1]);

  row[2] = (available_dataset[1].z - available_dataset[2].z) / d_2_3 -
           (available_dataset[0].z - available_dataset[1].z) / d_1_2;
  // DEBUG_PRINT(row[2]);

  // DEBUG_PRINT(row);
  return row;
}

double ConstructValue(const AvailableDataset& available_dataset) {
  double d_1_2 = available_dataset[1].distance - available_dataset[0].distance;
  double d_2_3 = available_dataset[2].distance - available_dataset[1].distance;

  double k_1 = (std::pow(available_dataset[1].x, 2) -
               std::pow(available_dataset[0].x, 2) +
               std::pow(available_dataset[1].y, 2) -
               std::pow(available_dataset[0].y, 2) +
               std::pow(available_dataset[1].z, 2) -
               std::pow(available_dataset[0].z, 2) -
               std::pow(d_1_2, 2)) /
               (2 * d_1_2);
  // DEBUG_PRINT(k_1);

  double k_2 = (std::pow(available_dataset[2].x, 2) -
               std::pow(available_dataset[1].x, 2) +
               std::pow(available_dataset[2].y, 2) -
               std::pow(available_dataset[1].y, 2) +
               std::pow(available_dataset[2].z, 2) -
               std::pow(available_dataset[1].z, 2) -
               std::pow(d_2_3, 2)) /
               (2 * d_2_3);
  // DEBUG_PRINT(k_2);

  // DEBUG_PRINT(k_1 - k_2 + d_1_2);
  return k_1 - k_2 + d_1_2;
}

std::pair<CoefficientMatrix, ValuesVector> les::ConstructCoefficientsAndValues(
    const satellite::SatelliteArray& satellites, std::size_t time_point) {
  CoefficientMatrix coef_mat;
  ValuesVector values_vec(0);

  // Calculating row for every satellite if possible
  for (std::size_t i = 0; i < satellite::kSatelliteArraySize; ++i) {
    auto [available_dataset, success] = GetAvailableDataset(satellites[i],
                                                            time_point);
    if (!success) {
      continue;
    }

    coef_mat.conservativeResize(coef_mat.rows() + 1, coef_mat.cols());
    coef_mat.row(coef_mat.rows() - 1) = ConstructCoefficientsRow(
      available_dataset);

    values_vec.conservativeResize(values_vec.size() + 1);
    values_vec[values_vec.size() - 1] = ConstructValue(available_dataset);
  }

  return {coef_mat, values_vec};
}

Eigen::Vector3d les::ApplyLeastSquares(const CoefficientMatrix& a,
                                  const ValuesVector& b) {
  // DEBUG_PRINT(a.transpose() * a);
  // DEBUG_PRINT((a.transpose() * a).determinant());
  Eigen::MatrixXd tmp_mat = (a.transpose() * a).inverse();
  // DEBUG_PRINT(tmp_mat);
  return tmp_mat * a.transpose() * b;
  // DEBUG_PRINT(a);
  // DEBUG_PRINT(a.determinant());
  // DEBUG_PRINT(a.inverse() * b);
  // return a.inverse() * b;
}
