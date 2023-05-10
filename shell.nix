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

        patches = [
          (pkgs.fetchpatch {
            name = "header-include-paths.patch";
            url = "https://github.com/Embedded-AMS/EmbeddedProto/commit/c50c17d83493652355fd130734939b5e73509d28.patch";
            sha256 = "sha256-6WqofVNGeQgaCqVEQ01v20KlqMKl8Trn8yGm0roqviM=";
          })

          (pkgs.fetchpatch {
            name = "header-no-copyright-header.patch";
            url = "https://github.com/Embedded-AMS/EmbeddedProto/commit/968baf0e893924d7cc6369cab0344b44a3325a92.patch";
            sha256 = "sha256-GkRH7jT27l/g2ZFiDurhfRQ2ZJO/DlcmfXA7CMGMaoY=";
          })
        ];

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


  pythonEnv = pkgs.python3.withPackages(ps: [
    ps.brotli
    ps.python-magic
    ps.ipython
  ]);

in
pkgs.mkShell {
  packages = [
    pkgs.platformio
    pkgs.nodejs
    pkgs.doctest

    pkgs.treefmt
    pkgs.clang-tools # clang-format

    pythonEnv

    pkgs.protobuf
    embeddedproto
  ];

  shellHook = ''
    export PATH=${builtins.toString ./pkgs/espressos-web/node_modules/.bin}:"$PATH"
  '';
}
