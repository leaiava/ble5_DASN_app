# invoke SourceDir generated makefile for app_ble.pem3
app_ble.pem3: .libraries,app_ble.pem3
.libraries,app_ble.pem3: package/cfg/app_ble_pem3.xdl
	$(MAKE) -f C:\Users\Lea\workspace_v10\ble5_DASN_app\TOOLS/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Lea\workspace_v10\ble5_DASN_app\TOOLS/src/makefile.libs clean

