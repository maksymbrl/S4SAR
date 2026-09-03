# flake.nix
#
{
  description = "Simple Stripmap Signal Simulator for Synthetic Aperture Radar";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";

    flake-parts = {
      url = "github:hercules-ci/flake-parts";
      inputs.nixpkgs-lib.follows = "nixpkgs";
    };
  };

  outputs = inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; }
    {
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      perSystem = { pkgs, ... }:
        let
          cppSupport = import ./nix/cplusplus.nix {
            inherit pkgs;
          };
        in
        {
          # `nix develop`
          devShells.default = pkgs.mkShell {
            packages = with pkgs; [
              git
              cacert
            ] ++ cppSupport.packages;

            env = {
              # Valid CA bundle for tools used interactively in the
              # development shell.
              SSL_CERT_FILE =
                "${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt";

              NIX_SSL_CERT_FILE =
                "${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt";
            } // cppSupport.env;

            shellHook = ''
              export REPO_ROOT="$(
                git rev-parse --show-toplevel 2>/dev/null || pwd
              )"

              ${cppSupport.shellHook}

              echo "SARender development environment"
              echo "Repository: $REPO_ROOT"
            '';
          };
        };
    };
}
