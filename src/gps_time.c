/**
 * @file gps_time.c
 * @brief Connects to gpsd and prints the current GPS UTC time once per second.
 *
 * This program communicates with the gpsd daemon running on localhost to read
 * GPS data from a USB GPS receiver (VK-172). Once a valid GPS fix
 * is obtained, it extracts and prints the UTC time each second.
 *
 * Dependencies:
 *   - gpsd and libgps (sudo apt install gpsd gpsd-clients libgps-dev)
 *   - A GPS receiver supported by gpsd (e.g., /dev/ttyACM0)
 *
 * @author Chloe Nuzillat
 * @date October 2025
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <gps.h>

int main() {
    struct gps_data_t gps_data;

    if (gps_open("localhost", "2947", &gps_data) != 0) {
        fprintf(stderr, "Could not connect to GPSD\n");
        return 1;
    }

    gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);

    while (1) {

        if (gps_waiting(&gps_data, 5000000)) {

            if (gps_read(&gps_data, NULL, 0) == -1) {
                fprintf(stderr, "Error reading GPS data\n");
                break;
            }
            else {
                if (gps_data.fix.mode >= MODE_2D) {

                        time_t raw_time = gps_data.fix.time.tv_sec;
                        struct tm *utc_time = gmtime(&raw_time);

                        printf("GPS Time (UTC): %02d:%02d:%02d\n",
                            utc_time->tm_hour,
                            utc_time->tm_min,
                            utc_time->tm_sec);

                        printf("Latitude: %.6f | Longitude: %.6f | Mode: %d\n",
                            gps_data.fix.latitude,
                            gps_data.fix.longitude,
                            gps_data.fix.mode);
                }
                else {
                    printf("Waiting for GPS fix...\n");
                }
            }
        }
        else {
            printf("No data from GPSD (timeout)\n");
        }

        sleep(1);
    }

    gps_stream(&gps_data, WATCH_DISABLE, NULL);
    gps_close(&gps_data);

    return 0;
}
