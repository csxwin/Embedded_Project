# invoke SourceDir generated makefile for empty_min.pem4f
empty_min.pem4f: .libraries,empty_min.pem4f
.libraries,empty_min.pem4f: package/cfg/empty_min_pem4f.xdl
	$(MAKE) -f C:\Users\csxwin\workspace_v6.1.2\LAB7CLK/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\csxwin\workspace_v6.1.2\LAB7CLK/src/makefile.libs clean

