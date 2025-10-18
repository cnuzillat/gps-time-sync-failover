#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>

#define NTP_TIMESTAMP_DELTA 2208988800ull

int main() {
	int sockfd;
	struct sockaddr_in server_addr;
	unsigned char packet[48] = {0};

	packet[0] = 0b11100011;

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sockfd < 0) {
		perror("Socket creation failed");
		return 1;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(123);
	server_addr.sin_addr.s_addr = inet_addr("129.6.15.28");

	if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Send failed");
		close(sockfd);
		return 1;
	}

	struct sockaddr_in from_addr;
	socklen_t addr_len = sizeof(from_addr);
	if (recvfrom(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&from_addr, &addr_len) < 0) {
		perror("Receive failed");
		close(sockfd);
		return 1;
	}

	unsigned long seconds_since_1900 =
		((unsigned long)packet[40] << 24) |
		((unsigned long)packet[41] << 16) |
		((unsigned long)packet[42] << 8) |
		((unsigned long)packet[43]);

	time_t epoch_time = (time_t)(seconds_since_1900 - NTP_TIMESTAMP_DELTA);

	printf("NTP Time (UTC): %s", ctime(&epoch_time));

	close(sockfd);
	return 0;
}
