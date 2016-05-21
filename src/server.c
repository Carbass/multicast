#include "includes.h"
#include "ownthread.h"

void messageFunction( ThreadInfo *info);
int main(int argc, char **argv)
{
    int miSocket;
    int status;
    struct sockaddr_in6 serveraddr, clientaddr;
    socklen_t sock6len;
    char buffer[MAXLINE];
    time_t ticks;

    char const *fileName = "../files/mensajes.txt";
    FILE *messageFile;
    char line[100];
    char message[50], timeInterval[10], totalTime[10];
    char *ptr;
    char separador[2] = "#";
    char separaArgs[2] = "%";
    int numberOfLines= 0;
    int i;

    pthread_t *threads;
    ThreadInfo *threadInfo;
    struct ipv6_mreq destino_multicast;
    char varSepara[30];
    char ipv6_multicast[30];
    int port;
    char interface[8];

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

    if((miSocket = socket(AF_INET6, SOCK_DGRAM, 0)) < 0 )
    {
	perror("Error en socket\n");
	exit(1);
    
    }

    bzero(&serveraddr , sizeof(serveraddr));
    
    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_addr = in6addr_any;
    serveraddr.sin6_port = htons(0);
    
    sock6len = sizeof(serveraddr);

    status = bind ( miSocket, (SA *) &serveraddr, sock6len);
    if(status < 0)
    {
	perror("Error en bind\n");
	exit(1);
     
    }

    numberOfLines = numberOfLinesOfFile(fileName);

    threadInfo = (ThreadInfo*)malloc(sizeof(ThreadInfo)*numberOfLines);
    threads = (pthread_t *)malloc(sizeof(pthread_t)*numberOfLines);

    if( inet_pton(AF_INET6,ipv6_multicast,&destino_multicast.ipv6mr_multiaddr)==-1)
    {
	printf("Error inet_pton");
    }

    destino_multicast.ipv6mr_interface = if_nametoindex(interface);

    messageFile = fopen(fileName,"r");

    for(i=0; i<numberOfLines; i++){
    
	fgets(line, sizeof(line),messageFile);
    
	ptr = strtok(line, separador);
	strcpy(message, ptr);
	ptr = strtok(NULL, separador);
	strcpy(timeInterval, ptr);
	ptr = strtok(NULL , separador);
	strcpy(totalTime, ptr);

	strcpy(threadInfo[i].messageInfo.message,message);
	strcpy(threadInfo[i].messageInfo.timeInterval, timeInterval);
	strcpy(threadInfo[i].messageInfo.totalTime, totalTime);

	threadInfo[i].netInfo.socket = miSocket;
	threadInfo[i].netInfo.addr.sin6_family=AF_INET6;
	threadInfo[i].netInfo.addr.sin6_addr=destino_multicast.ipv6mr_multiaddr;
	threadInfo[i].netInfo.addr.sin6_port = htons(port);

	status = pthread_create(&threads[i], NULL, (void *)&messageFunction, &threadInfo[i]);
	if( status !=0)
	{
	    perror("Error al crear el thread\n");
	}
    }

    fclose(messageFile);

    for(i=0; i<numberOfLines; i++){ 

	status = pthread_join( threads[i], NULL);

	if(status != 0)
	{
	    printf("Thread unido\n");
	}
    }

    
    free(threads);
    free(threadInfo);
    close(miSocket);
    return 0;
}


int numberOfLinesOfFile(char *fileName){

    FILE *file;
    char line[100];
    int numberOfLines= 0;

    file = fopen(fileName,"r");

    while(fgets(line, sizeof(line),file))
    {
	numberOfLines++;
    }
    
    fclose(file);
    return numberOfLines; 
}

void messageFunction( ThreadInfo *info){

    int interval = atoi(info->messageInfo.timeInterval);
    int totalTime = atoi(info->messageInfo.totalTime);
    char buffer[MAXLINE];
    int status;
    int sock6len;
    int i;

    sock6len = sizeof(info->netInfo.addr);

    for(i=0;i<=totalTime ;i=i+interval)
    {
	sleep(interval);    
	printf("Mensaje enviado\n");
	snprintf(buffer ,sizeof(buffer),"%.50s \n", info->messageInfo.message);
	status = sendto ( info->netInfo.socket, buffer, strlen(buffer), 0, (SA *)&info->netInfo.addr, sock6len);
    }      
}
