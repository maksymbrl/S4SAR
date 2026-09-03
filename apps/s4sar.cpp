// apps/s4sar.cpp

#include<print>
#include<cmath>
#include<numbers>

#include<s4sar/geometry.hpp>
#include<s4sar/platform.hpp>
#include<s4sar/scene.hpp>


int main()
{
  std::println("[Started]: S4SAR"); 

  // Scene Setup 

  const s4sar::Platform platform = 
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

  const s4sar::PointTarget target = 
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

    const s4sar::Vec3 position = platform.GetPositionAt(slowTime); 
    const double range = s4sar::GetSlantRange(position, target.position);

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
