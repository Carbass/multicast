#include "includes.h"

int miSocket;
char ipv6_multicast[30];

void signalHandler(int signum, siginfo_t *info, void *ptr);
int main(int argc, char **argv)
{
    int status;
    char recvline[MAXLINE + 1];
    char separador[2] = "%";
    char ipserver[255];
    char *ptr;

    struct sockaddr_in6 serveraddr;
    struct ipv6_mreq src_multicast;
    int enable=1;

    struct sigaction act;
    bzero(&act, sizeof(act));
    act.sa_sigaction = signalHandler;
    sigaction(SIGINT, &act, NULL);

    char separaArgs[2] = "%";
    char varSepara[30];
    int port;
    char interface[10];

    if( argc != 2){
	printf("No hay argumentos de entrada o son incorrectos.\n");
	printf("Uso: <IPv6GrupoMulticast>%c<Puerto>%c<Interfaz>\n",37,37);
	printf("Se utilizarán los argumentos por defecto\n");
	strcpy(ipv6_multicast, IP6_MULTICAST);
	port = PORT;
	strcpy(interface, INTERFACE);
    }else{
	ptr = strtok(argv[1], separaArgs);
	strcpy(ipv6_multicast, ptr);
	ptr = strtok(NULL, separaArgs);
	if(ptr != NULL){
	    strcpy(varSepara, ptr);
	    port = atoi(varSepara);
	    ptr = strtok(NULL , separaArgs);
	    if( ptr != NULL)
		strcpy(interface, ptr);
	    else
		strcpy(interface, INTERFACE);
	}else{
	    port = PORT;
	    strcpy(interface, INTERFACE);
	}
    }

    bzero(&serveraddr, sizeof(serveraddr));


    miSocket = socket(AF_INET6, SOCK_DGRAM, 0);

    if( inet_pton(AF_INET6,ipv6_multicast,&src_multicast.ipv6mr_multiaddr)==-1)
    {
	printf("Error inet_pton");
    }

    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_addr = src_multicast.ipv6mr_multiaddr;
    serveraddr.sin6_port = htons(port);  

    if(setsockopt(miSocket,SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))==-1)
    {
	printf("Error al unirse al grupo multicast\n");
    }

    status = bind ( miSocket, (SA *) &serveraddr, sizeof(serveraddr));
    if(status < 0)
    {
	perror("Error en bind\n");
	exit(1);
     
    }
    if(setsockopt(miSocket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &src_multicast, sizeof(src_multicast))==-1)
    {
	printf("Error al unirse al grupo multicast\n");
    }

    for (;;){
    status = recvfrom(miSocket, recvline, MAXLINE, 0, NULL, NULL);

    recvline[status]=0;
    printf("%s", recvline);
    }
    close(miSocket);

    return 0;
}

void signalHandler(int signum, siginfo_t *info, void *ptr){

    struct ipv6_mreq src_multicast;

    if( inet_pton(AF_INET6,ipv6_multicast,&src_multicast.ipv6mr_multiaddr)==-1)
    {
	printf("Error inet_pton");
    }

    if(setsockopt(miSocket, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, &src_multicast, sizeof(src_multicast))==-1)
    {
	printf("Error al salir del grupo multicast\n");
    }
    exit(1);
}
