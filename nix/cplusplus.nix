# nix/cplusplus.nix
#
{
  pkgs,
}:

let
  lib = pkgs.lib;

  # Tools required to configure and build C++ projects.
  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    pkg-config
  ];

  # Test dependencies.
  checkInputs = with pkgs; [
    catch2_3
  ];

  # Development-only tooling.
  #
  # We deliberately do not add a second C++ compiler here.
  # The compiler is provided by Nix's stdenv:
  #
  #   Linux  -> GCC toolchain
  #   Darwin -> Clang toolchain
  #
  # clang-tools gives us clangd, clang-format and clang-tidy without
  # replacing the compiler used by stdenv.
  developmentTools = [
    pkgs.llvmPackages.clang-tools
  ]
  ++ lib.optionals pkgs.stdenv.isLinux [
    pkgs.gdb
    pkgs.valgrind
  ];

  packages =
    nativeBuildInputs
    ++ checkInputs
    ++ developmentTools;

  env = {
    # Use Ninja consistently for interactive CMake builds.
    CMAKE_GENERATOR = "Ninja";

    # Required by clangd and useful for clang-tidy and other tooling.
    CMAKE_EXPORT_COMPILE_COMMANDS = "ON";
  };

  shellHook = ''
    echo "C++: $(c++ --version | head -n 1)"
    echo "CMake: $(cmake --version | head -n 1)"
    echo "Ninja: $(ninja --version)"
    echo "clangd: $(clangd --version | head -n 1)"
  '';
in
{
  inherit
    nativeBuildInputs
    checkInputs
    developmentTools
    packages
    env
    shellHook
    ;
}

