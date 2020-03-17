/**
 * Copyright 2020, Daniil Polyakov, All rights reserved
 */

#pragma once

#include <utility>

#include "satellite/satellite.h"
#include "3rdparty/eigen/Eigen/Dense"

/**
 * les - Linear Equation System. Here are declared some functions, which helps
 * to construct required data for system.
 */
namespace les {

using CoefficientMatrix = Eigen::Matrix<double, Eigen::Dynamic, 3>;

using ValuesVector = Eigen::VectorXd;

/**
 * ConstructCoefficientsAndValues() - builds matrix with 3 columns and
 * x rows, depends on relevant data existence.
 * 
 * @param satellites - array of satellites data from which coefficients will be consturcted.
 * 
 * @param time_point - to which time to examine data. Function will look from time_point - 3 to time_point.
 * 
 * @return pair of matrix of coefficients and vector of values of les.
 */
std::pair<CoefficientMatrix, ValuesVector> ConstructCoefficientsAndValues(
    const satellite::SatelliteArray& satellites, std::size_t time_point);

/**
 * ApplyLeastSquares() - applies Least Squares Method and returns the result.
 * 
 * @param a - matrix of coefficients of linear system.
 * 
 * @param b - vector of values of linear system.
 * 
 * @return 3-dimensional vector with result of LSM.
 */
Eigen::Vector3d ApplyLeastSquares(const CoefficientMatrix& a,
                                  const ValuesVector& b);

}  // namespace les
