let
	nixpkgs = fetchTarball "https://github.com/NixOS/nixpkgs/archive/refs/tags/25.05.tar.gz";
	pkgs = import nixpkgs {};
in
pkgs.callPackage ./build.nix {}
