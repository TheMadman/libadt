let
	pkgs = import <nixpkgs> {};
in
pkgs.callPackage ./build.nix {}
