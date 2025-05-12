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
#include <ctype.h>

char *login_admin(char *message, char *response){
    char *cookie_login = NULL;
    JSON_Value *val = json_value_init_object();
    JSON_Object *obj = json_value_get_object(val);

    char *username = calloc(BUFLEN, sizeof(char));
    char *password = calloc(BUFLEN, sizeof(char));
    printf("username="); 
    scanf("%s", username);

    printf("password=");
    scanf("%s", password);
    json_object_set_string(obj, "username", username);
    json_object_set_string(obj, "password", password);
    
    char *string = json_serialize_to_string(val);
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (cookie_login == NULL)
        message = compute_post_request("63.32.125.183:8081", "/api/v1/tema/admin/login", "application/json", &string, 1, NULL, 0, NULL);
    else
        message = compute_post_request("63.32.125.183:8081", "/api/v1/tema/admin/login", "application/json", &string, 1, &cookie_login, 1, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "Admin logged in successfully")){
        printf("SUCCESS: Admin autentificat cu succes\n");
        cookie_login = strtok(strstr(response, "session"), ";");
    }
    else if (strstr(response, "Invalid credentials"))
        printf("ERROR: Credentialele nu se potrivesc\n");
    else
        printf("ERROR: Reautentificare cand deja logat\n");

    json_free_serialized_string(string);
    json_value_free(val);
    free(username);
    free(password);
    close_connection(sockfd);
    return cookie_login;
}

void add_user(char *message, char *response, char *cookie_login){
    JSON_Value *val = json_value_init_object();
    JSON_Object *obj = json_value_get_object(val);

    char *username = calloc(BUFLEN, sizeof(char));
    char *password = calloc(BUFLEN, sizeof(char));
    printf("username="); 
    scanf("%s", username);
    int c = getchar();
    if (isspace(c) && c != '\n'){
        printf("ERROR: Informatii incomplete/incorecte.\n");
        free(username);
        free(password);
        return;
    }
    printf("password=");
    scanf("%s", password);
    c = getchar();
    if (isspace(c) && c != '\n'){
        printf("ERROR: Informatii incomplete/incorecte.\n");
        free(username);
        free(password);
        return;
    }
    json_object_set_string(obj, "username", username);
    json_object_set_string(obj, "password", password);
    
    char *string = json_serialize_to_string(val);
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (cookie_login == NULL)
        message = compute_post_request("63.32.125.183:8081", "/api/v1/tema/admin/users", "application/json", &string, 1, NULL, 0, NULL);
    else
        message = compute_post_request("63.32.125.183:8081", "/api/v1/tema/admin/users", "application/json", &string, 1, &cookie_login, 1, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "User created successfully"))
        printf("SUCCESS: Utilizator adaugat cu succes\n");
    else if (strstr(response, "Admin privileges required"))
        printf("ERROR: Lipsa rol admin\n");
    else
        printf("ERROR: Acest user deja exista\n");

    json_free_serialized_string(string);
    json_value_free(val);
    free(username);
    free(password);
    close_connection(sockfd);

}

void get_users(char *message, char *response, char *cookie_login){
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (cookie_login == NULL)
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/admin/users", NULL, NULL, 0, NULL);
    else
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/admin/users", NULL, &cookie_login, 1, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strstr(response, "200 OK")){
        printf("SUCCESS: Lista utilizatorilor\n");
        JSON_Value *root_value = json_parse_string(strchr(response, '{'));
        JSON_Object *root_obj = json_value_get_object(root_value);
        JSON_Array  *users = json_object_get_array(root_obj, "users");
        int number_users = json_array_get_count(users);
        for (int i = 0; i < number_users ; i++) {
            JSON_Object *user_obj = json_array_get_object(users, i);
            const char *username = json_object_get_string(user_obj, "username");
            const char *password = json_object_get_string(user_obj, "password");
            printf("#%d %s:%s\n", (i + 1), username, password);
        }
    }
    else
        printf("ERROR: Lipsa rol admin\n");
    close_connection(sockfd);

}

void delete_user(char *message, char *response, char *cookie_login){
    printf("username=");
    char *username = calloc(BUFLEN, sizeof(char));
    scanf("%s", username);
    int c = getchar();
    if (isspace(c) && c != '\n'){
        printf("ERROR: Informatii incomplete/incorecte.\n");
        free(username);
        return;
    }
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    char *url = calloc(BUFLEN, sizeof(char));
    strcpy(url, "/api/v1/tema/admin/users/"); 
    strcat(url, username);
    if (cookie_login == NULL)
        message = compute_delete_request("63.32.125.183:8081", url, NULL, NULL, 0, NULL);
    else
        message = compute_delete_request("63.32.125.183:8081", url, NULL, &cookie_login, 1, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "deleted successfully"))
        printf("SUCCESS: Utilizator sters\n");
    else if (strstr(response, "Admin privileges required"))
        printf("ERROR: Lipsa rol admin\n");
    else
        printf("ERROR: Username invalid\n");
    free(username);
    free(url);
    close_connection(sockfd);

}


void logout_admin(char *message, char *response, char *cookie_login){
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (cookie_login == NULL)
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/admin/logout", NULL, NULL, 0, NULL);
    else
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/admin/logout", NULL, &cookie_login, 1, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "Admin logged out successfully"))
        printf("SUCCESS: Admin delogat\n");
    else
        printf("ERROR: Neautentificat\n");
    close_connection(sockfd);

}

char *login(char *message, char *response, char *cookie_login){
    JSON_Value *val = json_value_init_object();
    JSON_Object *obj = json_value_get_object(val);

    char *admin_username = calloc(BUFLEN, sizeof(char));
    char *username = calloc(BUFLEN, sizeof(char));
    char *password = calloc(BUFLEN, sizeof(char));
    printf("admin_username="); 
    scanf("%s", admin_username);
    int c = getchar();
    if (isspace(c) && c != '\n'){
        printf("ERROR: Informatii incomplete/incorecte.\n");
        free(admin_username);
        free(username);
        free(password);
        return cookie_login;
    }
    printf("username="); 
    scanf("%s", username);
    c = getchar();
    if (isspace(c) && c != '\n'){
        printf("ERROR: Informatii incomplete/incorecte.\n");
        free(admin_username);
        free(username);
        free(password);
        return cookie_login;
    }
    printf("password=");
    scanf("%s", password);
    c = getchar();
    if (isspace(c) && c != '\n'){
        printf("ERROR: Informatii incomplete/incorecte.\n");
        free(admin_username);
        free(username);
        free(password);
        return cookie_login;
    }
    json_object_set_string(obj, "admin_username", admin_username);
    json_object_set_string(obj, "username", username);
    json_object_set_string(obj, "password", password);
    
    char *string = json_serialize_to_string(val);
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (cookie_login == NULL)
        message = compute_post_request("63.32.125.183:8081", "/api/v1/tema/user/login", "application/json", &string, 1, NULL, 0, NULL);
    else
        message = compute_post_request("63.32.125.183:8081", "/api/v1/tema/user/login", "application/json", &string, 1, &cookie_login, 1, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "User logged in successfully")){
        printf("SUCCESS: Autentificare reusita\n");
        cookie_login = strtok(strstr(response, "session"), ";");
    }
    else if (strstr(response, "Invalid credentials") || strstr(response, "Admin user not found"))
        printf("ERROR: Credentialele nu se potrivesc\n");
    else
        printf("ERROR: Reautentificare\n");

    json_free_serialized_string(string);
    json_value_free(val);
    free(username);
    free(password);
    free(admin_username);
    close_connection(sockfd);
    return cookie_login;
}

int logout(char *message, char *response, char *cookie_login){
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (cookie_login == NULL)
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/user/logout", NULL, NULL, 0, NULL);
    else
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/user/logout", NULL, &cookie_login, 1, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "User logged out successfully")){
        printf("SUCCESS: Utilizator delogat\n");
        return 0;
    }
    else{
        printf("ERROR: Neautentificat\n");
        return -1;
    }
    close_connection(sockfd);

}

char *get_access(char *message, char *response, char *cookie_login){
    char *token;
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (cookie_login == NULL)
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/library/access", NULL, NULL, 0, NULL);
    else
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/library/access", NULL, &cookie_login, 1, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "200 OK")){
        printf("SUCCESS: Token JWT primit\n");
        token = strtok(strstr(response, "token") + 8, "\"");
    }
    else
        printf("ERROR: Neautentificat\n");
    close_connection(sockfd);
    return token;
}

void get_movies(char *message, char *response, char *cookie_login, char *token){
    if (cookie_login == NULL){
        printf("ERROR: Fara acces library\n");
        return;
    }
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (token == NULL)
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/library/movies", NULL, &cookie_login, 1, NULL);
    else
        message = compute_get_request("63.32.125.183:8081", "/api/v1/tema/library/movies", NULL, &cookie_login, 1, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "movies")){
        printf("SUCCESS: Lista filmelor\n");
        JSON_Value *root_value = json_parse_string(strchr(response, '{'));
        JSON_Object *root_obj = json_value_get_object(root_value);
        JSON_Array  *movies = json_object_get_array(root_obj, "movies");
        int number_movies = json_array_get_count(movies);
        for (int i = 0; i < number_movies; i++) {
            JSON_Object *user_obj = json_array_get_object(movies, i);
            const char *title = json_object_get_string(user_obj, "title");
            const int id = (int)json_object_get_number(user_obj, "id");
            printf("#%d %s\n", id, title);
        }
    }
    else
        printf("ERROR: Fara acces library\n");
    close_connection(sockfd);
}

void add_movie(char *message, char *response, char *cookie_login, char *token){
    if (cookie_login == NULL){
        printf("ERROR: Fara acces library\n");
        return;
    }
    JSON_Value *val = json_value_init_object();
    JSON_Object *obj = json_value_get_object(val);

    char *title = calloc(BUFLEN, sizeof(char));
    int year;
    char *description = calloc(BUFLEN, sizeof(char));
    double rating;
    int c;
    while ((c = getchar()) != '\n');  
    printf("title=");
    fgets(title, BUFLEN, stdin);
    title[strcspn(title, "\n")] = '\0'; 
    printf("year="); 
    int rc = scanf("%d", &year);
    if (rc == 0 || year < 0){
        printf("ERROR: Date invalide/incomplete\n");
        free(title);
        free(description);
        return;
    }
    while (getchar() != '\n');
    printf("description=");
    fgets(description, BUFLEN, stdin);
    description[strcspn(description, "\n")] = '\0';
    printf("rating=");
    rc = scanf("%lf", &rating);
    if (rc == 0 || rating < 0){
        printf("ERROR: Date invalide/incomplete\n");
        free(title);
        free(description);
        return;
    }
    while (getchar() != '\n');
    json_object_set_string(obj, "title", title);
    json_object_set_number(obj, "year", year);
    json_object_set_string(obj, "description", description);
    json_object_set_number(obj, "rating", rating);
    
    char *string = json_serialize_to_string(val);
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (token == NULL)
        message = compute_post_request("63.32.125.183:8081", "/api/v1/tema/library/movies", "application/json", &string, 1, &cookie_login, 1, NULL);
    else
        message = compute_post_request("63.32.125.183:8081", "/api/v1/tema/library/movies", "application/json", &string, 1, &cookie_login, 1, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "201 CREATED"))
        printf("SUCCESS: Film adaugat\n");
    else if (strstr(response, "Rating must be between 0 and 9.9")){
        printf("ERROR: Date invalide/incomplete\n");
    }
    else
        printf("ERROR: Fara acces library\n");

    json_free_serialized_string(string);
    json_value_free(val);
    free(title);
    free(description);
    close_connection(sockfd);
}

void get_movie(char *message, char *response, char *cookie_login, char *token){
    if (cookie_login == NULL){
        printf("ERROR: Fara acces library\n");
        return;
    }
    char *id = calloc(BUFLEN, sizeof(char));
    printf("id=");
    scanf("%s",id);
    char *url = calloc(BUFLEN, sizeof(char));
    strcpy(url, "/api/v1/tema/library/movies/");
    strcat(url, id);
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (token == NULL)
        message = compute_get_request("63.32.125.183:8081", url, NULL, &cookie_login, 1, NULL);
    else
        message = compute_get_request("63.32.125.183:8081", url, NULL, &cookie_login, 1, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "200 OK")){
        printf("SUCCESS: Filmul dat are urmatoarele informatii:\n");
        JSON_Value *root_value = json_parse_string(strchr(response, '{'));
        JSON_Object *root_obj = json_value_get_object(root_value);
        const char *title = json_object_get_string(root_obj, "title");
        const char *description = json_object_get_string(root_obj, "description");
        const char * rating = json_object_get_string(root_obj, "rating");
        const int year = (int)json_object_get_number(root_obj, "year");
        printf("Title: %s\n", title);
        printf("Year: %d\n", year);
        printf("Description: %s\n", description);
        printf("Rating: %lf\n", atof(rating));
    }
    else if (strstr(response, "Movie not found")){
        printf("ERROR: ID invalid\n");
    }
    else
        printf("ERROR: Fara acces library\n");
    free(url);
    free(id);
    close_connection(sockfd);
}


void delete_movie(char *message, char *response, char *cookie_login, char *token){
    if (cookie_login == NULL){
        printf("ERROR: Fara acces library\n");
        return;
    }
    char *id = calloc(BUFLEN, sizeof(char));
    printf("id=");
    scanf("%s",id);
    char *url = calloc(BUFLEN, sizeof(char));
    strcpy(url, "/api/v1/tema/library/movies/");
    strcat(url, id);
    int sockfd = open_connection("63.32.125.183", 8081, AF_INET, SOCK_STREAM, 0);
    if (token == NULL)
        message = compute_delete_request("63.32.125.183:8081", url, NULL, &cookie_login, 1, NULL);
    else
        message = compute_delete_request("63.32.125.183:8081", url, NULL, &cookie_login, 1, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    if (strstr(response, "deleted successfully")){
        printf("SUCCESS: Film sters cu succes\n");
    }
    else if (strstr(response, "Invalid movie id")){
        printf("ERROR: ID invalid\n");
    }
    else
        printf("ERROR: Fara acces library\n");
    free(url);
    free(id);
    close_connection(sockfd);
}



int main(int argc, char *argv[])
{
    char *command = calloc(BUFLEN, sizeof(char));
    char *message = calloc(BUFLEN, sizeof(char));
    char *response = calloc(BUFLEN, sizeof(char));
    char *cookie_login_admin = NULL;
    char *cookie_login = NULL;
    char *token = NULL;
    do {
        scanf("%s", command);
        if (!strncmp(command, "login_admin", 11)){
            cookie_login_admin = login_admin(message, response);
        }
        else if (!strncmp(command, "add_user", 8)){
            add_user(message, response, cookie_login_admin);
        }
        else if (!strncmp(command, "get_users", 9)){
            get_users(message, response, cookie_login_admin);
        }
        else if (!strncmp(command, "delete_user", 11)){
            delete_user(message, response, cookie_login_admin);
        }
        else if (!strncmp(command, "logout_admin", 12)){
            logout_admin(message, response, cookie_login_admin);
        }
        else if (!strncmp(command, "login", 5)){
            cookie_login = login(message, response, cookie_login);
        }
        else if (!strncmp(command, "logout", 6)){
            if (logout(message, response, cookie_login) == 0)
                token = NULL;
        }
        else if (!strncmp(command, "get_access", 10)){
            token = get_access(message, response, cookie_login);
        }
        else if (!strncmp(command, "get_movies", 10)){
            get_movies(message, response, cookie_login, token);
        }
        else if (!strncmp(command, "add_movie", 9)){
            add_movie(message, response, cookie_login, token);
        }
        else if (!strncmp(command, "get_movie", 9)){
            get_movie(message, response, cookie_login, token);
        }
        else if (!strncmp(command, "delete_movie", 12)){
            delete_movie(message, response, cookie_login, token);
        }
    } while (strncmp(command, "exit", 4));

    free(message);
    free(response);
    free(command);
    return 0;
}
