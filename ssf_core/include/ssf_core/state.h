/*
 * state.h
 *
 *  Created on: Jun 20, 2012
 *      Author: acmarkus
 */

#ifndef STATE_H_
#define STATE_H_

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <vector>
#include <ssf_core/eigen_conversions.h>
#include <sensor_fusion_comm/ExtState.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#define N_STATE 25 /// error state size

namespace ssf_core
{
/**
 * This class defines the state, its associated error state covarinace and the
 * system inputs. The values in the braces determine the state's position in the
 * state vector / error state vector.
 */
class State
{
public:
  // states varying during propagation
  Eigen::Matrix<double, 3, 1> p_;         ///< position (IMU centered)          (0-2 / 0-2)
  Eigen::Matrix<double, 3, 1> v_;         ///< velocity                         (3- 5 / 3- 5)
  Eigen::Quaternion<double> q_;           ///< attitude                         (6- 9 / 6- 8)
  Eigen::Matrix<double, 3, 1> b_w_;       ///< gyro biases                      (10-12 / 9-11)
  Eigen::Matrix<double, 3, 1> b_a_;       ///< acceleration biases              (13-15 / 12-14)

  // states not varying during propagation
  double L_;                              ///< visual scale                     (16 / 15)
  Eigen::Quaternion<double> q_wv_;        ///< vision-world attitude drift      (17-20 / 16-18)
  Eigen::Quaternion<double> q_ci_;        ///< camera-imu attitude calibration  (21-24 / 19-21)
  Eigen::Matrix<double, 3, 1> p_ic_;      ///< camera-imu position calibration  (25-27 / 22-24)

  // system inputs
  Eigen::Matrix<double,3,1> w_m_;         ///< angular velocity from IMU
  Eigen::Matrix<double,3,1> a_m_;         ///< acceleration from IMU

  Eigen::Quaternion<double> q_int_;       ///< this is the integrated ang. vel. no corrections applied, to use for delta rot in external algos...

  Eigen::Matrix<double, N_STATE, N_STATE> P_;///< error state covariance

  double time_; ///< time of this state estimate

  /// resets the state
  /**
   * 3D vectors: 0; quaternion: unit quaternion; scale: 1; time:0; Error covariance: zeros
   */
  void reset();

  /// writes the covariance corresponding to position and attitude to cov
  void getPoseCovariance(geometry_msgs::PoseWithCovariance::_covariance_type & cov);

  /// assembles a PoseWithCovarianceStamped message from the state
  /** it does not set the header */
  void getPoseMsg(geometry_msgs::PoseWithCovarianceStamped & pose);

  /// assembles an ExtState message from the state
  /** it does not set the header */
  void getStateMsg(sensor_fusion_comm::ExtState & state);



};

}

#endif /* STATE_H_ */
