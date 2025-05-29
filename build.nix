{
	stdenv,
	cmake,
}:

stdenv.mkDerivation {
	pname = "libadt";
	version = "0.0.1";
	src = ./.;
	nativeBuildInputs = [cmake];
	cmakeFlags = [
		"-DBUILD_TESTING=True"
	];

	doCheck = true;
	checkTarget = "test";
}
