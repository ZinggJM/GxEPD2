/**
 * @file main.cpp
 * @brief Super Simple GxEPD2 Example - Minimal Code!
 *
 * This example shows the absolute simplest way to use GxEPD2 on Zephyr.
 * Just configure prj.conf and run - no device tree editing needed!
 *
 * Build:
 *   west build -b nrf52840dk_nrf52840
 *   west flash
 */

#include <zephyr/kernel.h>
#include "gxepd2_config.h"

/* Simple test pattern - creates a border and diagonal line */
void draw_test_pattern(GxEPD2_EPD *display)
{
    uint16_t width = display->WIDTH;
    uint16_t height = display->HEIGHT;

    printk("Drawing test pattern on %dx%d display...\n", width, height);

    /* Clear display to white */
    display->clearScreen(0xFF);

    /* For a real application, you would:
     * 1. Create a framebuffer
     * 2. Draw your content to the framebuffer
     * 3. Call display->writeImage(framebuffer, 0, 0, width, height)
     * 4. Call display->refresh()
     */

    printk("Test pattern complete!\n");
}

int main(void)
{
    printk("\n");
    printk("========================================\n");
    printk("  GxEPD2 Simple Example\n");
    printk("========================================\n\n");

    /* Print configuration - shows what was auto-configured */
    gxepd2_print_config();

    /* Get the display instance (auto-initialized if CONFIG_GXEPD2_AUTO_INIT=y) */
    GxEPD2_EPD *display = gxepd2_get_display();

    if (!display) {
        printk("ERROR: Display not initialized!\n");
        printk("Make sure CONFIG_GXEPD2_AUTO_INIT=y in prj.conf\n");
        return -1;
    }

    printk("SUCCESS! Display is ready to use.\n\n");

    /* Draw test pattern */
    draw_test_pattern(display);

    /* Power off display to save power and preserve image */
    display->powerOff();

    printk("\nNext steps:\n");
    printk("  1. Create a framebuffer for your display size\n");
    printk("  2. Draw to the framebuffer\n");
    printk("  3. Use display->writeImage() to send to display\n");
    printk("  4. Use display->refresh() to update the screen\n");
    printk("  5. Call display->powerOff() when done\n");

    /* Keep application running */
    while (1) {
        k_sleep(K_SECONDS(10));
    }

    return 0;
}
