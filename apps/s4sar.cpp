// apps/s4sar.cpp

#include<print>
#include<cmath>

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

struct PointTarget
{
  Vec3 position;
  double reflectivity; 
};

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


int main()
{
  std::println("[Started]: S4SAR"); 

  // Scene Setup 

  const Platform platform = 
    {
      .initialPosition = 
        { 
          .x = 0.0, 
          .y = 0.0, 
          .z = 1000.0 
        },

      .velocity =
        { 
          .x = 100.0, 
          .y = 0.0, 
          .z = 0.0 
        },
    };

  const PointTarget target = 
    {
      .position = 
        {
          .x = 0.0, 
          .y = 100.0, 
          .z = 0.0 
        },
      .reflectivity = 1.0  
    };

  // Pulse Repetition Frequency [Hz] : 
  // Determines the slow-time interval between transmitted radar pulses.
  constexpr double prf = 1000.0; 

  // Number of slow-time samples.
  // Number of radar pulses simulated along the platform trajectory
  //
  // Together with PRF, determines the synthetic aperture duration.
  constexpr std::size_t pulseCount = 5;

  // For each sebsequent pulse p, we do:
  for(std::size_t p = 0; p < pulseCount; ++p)
  {

    const double slowTime = 
      ( 
        static_cast<double>(p) - 
        static_cast<double>(pulseCount - 1) / 2.0
      ) / prf ; 

    const Vec3 position = platform.GetPositionAt(slowTime); 
    const double range = GetSlantRange(position, target.position);

    std::println
    (
      "Pulse = {}, t = {:.6f}s, x = {:.3f}m, range = {:.9f}m", 
      p, 
      slowTime, 
      position.x, 
      range 
    );
  }

  std::println("[Finished]: S4SAR"); 

  return 0; 
}
