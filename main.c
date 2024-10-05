#include <stdlib.h>	/* defines system calls */
#include <stdio.h>	/* needed for printf */
#include <string.h>	/* needed for memset */
#include <sys/socket.h>
#include <netinet/in.h>	/* needed for sockaddr_in */
#include <netinet/ip.h>	/* needed for sockaddr_in */
#include <arpa/inet.h> /* needed for converting presentational addresses to network format. */
#include <sys/types.h>

#if !defined(IP_FREEBIND)
#define IP_FREEBIND 15
#endif /* !IP_FREEBIND */

#if !defined(SOL_IP)
#define SOL_IP 0
#endif /* !SOL_IP */

void paddr(unsigned char* a) {
    printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}

int main() {
    int i = 0;
    while (i < 65536) {
        int fd, portno;
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("cannot create socket");
            return -1;
        }
        int v = 1;
        // if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v)) != 0) {
        //     perror("cannot set socket options");
        //     return -5;
        // }
        if (setsockopt(fd, SOL_IP, IP_FREEBIND, &v, sizeof(v)) != 0) {
            perror("cannot set socket options");
            return -4;
        }
        struct sockaddr_in myaddr;
        socklen_t addrlen = sizeof(myaddr);

        /* INADDR_ANY is the IP address and 0 is the socket */
        /* htonl converts a long integer (e.g. address) to a network representation */
        /* htons converts a short integer (e.g. port) to a network representation */

        memset((char*)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        // myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        inet_pton(AF_INET, "192.168.51.24", &myaddr.sin_addr);

        myaddr.sin_port = htons(0);

        if (bind(fd, (struct sockaddr*)&myaddr, sizeof(myaddr)) < 0) {
            perror("bind failed");
            return -2;
        }

        if (getsockname(fd, (struct sockaddr*)&myaddr, &addrlen) < 0) {
            perror("ERROR on getsockname");
            return -3;
        }

        portno = ntohs(myaddr.sin_port);
        printf("i = %d | Bound port: %d\n", i, portno);

        i += 1;
    }

    return 0;

}