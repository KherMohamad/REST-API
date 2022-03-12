#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#define MAXCOMMAND 15
#define MAXUSERANDPASSSIZE 100
#define TYPE "application/json"
#define MAXIDSIZE 20
#define MAXBOOKCREDSIZE 150
int main(int argc, char *argv[])
{
    char message[BUFLEN];
    int sockfd;
    char host[51] = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com:8080";
    char input[MAXCOMMAND];
    char *buff;
    char *response;
    char *cookie;
    char *JWT;
    

 
    while(1) {
      char *response;	
      fgets(input, 150, stdin);
      if (!strcmp(input, "exit\n")) {

         break;

      } else if (!strcmp(input, "register\n")) {
      	char username[MAXUSERANDPASSSIZE];
      	char password[MAXUSERANDPASSSIZE];
        sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
      	
        printf("username=");
        fgets(username, MAXUSERANDPASSSIZE, stdin);
        username[strlen(username) - 1] = 0;

        printf("password=");
        fgets(password, MAXUSERANDPASSSIZE, stdin);
        password[strlen(password) - 1] = 0;

     
        JSON_Value *value = json_value_init_object();
        JSON_Object *object = json_value_get_object(value);
        char *payload = NULL;
        json_object_set_string(object, "username", username);
        json_object_set_string(object, "password", password);
        payload = json_serialize_to_string_pretty(value);

        
        buff = compute_post_request(host, "/api/v1/tema/auth/register", TYPE, payload, NULL, NULL);
       
        send_to_server(sockfd, buff);
        response = receive_from_server(sockfd);
        printf("%s", response);
        close_connection(sockfd);
    
      } else if (!strcmp(input, "login\n")) {
      	sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
      	char username[MAXUSERANDPASSSIZE];
        char password[MAXUSERANDPASSSIZE];
        cookie = NULL;

        printf("username=");
        fgets(username, MAXUSERANDPASSSIZE, stdin);
        username[strlen(username) - 1] = 0;

        printf("password=");
        fgets(password, MAXUSERANDPASSSIZE, stdin);
        password[strlen(password) - 1] = 0;

     
        JSON_Value *value = json_value_init_object();
        JSON_Object *object = json_value_get_object(value);
        char *payload = NULL;
        json_object_set_string(object, "username", username);
        json_object_set_string(object, "password", password);
        payload = json_serialize_to_string_pretty(value);


        buff = compute_post_request(host, "/api/v1/tema/auth/login", TYPE, payload, NULL, NULL);
        buff[strlen(buff)] = 0;
        printf("%s\n", buff);
     
        send_to_server(sockfd, buff);
        response = receive_from_server(sockfd);
        printf("%s", response);
        close_connection(sockfd);
        cookie = strstr(response, "Set-Cookie:");
        if (cookie != NULL) {
        cookie = strtok(cookie + 12, ";");
    }
   
        } else if (!strcmp(input, "enter_library\n")) {
        	sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        	char cookieHeader[BUFLEN + 8];
        	sprintf(cookieHeader, "Cookie: ");
        	if (cookie != NULL) {
        		sprintf(cookieHeader, "Cookie: %s", cookie);
        	} else {
        		printf("error: youd don't have a session cookie");
                continue;        	
        	}

            buff  = compute_get_request(host, "/api/v1/tema/library/access", cookieHeader, NULL);
            printf("%s", buff);
            send_to_server(sockfd, buff);
            response = receive_from_server(sockfd);
            //printf("%s\n", response);
            if (strstr(response, "error:") != NULL) {
            	printf("%s\n", response);
            } else {
            	JWT = strstr(response, "token");
            	if (JWT != NULL) {
            	JWT = strtok(JWT + 8, "}");
            }
            	JWT[strlen(JWT) - 1] = 0;
            	printf("Token: %s\n", JWT);
            	printf("%s\n", response);

            }
            close_connection(sockfd);
             
        	
        } else if (!strcmp(input, "get_books\n")) {
        	sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        	char cookieHeader[BUFLEN + 8];
        	char JWTHeader[BUFLEN + 22];
        	if (cookie != NULL) {
        		sprintf(cookieHeader, "Cookie: %s", cookie);
        	} else {
        		sprintf(cookieHeader, "Cookie: ");
        	}
        	if (JWT != NULL) {
        		sprintf(JWTHeader, "Authorization: Bearer %s", JWT);
        	} else {
        		sprintf(JWTHeader, "Authorization: Bearer ");
        	}
        	buff = compute_get_request(host, "/api/v1/tema/library/books", cookieHeader, JWTHeader);
        	printf("%s\n", buff);
        	send_to_server(sockfd, buff);
        	response = receive_from_server(sockfd);
        	printf("%s\n", response);
        	close_connection(sockfd);

        }else if (!strcmp(input, "get_book\n")) {
        	sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        	char cookieHeader[BUFLEN + 8];
        	char JWTHeader[BUFLEN + 22];
        	char id[MAXIDSIZE];
        	printf("id=");
            fgets(id, 15, stdin);
            id[strlen(id) - 1] = 0;
        	if (cookie != NULL) {
        		sprintf(cookieHeader, "Cookie: %s", cookie);
        	} else {
        		sprintf(cookieHeader, "Cookie: ");
        	}
        	if (JWT != NULL) {
        		sprintf(JWTHeader, "Authorization: Bearer %s", JWT);
        	} else {
        		sprintf(JWTHeader, "Authorization: Bearer ");
        	}
        	char path[28 + MAXIDSIZE] = "/api/v1/tema/library/books/";
        	strcat(path, id);
        	buff = compute_get_request(host, path, cookieHeader, JWTHeader);
        	printf("%s\n", buff);
        	send_to_server(sockfd, buff);
        	response = receive_from_server(sockfd);
        	printf("%s\n", response);
        	close_connection(sockfd);


        }else if (!strcmp(input,"add_book\n")) {
        	sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        	char title[MAXBOOKCREDSIZE];
        	char author[MAXBOOKCREDSIZE];
        	char genre[MAXBOOKCREDSIZE];
        	char publisher[MAXBOOKCREDSIZE];
        	int pagecount;
        	char cookieHeader[BUFLEN + 8];
        	char JWTHeader[BUFLEN + 22];
        	printf("title=");
        	fgets(title, MAXBOOKCREDSIZE, stdin);
        	title[strlen(title) - 1] = 0;
        	printf("author=");
        	fgets(author, MAXBOOKCREDSIZE, stdin);
        	author[strlen(author) - 1] = 0;
        	printf("genre=");
        	fgets(genre, MAXBOOKCREDSIZE, stdin);
        	genre[strlen(genre) - 1] = 0;
        	printf("publisher=");
        	fgets(publisher, MAXBOOKCREDSIZE, stdin);
        	publisher[strlen(publisher) - 1] = 0;
        	printf("pagecount=");
        	scanf("%d", &pagecount);
         
            JSON_Value *value = json_value_init_object();
            JSON_Object *object = json_value_get_object(value);
            char *payload = NULL;
            json_object_set_string(object, "title", title);
            json_object_set_string(object, "author", author);
            json_object_set_string(object, "genre", genre);
            json_object_set_number(object, "page_count", pagecount);
            json_object_set_string(object, "publisher", publisher);
            payload = json_serialize_to_string_pretty(value);


        	if (cookie != NULL) {
        		sprintf(cookieHeader, "Cookie: %s", cookie);
        	} else {
        		sprintf(cookieHeader, "Cookie: ");
        	}
        	if (JWT != NULL) {
        		sprintf(JWTHeader, "Authorization: Bearer %s", JWT);
        	} else {
        		sprintf(JWTHeader, "Authorization: Bearer ");
        	}
        	buff = compute_post_request(host, "/api/v1/tema/library/books", TYPE, payload, cookieHeader, JWTHeader);
        	printf("%s\n", buff);
        	send_to_server(sockfd, buff);
        	response  = receive_from_server(sockfd);
        	printf("%s\n", response);
        	close_connection(sockfd);
        }else if (!strcmp(input, "delete_book\n")) {
        	sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        	char cookieHeader[BUFLEN + 8];
        	char JWTHeader[BUFLEN + 22];
        	char id[MAXIDSIZE];
        	printf("id=");
        	fgets(id, MAXIDSIZE, stdin);
        	id[strlen(id) - 1] = 0;
        	if (cookie != NULL) {
        		sprintf(cookieHeader, "Cookie: %s", cookie);
        	} else {
        		sprintf(cookieHeader, "Cookie: ");
        	}
        	if (JWT != NULL) {
        		sprintf(JWTHeader, "Authorization: Bearer %s", JWT);
        	} else {
        		sprintf(JWTHeader, "Authorization: Bearer ");
        	}
        	char path[28 + MAXIDSIZE] = "/api/v1/tema/library/books/";
        	strcat(path, id);
        	buff = compute_delete_request(host, path, cookieHeader, JWTHeader);
        	printf("%s\n", buff);
        	send_to_server(sockfd, buff);
        	response = receive_from_server(sockfd);
        	printf("%s\n", response);
        	close_connection(sockfd);


        } else if (!strcmp(input, "logout\n")) {
        	sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        	 char cookieHeader[BUFLEN + 8];
      	    if (cookie != NULL) {
      	    	sprintf(cookieHeader, "Cookie: %s", cookie);
      	    } else {
      	    	sprintf(cookieHeader, "Cookie: ");
      	    }

      	    buff = compute_get_request(host, "/api/v1/tema/auth/logout", cookieHeader, NULL);
      	    printf("%s\n", buff);
      	    send_to_server(sockfd, buff);
      	    response = receive_from_server(sockfd);
      	    printf("%s\n", response);
      	    cookie = NULL;
      	    JWT = NULL;
      	    close_connection(sockfd);

        }


        }
 return 0;
}
        
  
    
