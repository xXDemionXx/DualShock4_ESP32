# DualShock4_ESP32 library

A lightweight DualShock4 interface library for ESP32 focused on minimal setup and thread-safe polling/event APIs.

![Platform](https://img.shields.io/badge/platform-ESP32-orange)
![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.3-green)

**Features:**

- 🎮 DualShock4 input polling
- 🔔 Button event callbacks
- 🌈 RGB lightbar control
- 📳 Rumble support
- 🔒 MAC filtering

>[!NOTE]  
> This library only supports one controller connection.  
> DualShock4 touchpad input is currently unsupported.

## Compatibility

- Works only with the original ESP32!
- ESP-IDF v5.3.4 (higher versions might/might not work)
- DualShock4 v1/v2

## Installation

Just clone into your ESP-IDF projects `components/` folder and extract the two component from the repo.  
If you are in the root of your project you can just use this script:

```bash
cd components
git clone https://github.com/xXDemionXx/DualShock4_ESP32 tmp_repo
mv tmp_repo/dualshock4 .
mv tmp_repo/btstack .
rm -rf tmp_repo
```

>[!NOTE]  
> When cloned, this library comes bundled with a btstack component which is a port needed for Bluepad32 component and the library itself.

## Quick Start

1. Install the library (the section above this one)
2. Enable DS4 support in sdkconfig
3. Configure Bluepad32 settings in sdkconfig:
    - Target platform -> Custom
    - Enable GAP security -> OFF
4. Call `ds4Init()` in `app_main()`
5. Flash to ESP32
6. Read the Btstack MAC in the logs
7. Set the MAC address on the controller to be the same as your ESP's Bluetooth ([see the controller tools in basic setup](#controller-tools))
8. Turn on the controller with the PS button

## How to use the library

The API functions of this library are thread safe, and you can call them from anywhere. The only requirement is that before you call any API function, you must first initialize the library with `ds4Init()`. We recommend calling this function in `app_main()` at the start of your program.

After that you can freely call any other `ds4` function. You can see how to use all of the features in the `dualshock4/examples/` folder. Here is a minimal example for the basic features:

```c
#include "freertos/FreeRTOS.h"
#include "ds4.h"

ds4_data_t data = {};   // Struct that will receive controller data

void app_main(void)
{

    ds4Init(); // Initialize the library
    ds4SetPollingStruct(&data);

#ifdef CONFIG_DS4_MODE_EVENT    // Only if you have enabled events in  sdkconfig
    void print_on_event(void *param);
    // Add an event to the triangle button press
    ds4SetButtonEvent(DS4_BTN_TRIANGLE, DS4_BTN_EVENT_PRESS, &print_on_event, "Triangle button pressed");
#endif

    while (1)
    {
        // Controller must be connected and ready
        if (ds4GetConnectionStatus() == DS4_READY)
        {
            if (data.buttons.circle) // If pressed
            {
                printf("Circle pressed\n");
                ds4SetLightbar(255, 0, 0);  // Change color
                ds4PlayRumble(255, 100, 0); // Play Rumble
            }
            else
            {
                ds4SetLightbar(0, 255, 0);  // Change color
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#ifdef CONFIG_DS4_MODE_EVENT
void print_on_event(void *param)
{
    printf("%s\n", (const char *)param);
    ds4SetLightbar(100, 0, 100);
}
#endif
```

## Configuration

This library can work in multiple different ways. It is required for the user to set up the wanted way and the wanted features in sdkconfig, as well as the universal settings that are needed for this library to even compile.

### The Basic setup

Before you can use the API in your project, the library requires you to configure its basic behavior in `sdkconfig`, these are the steps:

> 1. In **DualShock4** turn on **Enable DS4**.
> 2. In **Bluepad32** set:
>     - Maximum of connected gamepads -> 1
>     - Target platform -> custom
>     - Maximum size of allowlist -> 1

This setup will allow you to connect any *DualShock4* controller to your *ESP32* but you must change the stored MAC address on the controller to be the same as the default MAC address of the ESP's Bluetooth stack. You can see the MAC of the ESP if you have the console enabled in *Bluepad32* or you can use the `ds4GetUserAddress()` API function. To change the controller's stored MAC, [see the heading bellow this one](#controller-tools).

If you did everything correctly, you will be able to compile the basic example and flash it to your ESP. To connect, you only need to turn on the controller with the ***PS button*** and the controller should automatically connect after a few seconds.  
*(if you have the Bluepad32 console turned on, you will be able to see the printout of how the connecting is happening)*

### Controller tools

To change the controller's stored connection MAC (or to just see what is the MAC of the controller), we recommend these tools:

> - **Windows** -> [SixAxisPairTool](https://sixaxispairtool.en.lo4d.com/windows)
> - **Linux** -> You are on your own
> - **MAC** -> You are on your own

---

#### Changing the ESP's MAC

If you don't want to change the MAC stored on your controller, you can find out what MAC is stored on the controller and make it so your *ESP* uses that MAC for its Bluetooth stack.  

> To do that in `sdkconfig` in **DualShock4** set:
>
> 1. Bluetooth MAC Address Mode -> Use custom Bluetooth MAC
> 2. Input your wanted MAC into the field that poped up bellow

#### Allow only a specific controller to connect

If you don't want any controller to be able to connect but only a specific one. You need to find out what the MAC address of the DualShock4 controller is (you can see that in the printed logs after a controller connects). After you know the controller's address, you need to change the connect mode of this library.

> To do that in `sdkconfig` in **DualShock4** set:
>
> 1. Controller connect mode -> Connect only to a specific MAC address of the controller
> 2. Input your wanted MAC into the field that poped up bellow

Before you connect for the first time, you will have to put the *DualShock4* controller into pairing mode. You do that by holding the *share* and *PS button* for five seconds while the controller is turned off. After the controller starts doing two rapid blinks repeatedly, you can turn off the controller and turn it back on normally.

---

### Event mode

The event mode allows gives the user access to the `ds4SetButtonEvent()` function. This function allows the user to register functions to be done on a button event in a separate task that handles button events. But if you plan on just using polling and not using this function, then you can choose ***DualShock4 operation mode -> Just polling,*** and you will not have an extra memory overhead of an extra task.

## API Overview

| Function | Description |
| ---------- | ------------- |
| `ds4Init()` | Initialize library |
| `ds4SetPollingStruct()` | Sets the struct that will receive controller data |
| `ds4SetButtonEvent()` | Set an event callback on a button event |
| `ds4GetConnectionStatus()` | Get the connection status of the controller |
| `ds4GetUserAddress()` | Get the MAC address of the Bluetooth stack |
| `ds4Disconnect()` | Disconnect the controller |
| `ds4SetLightbar()` | Set RGB lightbar |
| `ds4PlayRumble()` | Play rumble on both motors |
| `ds4PlayRumbleWeak()` | Play rumble on the weak motors |
| `ds4PlayRumbleStrong()` | Play rumble on the strong motors |
| `ds4PlayRumbleSpecific()` | Play rumble on both motors specificaly |

## Known Issues

- `ds4Disconnect()` is currently unstable and may crash your ESP32
