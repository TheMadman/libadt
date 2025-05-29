let
	pkgs = import <nixpkgs> {};
in
pkgs.mkShell {
	inputsFrom = [(import ./.)];
}
