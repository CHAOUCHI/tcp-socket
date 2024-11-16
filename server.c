#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define SERVER_PORT 3001


void debug_sleep(){
    // ---------- Debug ---- sleep pour voir les étapes défiler lentement.----------
    printf("\n");
    sleep(2);
    fflush(stdout);
}
int main(){



    /*
    *
    * 1. CREATION DU SOCKET
    * Je crée un socket avec la fonction socket()
    * Je lui fournit : 
    * - Sa famille de socket (AF_INET pour les socket réseau)
    * - Une IP (INADDR_ANY pour toutes les ip possible : 127.0.0.1 ou autres)
    * - Un port
    * 
    */
    int socket_id = socket(AF_INET,SOCK_STREAM,0);perror("socket()");
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(SERVER_PORT) // convert int port to binary port
    };



// debug_sleep();











    /**
     * 2. Bind(liaison) du socket à un file_descriptor
     */
    bind(socket_id,(struct sockaddr*)&addr,sizeof(addr));perror("bind()");
















    /**
     * 3.  Listen
     * Lancer l'écoute du serveur
     */
    // listen
    listen(socket_id,BUFSIZ);perror("listen()");
    printf("Server listen to 127.0.0.1:%d\n",SERVER_PORT);









// debug_sleep();





    int count_accept = 0;

    // accept les clients en boucle
    while(1){

        /**
         * 4. accept
         * ATTEND qu'un demande de connexion arrive d'un client
         * Le client qui utilise la fonction connect() déclanchera cet accept()
         */
        struct sockaddr client_addr;
        socklen_t len;
        printf("accept nuemro : %d\n",count_accept);
        printf("Wait for a client to connect...\n");
        int client_fd = accept(socket_id,&client_addr,&len); perror("accept()");
        count_accept++;
        
        












        /**
         * 5.  recv
         * Lecture du file descriptor en boucle pour recevoir les requete client
         */
        // Read client socket - recv
        char buf[BUFSIZ]; memset(buf,0,BUFSIZ);
        // Je remplis le buf avec le contenu du fd
        recv(client_fd,buf,BUFSIZ,0);
        perror("recv()");





    // debug_sleep();













        /**
         * 5.1 Si le fd contient du texte c'est qu'un client à utilisé send() pour envoyer une donnée
         */
        if(strlen(buf) > 0){
            // If the client_fd contain something

            // Le message du client est contenu dans le buf
            printf("> %s\n",buf); // Print the message sent by the client

            /**
             * 5.1bis Même si je ne suis pas obligé j'envoi un message au client qui viens se connecter
             * Typiquement le client à demandé une donnée et je lui renvoi la donnée en fonction de son message
             */
            char html[BUFSIZ];memset(html,0,BUFSIZ);
            FILE* fd_html = fopen("index.html","r+");
            fread(html,sizeof(char),BUFSIZ,fd_html);
            
            char response[BUFSIZ];memset(response,0,BUFSIZ);
            
            sprintf(response,"HTTP/1.1 200 OK\n\n%s",html);
            send(client_fd,response,strlen(response),0);







    // debug_sleep();







            // Close the connexion to the client



        }
        close(client_fd);perror("close");
    }

// debug_sleep();


    printf("Server shutsdown...\n");

    return 0;
}

