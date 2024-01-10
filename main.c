#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include "order.h"

void print_bytes(const char *, unsigned char *, int);
void print_message(struct Order *);
void unpack_message(struct Order *, unsigned char *);
int  checksum_valid(unsigned char *, int);

int main() {

	struct Order order;
	unsigned char buff[BUFF_SIZE];
	int msg_count = 1;

	while(next_order(buff)) {
		printf("\n=====================\n");
		printf("Processing Message %d\n", msg_count);
		printf("=====================\n");

		print_bytes("raw buffer", buff, BUFF_SIZE);

		int chk_valid = checksum_valid(buff, HDR_SIZE);

		if(chk_valid) {

		} else {

		}

		printf("\n");
		msg_count++;
	}

	return 0;
}
