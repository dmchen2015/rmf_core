/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef SRC__RMF_TRAFFIC__MOTIONINTERNAL_HPP
#define SRC__RMF_TRAFFIC__MOTIONINTERNAL_HPP

#include <rmf_traffic/Motion.hpp>

#include "Spline.hpp"

namespace rmf_traffic {

//==============================================================================
class SinglePointMotion : public Motion
{
public:

  SinglePointMotion(
      const Time t,
      Eigen::Vector3d p,
      Eigen::Vector3d v);

  Time start_time() const override;
  Time finish_time() const override;
  Eigen::Vector3d compute_position(Time t) const override;
  Eigen::Vector3d compute_velocity(Time t) const override;
  Eigen::Vector3d compute_acceleration(Time t) const override;

private:

  Time _t;
  Eigen::Vector3d _p;
  Eigen::Vector3d _v;

};

//==============================================================================
class SplineMotion : public Motion
{
public:

  SplineMotion(Spline spline);

  Time start_time() const override;
  Time finish_time() const override;
  Eigen::Vector3d compute_position(Time t) const override;
  Eigen::Vector3d compute_velocity(Time t) const override;
  Eigen::Vector3d compute_acceleration(Time t) const override;

private:

  Spline _spline;

};

} // namespace rmf_traffic

#endif // SRC__RMF_TRAFFIC__MOTIONINTERNAL_HPP
