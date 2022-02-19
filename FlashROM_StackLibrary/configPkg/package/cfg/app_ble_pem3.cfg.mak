# invoke SourceDir generated makefile for app_ble.pem3
app_ble.pem3: .libraries,app_ble.pem3
.libraries,app_ble.pem3: package/cfg/app_ble_pem3.xdl
	$(MAKE) -f /home/leandro/ti/workspace_v10/ble5_DASN_app/TOOLS/src/makefile.libs

clean::
	$(MAKE) -f /home/leandro/ti/workspace_v10/ble5_DASN_app/TOOLS/src/makefile.libs clean

