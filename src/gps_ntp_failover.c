/**
 * @file gps_ntp_failover.c
 * @brief Combined GPS + NTP failover time synchronization system.
 *
 * This program reads UTC time from a GPS receiver via gpsd. If the GPS data
 * becomes unavailable for more than 5 seconds, the program automatically
 * switches to NTP time as a backup source. Once the GPS signal is restored,
 * it switches back to GPS time.
 *
 * Features:
 *   - Real-time UTC time printing from GPS or NTP.
 *   - Automatic failover detection (5-second timeout).
 *   - Console logging to indicate source switching.
 *
 * Dependencies:
 *   - gpsd and libgps: sudo apt install gpsd gpsd-clients libgps-dev
 *   - Chrony or NTP installed for network time synchronization
 *
 * Author: Chloe Nuzillat
 * Date: October 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <gps.h>
#include <arpa/inet.h>
#include <string.h>

/**
 * @brief Retrieves the current UTC time from an NTP server.
 *
 * Sends an NTP request packet to a public NTP server and parses the
 * transmit timestamp (seconds since 1900) into a UNIX epoch time_t value.
 *
 * @return time_t The current UTC time from NTP, or 0 if request fails.
 */
time_t get_ntp_time() {
	const char *ntp_server_ip = "129.6.15.28";
	int sockfd;
	struct sockaddr_in server_addr;
	unsigned char packet[48] = {0};
	packet[0] = 0x1B;

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0) {
		perror("Socket creation failed");
		return 0;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(123);
	inet_pton(AF_INET, ntp_server_ip, &server_addr.sin_addr);

	if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Failed to send NTP request");
		close(sockfd);
		return 0;
	}

	struct timeval timeout = {2, 0};
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout, sizeof(timeout)));


	if (recvfrom(sockfd, packet, sizeof(packet), 0, NULL, NULL) < 0) {
		perror("No NTP response received");
		close(sockfd);
		return 0;
	}

	close(sockfd);

	unsigned long seconds_since_1900 =
		((unsigned long)packet[40] << 24) |
		((unsigned long)packet[41] << 16) |
		((unsigned long)packet[42] << 8) |
		((unsigned long)packet[43]);

	return (time_t)(seconds_since_1900 - 2208988800ull);
}

int main() {
	struct gps_data_t gps_data;
	time_t last_gps_update = time(NULL);
	int using_ntp = 0; // 0 = GPS active, 1 = NTP active

	if (gps_open("localhost", "2947", &gps_data) != 0) {
		fprintf(stderr, "Could not connect to GPSD\n");
		return 1;
	}

	gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);
	printf("[INFO] GPS and NTP failover system started.\n");

	while (1) {
		if (gps_waiting(&gps_data, 500000)) {
			if (gps_read(&gps_data, NULL, 0) == -1) {
				fprintf(stderr, "[ERROR] Reading GPS data failed.\n");
			}
			else if (gps_data.fix.mode >= MODE_2D) {
				last_gps_update = time(NULL);

				if (using_ntp) {
					printf("[LOG] GPS fix restored, switching back to GPS.\n");
					using_ntp = 0;
				}

				time_t raw_time = gps_data.fix.time.tv_sec;
				struct tm *utc_time = gmtime(&raw_time);

				printf("GPS Time (UTC): %02d:%02d:%02d | Lat: %.6f | Lon: %.6f\n",
					utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec,
					gps_data.fix.latitude, gps_data.fix.longitude);

			}
			else {
				printf("[INFO] Waiting for GPS fix...\n");
			}
		}
		else {
			if (difftime(time(NULL), last_gps_update) > 5 && !using_ntp) {
				printf("[LOG] GPS signal lost for >5 seconds, switching to NTP time.\n");
				using_ntp = 1;
			}
		}

		if (using_ntp) {
			time_t ntp_time = get_ntp_time();
			if (ntp_time != 0) {
				struct tm *utc_time = gmtime(&ntp_time);
				printf("NTP Time (UTC): %02d:%02d:%02d\n",
					utc_time->tm_hour, utc_time->tm_min, utc_time->tm_sec);
			}
			else {
				printf("[WARN] Unable to retrieve NTP time.\n");
			}
		}

		sleep(1);
	}

	gps_stream(&gps_data, WATCH_DISABLE, NULL);
	gps_close(&gps_data);
	return 0;
}
