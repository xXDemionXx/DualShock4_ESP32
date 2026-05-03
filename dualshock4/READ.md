# DualShock4 library

This library provides an easy way to use the DualShock4 controller with an ESP32 using a minimal and simple API.

### Basic features

- Polling the controller buttons, variable pressure buttons and joysticks data (the touch pad isn't supported)
- Callback registration on button events
- Seting the lightbar RGB value
- Playing rumble on the controller
- Setting the ESP32's Bluetooth MAC address
- Allowing only a specific controller  MAC to connect or autoconnecting

## sdkconfig

This library can work in multiple different ways. It is required for the user to setup the wanted way and the wanted features in sdkconfig, as well as the universal settings that are needed for this library to even compile.

### The Basic setup

Before you can use the API in your project, library requirers you to configure it's basic beahaviour in `sdkconfig`, these are the steps:

> 1. In **DualShock4** turn on **Enable DS4**.
> 2. In **Bluepad32** set:
>     - Maximum of connected gamepads -> 1
>     - Target platform -> custom
>     - Maximum size of allowlist -> 1

This setup will allow you to connect any *DualShock4* controller to your *ESP32* but you must change the stored MAC address on the controller to be the same as the default MAC address of the ESP's Bluetooth stack. You can see the MAC of the ESP if you have console enabled in *Bluepad32* or you can use the `ds4GetUserAddress()` API function. To change the controllers stored MAC there are several tools:

> - **Windows** -> [SixAxisPairTool](https://sixaxispairtool.en.lo4d.com/windows)
> - **Linux** -> You are on your own
> - **MAC** -> You are on your own

If you did everything correctly you will be able to compile the basic examople and flash it to your ESP. To connect you only need to turn on the controller with the ***PS button*** and the controller should automatically connect after a few seconds.  
*(if you have the Bluepad32 console turned on you will be able to see the printout of how the connecting is happening)*

---

### Changing the ESP's MAC

If you don't want to change the MAC stored on your controller you can find out what MAC is stored on the controller and make it so your *ESP* uses that MAC fore it's Bluetooth stack.  

> To do that in `sdkconfig` in **DualShock4** set:
>
> 1. Bluetooth MAC Address Mode -> Use custom Bluetooth MAC
> 2. Input your wanted MAC into the field that poped up bellow

---

### Allow only a specific controller to connect

If you don't want any controller to be able to connect but only a specific one. You need to find out what is the MAC address of the DualShock4 controller (you can see that in the printed logs after a controller connects). After you know the controllers address you need to change the connect mode of this library.

> To do that in `sdkconfig` in **DualShock4** set:
>
> 1. Controller connect mode -> Connect only to a specific MAC address of the controller
> 2. Input your wanted MAC into the field that poped up bellow

Before you connect the first time you will have to put the *DualShock4* controller into pairing mode. You do that by holding the *share* and *PS button* for five seconds while the controller is turned off. After the controller starts doing two rapid blinks repetedly, you can turn off the controller and turn it back on normally.

---

### Event mode

The event mode allows gives the user access to the `ds4SetButtonEvent()` function. This function allows the user to register functions to be done on a button event in a seperate task that handles button events. But if you plan on just using polling and not use this function, than you can chose ***DualShock4 operation mode -> Just polling*** and you will not have an extra memory overhead of an extra task.

## How to use library

The API functions of this library are thread safe and you can call them from anywhare, The only requirement is that before you call any API function you must first initialize the library with `ds4Init()`. We recomend calling this function in `app_main()` at the start of your program.  

After that you can freely call any other `ds4` function. You can see how to use all of the feature in the `examples/` folder.
