/*Include the needed libraries for functionality*/
#include "ADS1299_definitions.h"
#include "ADS1299_bcm2835.h"
#include "ads1299.h"
#include <math.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{

    int opt;
    bool  do_bootup = true, do_test = false, dry_run = false, output_wave = false, power_down = false;


    char* test_name = NULL;
    bool bootup_success = false;
    int num_attempts = 1;
    int output_test_pin_num = 2;
    int test_pin = TEST_PIN_2;
    int attempt = 1;

    // Parse command line options
    // -b -> Perform bootup sequence: bcm2835_init() -> ADS1299_init() -> AS1299_bootup()
    // -t -> Perform specific test
    // -w -> Output wave on to one of the three test pins.
    // -r -> Repeat test if failed. Default number of repeats is 1
    // -d -> Running dry_run mode (not on Raspberry Pi)
    // -p -> Power down chip after testing

     while ((opt = getopt(argc, argv, "bdpt:r:w:")) != -1) {
        switch (opt) {
        case 'd':
            dry_run = true;
            printf("\ngetopt - received 'd'\n");
            break;
        case 'r':
            num_attempts = atoi(optarg);
            printf("\ngetopt - received 'r'. num_attempts = %d\n", num_attempts);
            break;
        case 'b':
            do_bootup = true;
            printf("\ngetopt - received 'b'\n");
            break;
        case 'w':
            output_wave = true;
            // Do not bootup the chip
            do_bootup = false;

            output_test_pin_num = atoi(optarg);
            if (output_test_pin_num == 2)
                test_pin = TEST_PIN_2;
            else if (output_test_pin_num == 3)
                test_pin = TEST_PIN_3;
            else {
                fprintf(stderr, "Error: Invalid test pin. Must be 2 or 3");
                exit(EXIT_FAILURE);
            }

            printf("\ngetopt - received 'w'. test_pin = %d\n", output_test_pin_num);

            break;
        case 't':
            do_test = true;
            do_bootup = true;
            test_name = optarg;
            printf("\ngetopt - received 't'. test_name = %s\n", test_name);
            break;
        case 'p':
            power_down = true;
            printf("\ngetopt - received 'p'\n");
            break;
        default:
            fprintf(stderr, "Usage: %s [-bt] [file...]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (!dry_run) { // If running on the Raspberry Pi

        // If error in intializing bcm
        if (!bcm2835_init()) {
    	   printf("Failed to init the Raspberry PI");
    	   return 1;
        }

       // Note: Disable other tests or sequences if testing with a test pin
        if (output_wave) {
            // set attempt = num_attempts + 1 so we don't enter the following loop
            attempt = num_attempts + 1;

            // Output a 100 kHz wave to test_pin for 300 seconds (5 minutes).
            // Note: This is a blocking call.
            output_square_wave(test_pin, 100000, 300);
        }

        while (attempt <= num_attempts && !bootup_success) {

            if (do_bootup) {

                printf("\n-- ADS1299 Boot-up --\n");

                // Initialize BCM Configuration to communicate with chip
                ADS1299_init();

        	    delayMicroseconds(100);

                // bootup sequence
                ADS1299_bootup();
            }

            // Tests:
            //
            //      register_test        - Bootup and read the 3 CONFIG registers and compare the read values against their default values
            //      sdatac_test          - Bootup and send sdatac command
            //      pwrup_ref_buf_test   - Bootup and write to 0xE0 to CONFIG3 register to power up the reference buffer.
            //      power_cycle_test     - Do 5 cycles of bootup and power down

    	    if (do_test) {

                // Cannot do tests if ADS1299 was not boot-up
                if (!do_bootup) {
                    printf("\nError: ADS1299 is not bootup!!!\n");
                    return 1;
                }

                if (!strcmp(test_name, "register_test")) { // Test registers' default values

                    // Send SDATAC before reading/writing registers
                    transferCmd(_SDATAC);

                    // Write PD_REFBUF = 1
                    ADS1299_config();

                    // Set data rate
                    printf("Set data rate - start\n");
                    ADS1299_write_register(CONFIG1, 0x96);
                    ADS1299_write_register(CONFIG2, 0xC0);
                    printf("Set data rate - done\n");

                    // Set all channels to input short
                    int i;

                    printf("\nWrite all CHNSET to 0x01\n");
                    for (i=0; i < NUM_CHANNELS; i++) {
                        printf("Write CH%0dSET to 0x01 - Start", i);
                        ADS1299_write_register(CH1SET+i, 0x01);
                        printf("\nWrite CH%0dSET to 0x01 - End", i);
                    }

                    delayMicroseconds(50000);
                    // Send START command
                    printf("\nSending START commad\n");
                    transferCmd(_START);

                    // Put back in RDATAC
                    printf("\nSending RDATAC commad\n");
                    transferCmd(_RDATAC);


                    // sET-UP TEST SGIANLS
                    ADS1299_write_register(CONFIG2, 0xD0);
                    for (i=0; i < NUM_CHANNELS; i++) {
                        printf("Write CH%0dSET to 0x05 - Start", i);
                        ADS1299_write_register(CH1SET+i, 0x05);
                        printf("\nWrite CH%0dSET to 0x05 - End", i);
                    }

                    // Loop forever
                    /*
                    int recv_data = 0;
                    while(1) {
                        //if (!bcm2835_gpio_lev(PIN_DRDY)) { // conversion finished
                            // Issue 216 = 24 + 8 * 24 SCLKS
                            int j;
                            for (j = 0; j < NUM_CHANNELS + 1; j++) {
                                recv_data = transferData(0x00);

                                printf("Packet %d, Recv Data %d\n", j, recv_data);

                                // Delay 1/250 sec = 4 ms
                                delayMicroseconds(4000);
                            }
                        //} else
                    }
                    */

                    ///////////////////////////////////////////////


                    bootup_success = ADS1299_test_registers();

                    if (bootup_success) {
                        printf("\n-- ADS1299 Boot-up Successful --\n");
                    } else {
                        printf("\n-- ADS1299 Boot-up Failed --\n");
                    }

                } else if (!strcmp(test_name, "sdatac_test")) {

                    printf("\n--- ADS1299 SDATAC test ---\n");

                    transferCmd(_SDATAC);

                    printf("\n--- ADS1299 SDATAC test Finished ---\n");

                } else if (!strcmp(test_name, "pwrup_ref_buf_test")) {

                    printf("\n--- ADS1299 Power-up reference buffer test ---\n");

                    transferCmd(_SDATAC);

                    ADS1299_config();

                    printf("\n--- ADS1299 Power-up reference buffer test Finished ---\n");

                } else if (!strcmp(test_name, "power_cycle_test")) {

                    int cycle;
                    printf("\n--- ADS1299 power cycle test ---\n");

                    if (!do_bootup) {

                        ADS1299_init();

                        delayMicroseconds(100);

                        ADS1299_bootup();
                    }

                    for (cycle = 0; cycle < 5; cycle++) {
                        printf("\nReboot #%d\n", cycle + 1);

                        // Power down the chip, wait 2 seconds, and reboot
                        ADS1299_reboot();
                    }

                    printf("\n--- ADS1299 power cycle test Finished ---\n");
                } else if (!strcmp(test_name, "data_test")) {

                    printf("\n--- ADS1299 data test Started ---\n");
                    transferCmd(_START);
                    delayMicroseconds(500);
                    display_all_pin_states();
                    printf("\n--- ADS1299 data test Finished ---\n");
                } else if (!strcmp(test_name, "stop_test")) {

                    printf("\n--- ADS1299 stop test Started ---\n");
                    transferCmd(_STOP);
                    delayMicroseconds(500);
                    display_all_pin_states();
                    printf("\n--- ADS1299 stop test Finished ---\n");
                } else {
                    fprintf(stderr, "Test %s not found\n", test_name);
                    exit(EXIT_FAILURE);
                }
            }

            attempt++;
            // Delay 200 us before moving on or reattempting bootup
            bcm2835_delayMicroseconds(200);

            // Only power down if we did bootup
            if (do_bootup && power_down) {
                ADS1299_power_down();
            }

            bcm2835_delayMicroseconds(200);
        }

        if (!bootup_success ) {
            printf("\nAttempts %d of %d to boot ADS1299 unsuccessful. Aborting...\n", num_attempts, num_attempts);
    	   return 1;
        }

    }

    return 0;
}
