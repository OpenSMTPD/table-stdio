{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [ autoreconfHook pkg-config ];

  buildInputs = with pkgs; [ libbsd ];
}
