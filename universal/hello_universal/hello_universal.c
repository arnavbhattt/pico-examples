#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "boot/picoboot.h"
#include "FreeRTOS.h"
#include "task.h"

// Task function prototypes
void hello_task(void *pvParameters);
void toggle_led_task(void *pvParameters);

// Delay between LED toggling and maximum reboot count
#define LED_TOGGLE_DELAY_MS 1000
#define MAX_REBOOT_COUNT 10

// Task priorities
#define HELLO_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define TOGGLE_LED_TASK_PRIORITY (tskIDLE_PRIORITY + 2)

// Stack sizes for each task
#define HELLO_TASK_STACK_SIZE 1024
#define TOGGLE_LED_TASK_STACK_SIZE 1024

void hello_task(void *pvParameters)
{
        uint reboot_count = 0;

        while (true)
        {
                printf("Hello, world!\n");

#if PICO_RP2350
                printf("I'm an RP2350 ");
#ifdef __riscv
                printf("running RISC-V\n");
#else
                printf("running ARM\n");
#endif
                reboot_count++;
                if (reboot_count >= MAX_REBOOT_COUNT)
                {
#ifdef __riscv
                        rom_reboot(REBOOT2_FLAG_REBOOT_TYPE_NORMAL | REBOOT2_FLAG_REBOOT_TO_ARM, 1000, 0, 0);
#else
                        rom_reboot(REBOOT2_FLAG_REBOOT_TYPE_NORMAL | REBOOT2_FLAG_REBOOT_TO_RISCV, 1000, 0, 0);
#endif
                        printf("Rebooting to other architecture\n");
                }
#else
                printf("I'm an RP2040\n");
#endif

                // Delay before printing the next message
                vTaskDelay(pdMS_TO_TICKS(LED_TOGGLE_DELAY_MS));
        }
}

void toggle_led_task(void *pvParameters)
{
#ifdef PICO_DEFAULT_LED_PIN
        gpio_init(PICO_DEFAULT_LED_PIN);
        gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

        bool toggle = true;
        while (true)
        {
                gpio_put(PICO_DEFAULT_LED_PIN, toggle);
                toggle = !toggle;

                // Delay before toggling again
                vTaskDelay(pdMS_TO_TICKS(LED_TOGGLE_DELAY_MS));
        }
#else
        while (true)
        {
                // If no LED is available, this task does nothing
                vTaskDelay(pdMS_TO_TICKS(LED_TOGGLE_DELAY_MS));
        }
#endif
}

int main()
{
        stdio_init_all();

        // Create FreeRTOS tasks
        xTaskCreate(hello_task, "HelloTask", HELLO_TASK_STACK_SIZE, NULL, HELLO_TASK_PRIORITY, NULL);
        xTaskCreate(toggle_led_task, "ToggleLEDTask", TOGGLE_LED_TASK_STACK_SIZE, NULL, TOGGLE_LED_TASK_PRIORITY, NULL);

        // Start the FreeRTOS scheduler
        vTaskStartScheduler();

        // The program should never reach here
        while (true)
        {
        }
}
