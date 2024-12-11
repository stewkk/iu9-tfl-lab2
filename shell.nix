{ pkgs ? import <nixpkgs> {} }:

let
  pythonWithTk = pkgs.python310.buildEnv.override {
    extraLibs = with pkgs.python310Packages; [
      tkinter
      pip
      virtualenv
    ];
    ignoreCollisions = true;
  };
in
pkgs.mkShell.override {stdenv = pkgs.llvmPackages_18.stdenv;} {
  buildInputs = [
    pythonWithTk
    pkgs.tk
    pkgs.tcl
    pkgs.nodePackages.pyright
    pkgs.plantuml
    pkgs.pre-commit
    pkgs.cmake
    # pkgs.clang_18

    # keep this line if you use bash
    pkgs.bashInteractive
  ];

  nativeBuildInputs = [
    pkgs.clang-tools_18
  ];

  NIX_LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
    pkgs.stdenv.cc.cc
    pkgs.zlib
  ];
  NIX_LD = pkgs.lib.fileContents "${pkgs.stdenv.cc}/nix-support/dynamic-linker";
  shellHook = ''
    export VENV_DIR="$PWD/.venv"
    if [ ! -d "$VENV_DIR" ]; then
      ${pythonWithTk}/bin/python -m venv $VENV_DIR
      source $VENV_DIR/bin/activate
      pip install pip setuptools wheel
    else
      source $VENV_DIR/bin/activate
    fi

    export LD_LIBRARY_PATH=$NIX_LD_LIBRARY_PATH
    export PYTHONPATH="${pythonWithTk}/lib/python3.10/site-packages:$PYTHONPATH"
    export TCL_LIBRARY="${pkgs.tcl}/lib/tcl${pkgs.tcl.version}"
    export TK_LIBRARY="${pkgs.tk}/lib/tk${pkgs.tk.version}"

    export MPLBACKEND=TkAgg

    if [ -f mat/advanced12iq/requirements.txt ]; then
      pip install -r mat/advanced12iq/requirements.txt
    fi

    python --version
  '';
}
