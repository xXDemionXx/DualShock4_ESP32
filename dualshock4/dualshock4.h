// This is the API that you use to use the controller

#ifndef DUALSHOCK4_H
#define DUALSHOCK4_H

#include "dualshock4_initializer.h"
#include "ds4_polling.h"
#include "ds4_receive_type.h"

#ifdef CONFIG_DS4_MODE_EVENT
#include "btn_events.h"
#include "ds4_event_handling_init.h"
#endif

// Defines

// Types

/**
 * @brief Describes how the initialization process went.
 */
typedef enum
{
    DS4_INIT_SUCCES,
    DS4_INIT_BTSTACK_INIT_FAILED,
    DS4_INIT_BLUEPAD_INIT_FAILED,
    DS4_INIT_BUTTONS_EVENT_HANDLER_TASK_FAILED,
    DS4_INIT_FAILED
} ds4_init_e;

typedef enum
{
    DS4_COMMAND_SEND_SUCCES,
    DS4_COMMAND_SEND_FAIL_NO_CONTROLLER,
    DS4_COMMAND_SEND_FAIL_LAST_COMMAND_NOT_SENT
} ds4_command_send_e;

// Callable functions

void ds4_run_loop(void);

/**
 * @brief Sets up everything needed for ds4 before we can connect.
 *
 * @return With what state did the initialization exit.
 */
ds4_init_e ds4_init(void);

/**
 * @brief Set the RGB value of the lightbar
 *
 * @return Error code
 */
ds4_command_send_e ds4SetLightbar(uint8_t R, uint8_t G, uint8_t B);

/**
 * @brief Play rumble on the controller
 *
 * This function sends the same magnitude to the weak and strong motors.
 *
 * @param magnitude How strong should the rumble be (0-255)
 * @param duration How long the rumble will last in ms
 * @param start_delay After what time after receiving will the rumble start in ms
 * @return Error code
 */
ds4_command_send_e ds4PlayRumble(uint8_t magnitude, uint16_t duration, uint16_t start_delay);

/**
 * @brief Play weak rumble on the controller
 *
 * This function sends the magnitude only to the weak motor.
 *
 * @param magnitude How strong should the rumble be (0-255)
 * @param duration How long the rumble will last in ms
 * @param start_delay After what time after receiving will the rumble start in ms
 * @return Error code
 */
ds4_command_send_e ds4PlayRumbleWeak(uint8_t magnitude, uint16_t duration, uint16_t start_delay);

/**
 * @brief Play weak rumble on the controller
 *
 * This function sends the magnitude only to the strong motor.
 *
 * @param magnitude How strong should the rumble be (0-255)
 * @param duration How long the rumble will last in ms
 * @param start_delay After what time after receiving will the rumble start in ms
 * @return Error code
 */
ds4_command_send_e ds4PlayRumbleStrong(uint8_t magnitude, uint16_t duration, uint16_t start_delay);

/**
 * @brief Play weak rumble on the controller
 *
 * This function allows you to send specific values to the weak and strong motor.
 *
 * @param magnitude_weak How strong should the rumble be on the weak motor (0-255)
 * @param magnitude_strong How strong should the rumble be on the strong motor (0-255)
 * @param duration How long the rumble will last in ms
 * @param start_delay After what time after receiving will the rumble start in ms
 * @return Error code
 */
ds4_command_send_e ds4PlayRumbleSpecific(uint8_t magnitude_weak, uint8_t magnitude_strong, uint16_t duration, uint16_t start_delay);

/**
 * @brief Returns the connection status
 *
 * @return Connection status
 */
ds4_connection_status_e ds4GetConnectionStatus(void);

// When event mode set in config
#ifdef CONFIG_DS4_MODE_EVENT
/**
 * @brief Use this function to register a button event
 *
 * Allows you to register an event, set the function that the evnt will call and to give it
 * a vector of arguments that will be sent to the function
 *
 * @param button On which button to register the event
 * @param event Event type that will trigger the function
 * @param trigger_func Pointer to a function that will be called when the event happens
 * @param argv vector of arguments that will be sent to the triggered function
 */
void ds4SetButtonEvent(btn_e button, const ds4_btn_event_e event, void (*trigger_func)(void *), void *argv);
#endif

/**
 * @brief Pass the pointer to your ds4_data_t variable that you can use to access button values
 *
 * Access your variable like you are accessing a normal struct. The polling aproach will
 * not guarante that you allways read the freshest data, just that incoming controller
 * data will continuously be saved into the variable given by the pointer.
 * 
 */
void ds4SetPollingStruct(ds4_data_t *p);

/**
 * @brief Disconnect the controller
 *
 * If you want to use it on a button event, use it like this:
 * ds4SetButtonEvent(SOME_BUTTON, SOME_EVENT, (void (*)(void *))ds4Disconnect, NULL);
 */
void ds4Disconnect(void);

// void ds4Autoconnect();

#endif // DUALSHOCK4_H
