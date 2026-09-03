// apps/s4sar.cpp

#include<print>
#include<cmath>
#include<numbers>
#include<complex>
#include<vector>
#include<fstream>

#include<s4sar/geometry.hpp>
#include<s4sar/platform.hpp>
#include<s4sar/scene.hpp>



struct LfmWaveform
{
  double bandwidth;     // [Hz]
  double pulseDuration; // [s]

  [[nodiscard]] 
  double GetChirpRate() const 
  {
    return bandwidth / pulseDuration; 
  }

  [[nodiscard]]
  std::complex<double> GetSample(double fastTime) const 
  {
    if(std::abs(fastTime) > pulseDuration / 2.0)
    {
      return {0.0, 0.0};
    }

    const double phase = std::numbers::pi * GetChirpRate() * std::pow(fastTime, 2); 

    return std::polar(1.0, phase); 
  }

};


int main()
{
  std::println("[Started]: S4SAR"); 

  // =====================================
  // Scene Setup 
  // =====================================

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

  // Sampling Frequency [Hz]
  constexpr double samplingFrequency = 40.0e6; 

  // Number of fast-time samples
  constexpr std::size_t rangeSampleCount = 1024; 

  const double wavelength = speedOfLight / carrierFrequency; 

  const LfmWaveform waveform = 
    {
      .bandwidth = 20.0e6, 
      .pulseDuration = 10.0e-6
    }; 

  std::vector<std::complex<double>> rawData 
    (
      pulseCount * rangeSampleCount 
    );

  // =====================================
  // Calculations 
  // =====================================
  
  // A receive-window start time
  
  const double referenceRange = s4sar::GetSlantRange
    (
      platform.initialPosition, target.position
    );

  const double referenceDelay = 2.0 * referenceRange / speedOfLight; 

  const double fastTimeStart = 
    referenceDelay - static_cast<double>(rangeSampleCount) / 
    (2.0 * samplingFrequency);  
  
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

    const double propagationPhase = -4.0 * std::numbers::pi * range / wavelength; 

    const std::complex<double> propagation = std::exp
      (
        std::complex<double>{0.0, propagationPhase}
      );


    std::size_t activeSampleCount = 0; 
    std::size_t firstActiveSample = rangeSampleCount;
    std::size_t lastActiveSample = 0;

    for(std::size_t r = 0; r < rangeSampleCount; ++r)
    {
      const double fastTime =
        fastTimeStart +
        static_cast<double>(r) / samplingFrequency;

      const double delayedFastTime = fastTime - delay;

      const std::complex<double> echo =
        target.reflectivity *
        waveform.GetSample(delayedFastTime) *
        propagation;

      rawData[p * rangeSampleCount + r] = echo;

      if(std::abs(echo) > 0.0)
      {
        if(activeSampleCount == 0)
        {
          firstActiveSample = r; 
        }

        lastActiveSample = r; 
        ++activeSampleCount; 
      }
    }

    std::println
    (
      "Pulse = {},\n"
      "-> t              = {:.6f}s,\n"
      "-> range          = {:.9f}m,\n"
      "-> delay          = {:.9e}s,\n"
      "-> phase          = {:.6f}rad\n"
      "-> active samples = {}\n"
      "-> active range   = [{}..{}]", 
      p, 
      slowTime, 
      range, 
      delay, 
      propagationPhase, 
      activeSampleCount, 
      firstActiveSample,
      lastActiveSample
    );
  }

  // Exporting pulse to CSV file
  const std::size_t pulseToExport = pulseCount / 2;

  std::ofstream output("raw_pulse.csv");

  output << "sample,fast_time,real,imag,magnitude\n";

  for(std::size_t r = 0; r < rangeSampleCount; ++r)
  {
    const double fastTime =
      fastTimeStart +
      static_cast<double>(r) / samplingFrequency;

    const std::complex<double>& sample =
      rawData[pulseToExport * rangeSampleCount + r];

    output
      << r << ","
      << fastTime << ","
      << sample.real() << ","
      << sample.imag() << ","
      << std::abs(sample) << "\n";
  }

  std::println("[Finished]: S4SAR"); 

  return 0; 
}
