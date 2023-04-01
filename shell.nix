let
  pkgs = import <nixpkgs> { };
in
pkgs.mkShell {
  packages = [
    pkgs.platformio
    pkgs.clang-tools  # clang-format
  ];
}
