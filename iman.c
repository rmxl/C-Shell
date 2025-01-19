#include "./iman.h"

void removeTagsAndPrint(char* buffer){
    int i = 0, flag = 0;

    while(buffer[i] != '<') i++; // going past headers
 
    while(buffer[i] != '\0'){
        if(buffer[i] == '<'){
            //inside a tag so we don't print
            flag = 1;
        }
        if(flag == 0){
            printf("%c", buffer[i]);
        }
        if(buffer[i] == '>'){
            // outside tag now
            flag = 0;
        }
        i++;
    }
}

int iman(char* commmandMan){
    char commandFinal[INPUT_LENGTH_MAX] = {0};
    char temp[INPUT_LENGTH_MAX] = {0};
    int i;
    for(i = 0 ; i < strlen(commmandMan) ; i++){
        if(commmandMan[i] != '(' ){
            commandFinal[i] = commmandMan[i];
        }
        else{
            //if user wants to access sleep(2) instead of sleep
            int j = 0;
            i++;
            while(commmandMan[i] != ')'){
                temp[j] = commmandMan[i];
                i++; j++;
            }
            break;
        }
    }
    char link[INPUT_LENGTH_MAX] = {0}; 
    char site[INPUT_LENGTH_MAX] = "man.he.net";

    struct addrinfo addresses, *output;
    memset(&addresses, 0, sizeof( addresses));
    addresses.ai_family = AF_UNSPEC;
    addresses.ai_socktype = SOCK_STREAM;

    int status;
    if ((status = getaddrinfo(site, "80", &addresses, &output)) != 0) {
        printColor("ERROR : Not able to connect.", RED);
        freeaddrinfo(output);
        return -1;
    }

    int s = socket(output->ai_family, output->ai_socktype, output->ai_protocol);
    if(s == -1){
        printColor("ERROR : Not able to create socket.", RED);
        freeaddrinfo(output);
        return -1;
    }

    if(connect(s, output->ai_addr, output->ai_addrlen) == -1){
        printColor("ERROR : Not able to connect.", RED);
        freeaddrinfo(output);
        return -1;
    }

    sprintf(link, "topic=%s&section=%s", commandFinal, i == strlen(commmandMan) ? "all" : temp); // link to page
    char getRequest[INPUT_LENGTH_MAX] = {0};
    sprintf(getRequest, "GET /?%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", link, site);
    // the GET request

    if(send(s, getRequest, strlen(getRequest), 0) == -1){
        printColor("ERROR : Not able to send.", RED);
        freeaddrinfo(output);
        return -1;
    }

    char buffer[15000] = {0};
    int bytesReceived = 0;
    while((bytesReceived = recv(s, buffer, 10000, 0)) > 0){
        buffer[bytesReceived] = '\0';
        removeTagsAndPrint(buffer);
    }

    if(bytesReceived == -1){
        printColor("ERROR : Not able to receive.", RED);
        return -1;
    }

    close(s);
    freeaddrinfo(output);
    return 0;    
}
