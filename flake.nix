{
  description = "C development shell with SDL2 support";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    { self
    , nixpkgs
    , flake-utils
    , ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells.default = pkgs.mkShell {
          name = "c-dev-sdl-shell";

          packages = with pkgs; [
            gcc # C compiler
            gnumake # build system
            SDL2 # SDL2 main library
            SDL2_image # image loading
            SDL2_mixer # sound/music
            SDL2_ttf # TrueType font support
            pkg-config # for compiling/linking easily
            bear
          ];

          shellHook = ''
            echo "🧪 Entering C development shell with SDL2 support!"
            echo "📦 SDL2 version: ${pkgs.SDL2.version}"
            export SHELL=$(which zsh)
            exec $SHELL
          '';
        };
      }
    );
}
