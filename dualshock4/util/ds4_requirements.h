/**
 * This file checks if other components are setup correctly for ds4.
 */
#ifndef DS4_REQUIREMENTS_H
#define DS4_REQUIREMENTS_H

#include "sdkconfig.h"

#ifndef CONFIG_BLUEPAD32_PLATFORM_CUSTOM
#error "DS4 library requires Bluepad32 platform set to CUSTOM"
#endif

#ifdef CONFIG_BLUEPAD32_GAP_SECURITY
#error "DS4 library requires Bluepad32 GAP security to be turned off"
#endif

#ifndef CONFIG_BT_ENABLED
#error "DS4 library requires Bluetooth to be enabled"
#endif

#ifndef CONFIG_BT_BLUEDROID_ENABLED
#error "DS4 library requires Bluedroid to be enabled"
#endif

#ifndef CONFIG_BLUEDROID_PINNED_TO_CORE_0
#error "DS4 library requires Bluedroid pinned to Core 0"
#endif

#ifndef CONFIG_BT_CLASSIC_ENABLED
#error "DS4 library requires Bluetooth Classic support to be enabled"
#endif

#ifndef CONFIG_BT_CONTROLLER_ENABLED
#error "DS4 library requires Bluetooth Controller to be enabled"
#endif

#ifndef CONFIG_DS4_ENABLE
#error "Wrong config settings for DS4"
#endif

#ifdef CONFIG_BLUEPAD32_MAX_DEVICES
#if CONFIG_BLUEPAD32_MAX_DEVICES < 1
#error "DS4 library can't have less than 1 max Bluepad32 devices"
#endif
#if CONFIG_BLUEPAD32_MAX_DEVICES != 1
#warning "DS4 library uses only 1 device, it is recommended to put Blupad32 max devices to 1"
#endif
#endif

#ifdef CONFIG_BLUEPAD32_MAX_ALLOWLIST
#if CONFIG_BLUEPAD32_MAX_ALLOWLIST < 1
#error "DS4 library can't have less than 1 max allowlist Bluepad32 devices"
#endif
#if CONFIG_BLUEPAD32_MAX_ALLOWLIST != 1
#warning "DS4 library uses only 1 device, it is recommended to put Blupad32 max devices to 1"
#endif
#endif

#endif // DS4_REQUIREMENTS_H
