#include <stdlib.h>	/* defines system calls */
#include <stdio.h>	/* needed for printf */
#include <string.h>	/* needed for memset */
#include <sys/socket.h>
#include <netinet/in.h>	/* needed for sockaddr_in */
#include <netinet/ip.h>	/* needed for sockaddr_in */
#include <arpa/inet.h> /* needed for converting presentational addresses to network format. */
#include <sys/types.h>
#include <stdio.h>
#include <netdb.h>      /* for gethostbyname() */
#include <unistd.h>  

#if !defined(IP_FREEBIND)
#define IP_FREEBIND 15
#endif /* !IP_FREEBIND */

#if !defined(SOL_IP)
#define SOL_IP 0
#endif /* !SOL_IP */


int connection_test(int);

void paddr(unsigned char* a) {
    printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}

int main() {
    int i = 0, j = 63, k = 0;
    char ip[16];
    while (i < 1) {
        int sockfd, portno;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("cannot create socket");
            return -1;
        }
        int v = 1;
        // if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v)) != 0) {
        //     perror("cannot set socket options");
        //     return -5;
        // }
        if (setsockopt(sockfd, SOL_IP, IP_FREEBIND, &v, sizeof(v)) != 0) {
            perror("cannot set socket options");
            return -4;
        } else{
            k = 1;
        }
        struct sockaddr_in myaddr;
        socklen_t addrlen = sizeof(myaddr);

        /* INADDR_ANY is the IP address and 0 is the socket */
        /* htonl converts a long integer (e.g. address) to a network representation */
        /* htons converts a short integer (e.g. port) to a network representation */

        memset((char*)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        // myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        sprintf(ip, "192.168.51.%d",j);
        inet_pton(AF_INET, ip, &myaddr.sin_addr);

        myaddr.sin_port = htons(0);

        if (bind(sockfd, (struct sockaddr*)&myaddr, sizeof(myaddr)) < 0) {
            perror("bind failed");
            j += 1;
            if (!k) return -2;
        }

        if (getsockname(sockfd, (struct sockaddr*)&myaddr, &addrlen) < 0) {
            perror("ERROR on getsockname");
            return -3;
        }

        portno = ntohs(myaddr.sin_port);
        printf("i = %d | IP: %s, port: %d\n", i, ip, portno);

        i += 1;

        connection_test(sockfd);
    }

    return 0;

}

int connection_test(int fd) {
    // char *host = "example.com";
    // struct hostent *hp;     /* host information */
    // /* look up the address of the server given its name */
    // hp = gethostbyname(host);
    // if (!hp) {
    //     fprintf(stderr, "could not obtain address of %s\n", host);
    //     return 0;
    // } else {
    //     printf("IP Address of Host: ");
    //     paddr(hp->h_addr_list[0]);
    // }

    struct sockaddr_in servaddr;    /* server address */
    /* fill in the server's address and data */
    memset((char*)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);



    inet_pton(AF_INET, "192.168.51.63", &servaddr.sin_addr);

    // /* put the host's address into the server address structure */
    // memcpy((void *)&servaddr.sin_addr,hp->h_addr_list[0], hp->h_length);

    /* connect to server */
    if (connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
        return 0;
    }
    write(fd,"Hello Server",12);
}