// apps/s4sar.cpp

#include<print>
#include<cmath>
#include<numbers>

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

  // Light Speed [m/s]
  constexpr double speedOfLight = 299'792'458.0;

  // Sensor Frequency [Hz]
  constexpr double carrierFrequency = 5.405e9;

  const double wavelength = speedOfLight / carrierFrequency; 


  // For each subsequent pulse p, we do:
  for(std::size_t p = 0; p < pulseCount; ++p)
  {

    const double slowTime = 
      ( 
        static_cast<double>(p) - 
        static_cast<double>(pulseCount - 1) / 2.0
      ) / prf ; 

    const Vec3 position = platform.GetPositionAt(slowTime); 
    const double range = GetSlantRange(position, target.position);

    // Two-Way Propagation Delay 
    //
    // [Note]: factor 2 accounts for a round-trip: radar -> target -> radar 
    const double delay = 2.0 * range / speedOfLight; 

    const double phase = -4.0 * std::numbers::pi * range / wavelength; 

    std::println
    (
      "Pulse = {}, t = {:.6f}s, range = {:.9f}m, delay = {:.9e}s, phase = {:.6f}rad", 
      p, 
      slowTime, 
      range, 
      delay, 
      phase
    );
  }

  std::println("[Finished]: S4SAR"); 

  return 0; 
}
