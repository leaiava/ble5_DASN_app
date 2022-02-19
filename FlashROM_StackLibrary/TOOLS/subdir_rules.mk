################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
build-581820703:
	@$(MAKE) --no-print-directory -Onone -f TOOLS/subdir_rules.mk build-581820703-inproc

build-581820703-inproc: ../TOOLS/app_ble.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/home/leandro/ti/xdctools_3_51_03_28_core/xs" --xdcpath="/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source;/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/kernel/tirtos/packages;/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M3 -p ti.platforms.simplelink:CC2640R2F -r release -c "/home/leandro/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS" --compileOptions "-mv7M4 --code_state=16 -me -O2 --opt_for_speed=0 --include_path=\"/home/leandro/ti/workspace_v10/ble5_DASN_app\" --include_path=\"/home/leandro/ti/workspace_v10/ble5_DASN_app/Application\" --include_path=\"/home/leandro/ti/workspace_v10/ble5_DASN_app/Startup\" --include_path=\"/home/leandro/ti/workspace_v10/ble5_DASN_app/PROFILES\" --include_path=\"/home/leandro/ti/workspace_v10/ble5_DASN_app/Include\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/examples/rtos/CC2640R2_LAUNCHXL/ble5stack/project_zero/src/extra\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/controller/cc26xx/inc\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/inc\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/rom\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/common/cc26xx\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/icall/inc\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/target\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/hal/src/target/_common\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/hal/src/target/_common/cc26xx\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/hal/src/inc\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/heapmgr\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/icall/src/inc\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/osal/src/inc\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/services/src/saddr\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/ble5stack/services/src/sdata\" --include_path=\"/home/leandro/ti/-simplelink_cc2640r2_sdk_5_10_00_02/source/ti/devices/cc26x0r2\" --include_path=\"/home/leandro/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include\" --define=DeviceFamily_CC26X0R2 --define=uartlog_FILE=\"\"\"\" --define=UARTLOG_ENABLE -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-581820703 ../TOOLS/app_ble.cfg
configPkg/compiler.opt: build-581820703
configPkg/: build-581820703


