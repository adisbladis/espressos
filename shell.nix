let
  pkgs = import <nixpkgs> { };
  inherit (pkgs) lib;

  embeddedproto = pkgs.callPackage
    ({ stdenv
     , python3
     , fetchFromGitHub
     , protobuf
     }:
      let
        pname = "EmbeddedProto";
        version = "3.3.1";

        pythonEnv = python3.withPackages (ps: [
          ps.jinja2
          ps.markupsafe
          ps.protobuf
          ps.six
          ps.toposort
        ]);

      in
      stdenv.mkDerivation {
        inherit pname version;

        src = fetchFromGitHub {
          repo = "EmbeddedProto";
          owner = "Embedded-AMS";
          rev = "ddebda1f03aeee87b0d5d124300ad2c533f23fd9";  # develop
          sha256 = "sha256-Lw3Q9gCNLNLyLfwCgFYl/sQeyAvYYJppI7wgFkKTsCU=";
        };

        nativeBuildInputs = [ pythonEnv protobuf ];

        buildPhase = ''
          runHook preBuild
          protoc -I generator --python_out=generator embedded_proto_options.proto
          runHook postBuild
        '';

        installPhase = ''
          runHook preInstall

          mkdir -p $out/share/EmbeddedProto
          mv generator $out/share/EmbeddedProto/

          mkdir -p $out/bin
          cat > $out/bin/protoc-gen-eams <<EOF
          #!${pkgs.runtimeShell}
          exec ${pythonEnv.interpreter} $out/share/EmbeddedProto/generator/protoc-gen-eams.py --protoc-plugin "$@"
          EOF
          chmod +x $out/bin/protoc-gen-eams

          runHook postInstall
        '';

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
