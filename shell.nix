let
  pkgs = import <nixpkgs> { };
  inherit (pkgs) lib;

  embeddedproto = pkgs.python3.pkgs.callPackage
    ({ stdenv
     , buildPythonApplication
     , fetchFromGitHub
     , jinja2
     , markupsafe
     , six
     , toposort
     , protobuf
     , pip
     }:
      let
        pname = "EmbeddedProto";
        version = "3.3.1";

      in
      buildPythonApplication {
        inherit pname version;

        src = fetchFromGitHub {
          repo = "EmbeddedProto";
          owner = "adisbladis";
          rev = "setuptools";
          hash = "sha256-ros5pFMqzj5/Bza+ZyZiNW1GvF0ze1+RK31j6RnL050=";
        };

        sourceRoot = "source/generator";

        postPatch = ''
          substituteInPlace setup.py --replace "==" ">="
        '';

        nativeBuildInputs = [
          pip
        ];

        propagatedBuildInputs = [
          jinja2
          markupsafe
          protobuf
          six
          toposort
        ];
      })
    { };


  pythonEnv = pkgs.python3.withPackages (ps: [
    ps.brotli
    ps.python-magic
    ps.ipython
  ]);

  android-nixpkgs = pkgs.callPackage
    (import (builtins.fetchGit {
      url = "https://github.com/tadfisher/android-nixpkgs.git";
    }))
    { channel = "stable"; };

  androidSDK = android-nixpkgs.sdk (sdkPkgs: with sdkPkgs; [
    cmdline-tools-latest
    build-tools-30-0-3
    build-tools-34-0-0
    platform-tools
    platforms-android-33
    emulator
    patcher-v4
    tools
  ]);


in
pkgs.mkShell {
  packages = [
    pkgs.platformio
    pkgs.nodejs
    pkgs.doctest # C++ tests
    pkgs.reuse # License management

    pkgs.treefmt
    pkgs.clang-tools # clang-format
    pkgs.nixpkgs-fmt

    androidSDK

    pythonEnv

    pkgs.protobuf
    embeddedproto
  ];

  # Android workarounds
  JAVA_HOME = "${pkgs.openjdk.home}";
  GRADLE_OPTS = "-Dorg.gradle.project.android.aapt2FromMavenOverride=${androidSDK}/share/android-sdk/build-tools/34.0.0/aapt2";

  shellHook = ''
    export PATH=${builtins.toString ./pkgs/espressos-web/node_modules/.bin}:"$PATH"
  '';
}
