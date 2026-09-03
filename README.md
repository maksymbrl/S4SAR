# S4SAR

> **Simple Stripmap Signal Simulator for Synthetic Aperture Radar** (S4SAR) is
> a modern C++ implementation of a simple stripmap Synthetic Aperture Radar
> (SAR) signal simulator. It models the fundamental SAR acquisition process,
> including platform motion, LFM waveform generation, propagation delay,
> coherent echo synthesis from point scatterers, noise, and SAR image
> formation.

## Scope

S4SAR is not intended to be a general-purpose or high-fidelity electromagnetic
SAR simulator. The implementation focuses on a deliberately restricted stripmap
configuration in order to keep the underlying physical model transparent and
the software architecture manageable.

## Features

Planned and implemented functionality includes:

- stripmap SAR acquisition geometry;
- constant-velocity platform trajectories;
- linear frequency-modulated (LFM) pulse generation;
- point-scatterer scene representation;
- propagation delay and phase calculation;
- coherent raw echo synthesis;
- complex I/Q raw-data generation;
- additive noise models;
- range compression;
- Range–Doppler SAR focusing;
- numerical validation and automated tests.

## Background

The physical model implemented by S4SAR is based on established stripmap SAR
theory and is particularly inspired by the methodology presented in the
SARrawSim publication:

> A. J. Zozaya Sahad, “SARrawSim: Synthetic Aperture Radar Raw Data Simulator,” 
> SoftwareX, vol. 29, art. 102019, 2025. 
> doi: 10.1016/j.softx.2024.102019

S4SAR is an independent C++ implementation of the underlying physical and
mathematical model and is not a source-code translation of the original
SARrawSim implementation.
