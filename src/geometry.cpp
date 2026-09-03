// src/geometry.hpp

#include<cmath>

#include<s4sar/geometry.hpp>

namespace s4sar 
{

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

