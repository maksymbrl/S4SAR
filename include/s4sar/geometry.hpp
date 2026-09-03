// include/s4sar/geometry.hpp

#pragma once

#include<cmath>

namespace s4sar 
{
  /**
   * Coordinate system:
   *   x: along-track
   *   y: cross-track
   *   z: altitude
   */
  struct Vec3
  {
    double x; 
    double y;
    double z; 
  };

  /** 
  * Calculate Instantaneous Slant Range as simple Eucledian distance
  */
  [[nodiscard]]
  double GetSlantRange(const Vec3& a, const Vec3& b)
  {
    return std::hypot
      (
        b.x - a.x, 
        b.y - a.y, 
        b.z - a.z
      ); 
  }

}
