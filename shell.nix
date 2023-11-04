let
  pkgs = import <nixpkgs> { };

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

    pythonEnv

    pkgs.protobuf
    embeddedproto
  ];

  shellHook = ''
    export PATH=${builtins.toString ./pkgs/espressos-web/node_modules/.bin}:"$PATH"
  '';
}
