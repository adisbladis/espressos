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
          owner = "adisbladis";
          rev = "system-libs";
          sha256 = "sha256-yNL0dBlsIJLvMVFYDKBmxVdZDjfezm/kge7r81y19Tg=";
        };

        nativeBuildInputs = [ pythonEnv protobuf ];

        buildPhase = ''
          runHook preBuild
          python setup.py --system-libs

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

in
pkgs.mkShell {
  packages = [
    pkgs.platformio
    pkgs.clang-tools # clang-format
    pkgs.nodejs

    pkgs.protobuf
    embeddedproto
  ];

  shellHook = ''
    export PATH=${builtins.toString ./web/node_modules/.bin}:"$PATH"
  '';
}
