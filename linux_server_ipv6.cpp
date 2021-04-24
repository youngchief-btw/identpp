#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
using namespace std;

int main(int argc, char const *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char* buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR Opening socket.");
        exit(1);
    }
    else
    {
        cout << "Opened socket!\n";
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = 1130; // Port number goes here.

    // Configure server details.
    serv_addr.sin_family = AF_INET6;         // More details on types: man socket
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Allow any interface.
    serv_addr.sin_port = htons(portno);

    cout << "Port: " << portno << "\n"; // Output the port number, you could also ask for a custom port and use that.

    // Start binding the socket.
    if (
        bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR binding.");
        exit(1);
    }
    else
    {
        cout << "Binded socket!\n";
    }

    // Nap until it gets an incomming connection.
    if (listen(sockfd, 5) < 0)
    {
        perror("ERROR Listening to the socket");
    }
    else
    {
        // change 5 to how ever many connections you want in queue before refusing to connect
        cout << "Now listening to the socket!\n";
        clilen = sizeof(cli_addr); // 16 is default, change this to how ever many (or little) you want!
        cout << "Number of connections allowed in queue before refusing to connect: " << clilen
             << "\n";
    }

    while(1) 
    {
    // Accept the connection from client
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
    {
        perror("ERROR Socket accept");
        exit(1);
    }
    else
    {
        cout << "Accepted connection!\n";
    }

    // Connected established, let's start communicating
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("ERROR Reading from socket!\n");
        exit(1);
    }
    else
    {
        cout << "Connection estabilished!\n";
    }

    printf("Recieved message: \n%s\n", buffer);

    // Parse message.
    string bufferstr;
    bufferstr = *buffer;
    // bufferstr.copy(buffer);
    string clientport = bufferstr.substr(0, bufferstr.find(", "));
    string remoteport = bufferstr.substr(1, bufferstr.find(", "));

    // Compose reply. You can put if, if else, and more to customize the reply.
    string userid = "0"; // System user ID.
    string username = "youngchief"; // Username of user.
    string operatingSystem = "UNIX"; // Operating system.
    // Compose the reply for success.
    string composedReply = clientport + ", " + remoteport + " : " + userid + " : " + operatingSystem + " : " + username + "\r\n";
    // Compose the reply for failure
    string composedReplyError = clientport + ", " + remoteport + " : ERROR : NO-USER\r\n";

    // Send something back to client!
    n = write(newsockfd, composedReply.c_str(), sizeof(composedReply.c_str()));
    if (n < 0)
    {
        perror("ERROR writing to socket failed.");
        exit(1);
    }
    else
    {
        cout << "Sent the composed reply message!\n";
    }
    }

    // Close socket
    close(sockfd);
    close(newsockfd);
    
    return 0;
};
