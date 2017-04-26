win:
	make -f makefile.win

nix:
	make -f makefile.nix

clean:
	make -f makefile.win clean
	make -f makefile.nix clean
