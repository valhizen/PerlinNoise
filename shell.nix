{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc14
    cmake
    ninja
    gdb
bear
pkg-config
    clang-tools  # Keep this for clangd LSP
    SDL2
SDL2_ttf
  ];

  shellHook = ''
    echo "C++ Development Environment"
    echo "Compiler: $(g++ --version | head -n1)"

    alias b=' bear -- ./build.sh'
    alias r='./build/main'
  '';
}
