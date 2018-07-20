/**
 * @file
 * @date May 2016
 * @author Andrew Spielvogel (andrewspielvogel@gmail.com)
 * @brief Helper functions
 *
 */


#ifndef HELPER_FUNCS_H
#define HELPER_FUNCS_H

#include <math.h>
#include <stdio.h>
#include <Eigen/Core>
#include <unsupported/Eigen/MatrixFunctions>



/**
 * @brief Skew function.
 * 
 * Skew operator on \f${\rm I\!R}^3\f$.
 * @param w Input vector.
 */
extern Eigen::Matrix3d skew(Eigen::Vector3d w);

/**
 * @brief Unskew function.
 *
 * @param w_hat Input so(3) matrix.
 */
extern Eigen::Vector3d unskew(Eigen::Matrix3d w_hat);

/**
 * @brief Rotation around x-axis.
 * @param x angle (units: radians).
 */
extern Eigen::Matrix3d Rx(double x);

/**
 * @brief Rotation around y-axis.
 * @param y angle (units: radians).
 */
extern Eigen::Matrix3d Ry(double y);

/**
 * @brief Rotation around z-axis.
 * @param z angle (units: radians).
 */
extern Eigen::Matrix3d Rz(double z);

/**
 * @brief Rotation to roll, pitch, heading euler angles.
 * @param R Input rotation.
 */
extern Eigen::Vector3d rot2rph(Eigen::Matrix3d R);

/**
 * @brief Roll, pitch, yaw euler angles to rotation.
 * @param rpy Roll, pitch, yaw vector (units: radians).
 */

extern Eigen::Matrix3d rpy2rot(Eigen::Vector3d rpy);

/**
 * @brief Earth to NED rotation.
 * @param lat Latitude of instrument.
 */
extern Eigen::Matrix3d rot_earth2ned(float lat);

/**
 * @brief Returns Star to Earth rotation
 * @param t Time (seconds)
 */
extern Eigen::Matrix3d get_R_se(float t);

/**
 * @brief Returns Earth to NED rotation
 * @param lat Latitude (radians)
 */
extern Eigen::Matrix3d get_R_en(float lat);

/**
 * @brief Returns Star to NED rotation
 * @param lat Latitude (radians)
 * @param t Time (seconds)
 */
extern Eigen::Matrix3d get_R_sn(float lat, float t);

/**
 *
 * @brief Function for converting string to 3x3 diagonal matrix.
 *
 * @param str Comma seperated list of three diag elements. e.g. "1,1,1"
 *
 */
extern Eigen::Matrix3d stringToDiag(std::string str);

/**
 *
 * @brief IMU packet
 *
 */
struct ImuPacket
{

  Eigen::Vector3d ang; /**< Angular-rate measurement. */
  Eigen::Vector3d acc; /**< Linear-acceleration measurement. */
  Eigen::Vector3d mag; /**< Magnetometer measurement. */

  int seq_num; /**< Measurement sequence number. */

  double t; /**< Measurement timestamp. */

  float fluid_pressure; /**< Fluid Pressure. */
  double dt;

};


/**
 *
 * @brief Struct for storing estimator parameters.
 * 
 */
struct config_params {

  int hz;
  float lat;
  std::string o_file;
  std::string i_file;
  std::string last_mod;
  Eigen::Matrix3d K_acc;
  Eigen::Matrix3d K_mag;
  Eigen::Matrix3d K_ang_bias;
  Eigen::Matrix3d K_acc_bias;
  Eigen::Matrix3d K_mag_bias;
  Eigen::Matrix3d K_g;
  Eigen::Matrix3d K_north;
  Eigen::Matrix3d R0;
  Eigen::Matrix3d R_align;

  Eigen::VectorXd k;


  std::string frameId;
  
};

#endif
