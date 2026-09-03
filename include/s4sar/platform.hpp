// include/s4sar/platform.hpp 

#pragma once

namespace s4sar 
{

  struct Platform 
  {
    Vec3 initialPosition; 
    Vec3 velocity;

    /** 
    * Calculating the satellite's position at time t using simple kinematics
    */
    [[nodiscard]]
    Vec3 GetPositionAt(double time) const
    {
      return 
      {
        initialPosition.x + velocity.x * time, 
        initialPosition.y + velocity.y * time, 
        initialPosition.z + velocity.z * time, 
      };
    } 
  };

}
