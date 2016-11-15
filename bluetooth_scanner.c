#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
// gcc -o  bluetooth.c -lbluetooth
int main(int argc, char **argv)
{
    inquiry_info *ii = NULL; // array of BLE devices+info found in scan
    int max_rsp, num_rsp; // max_rsp: max # of receivers
    int dev_id, sock, len, flags; // flags: IREQ_CACHE_FLUSH: flush all prev devices found and start search from scratch; 0: include previous devices found in output
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    // SET UP BLUETOOTH RECEIVER (on laptop)
    dev_id = hci_get_route(NULL); // get adapter
    sock = hci_open_dev( dev_id ); // open connection
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    // Scan for BLE devices; collect info about them
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    // Loop through BLE devices found
    for (i = 0; i < num_rsp; i++) {
        // Convert bluetooth address of device to string
        ba2str(&(ii+i)->bdaddr, addr);

        memset(name, 0, sizeof(name));
        // Read user-friendly name of devices found
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
            name, 0) < 0)
        strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);
    }

    free( ii );
    close( sock );
    return 0;
}
