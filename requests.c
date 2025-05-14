#include "requests.h"
#include "helpers.h"
#include <arpa/inet.h>
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdio.h>
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */

char *compute_get_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count, char *token) {
  char *message = calloc(BUFLEN, sizeof(char));
  char *line = calloc(LINELEN, sizeof(char));

  if (query_params != NULL) {
    sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
  } else {
    sprintf(line, "GET %s HTTP/1.1", url);
  }

  compute_message(message, line);

  sprintf(line, "Host: %s", host);
  compute_message(message, line);

  if (cookies != NULL) {
    memset(line, 0, LINELEN);
    strcat(line, "Cookie: ");
    int i = 0;
    while (i < cookies_count - 1) {
      strcat(line, cookies[i]);
      strcat(line, "; ");
      i++;
    }
    strcat(line, cookies[i]);
    compute_message(message, line);
  }
  if (token != NULL) {
    sprintf(line, "Authorization: Bearer %s", token);
    compute_message(message, line);
  }
  compute_message(message, "");
  free(line);
  return message;
}

char *compute_post_request(char *host, char *url, char *content_type,
                           char **body_data, int body_data_fields_count,
                           char **cookies, int cookies_count, char *token) {
  char *message = calloc(BUFLEN, sizeof(char));
  char *line = calloc(LINELEN, sizeof(char));
  char *body_data_buffer = calloc(LINELEN, sizeof(char));

  sprintf(line, "POST %s HTTP/1.1", url);
  compute_message(message, line);

  sprintf(line, "Host: %s", host);
  compute_message(message, line);

  sprintf(line, "Content-Type: %s", content_type);
  compute_message(message, line);
  int i = 0;
  while (i < body_data_fields_count - 1) {
    strcat(body_data_buffer, body_data[i]);
    strcat(body_data_buffer, "&");
    i++;
  }
  strcat(body_data_buffer, body_data[i]);
  sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
  compute_message(message, line);

  if (cookies != NULL) {
    memset(line, 0, LINELEN);
    strcat(line, "Cookie: ");
    int i = 0;
    while (i < cookies_count - 1) {
      strcat(line, cookies[i]);
      strcat(line, "; ");
      i++;
    }
    strcat(line, cookies[i]);
    compute_message(message, line);
  }
  if (token != NULL) {
    sprintf(line, "Authorization: Bearer %s", token);
    compute_message(message, line);
  }

  compute_message(message, "");

  memset(line, 0, LINELEN);
  strcat(message, body_data_buffer);

  free(body_data_buffer);
  free(line);
  return message;
}

char *compute_delete_request(char *host, char *url, char *query_params,
                             char **cookies, int cookies_count, char *token) {
  char *message = calloc(BUFLEN, sizeof(char));
  char *line = calloc(LINELEN, sizeof(char));

  if (query_params != NULL) {
    sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
  } else {
    sprintf(line, "DELETE %s HTTP/1.1", url);
  }

  compute_message(message, line);

  sprintf(line, "Host: %s", host);
  compute_message(message, line);

  if (cookies != NULL) {
    memset(line, 0, LINELEN);
    strcat(line, "Cookie: ");
    int i = 0;
    while (i < cookies_count - 1) {
      strcat(line, cookies[i]);
      strcat(line, "; ");
      i++;
    }
    strcat(line, cookies[i]);
    compute_message(message, line);
  }
  if (token != NULL) {
    sprintf(line, "Authorization: Bearer %s", token);
    compute_message(message, line);
  }

  compute_message(message, "");
  free(line);
  return message;
}

char *compute_put_request(char *host, char *url, char *content_type,
                          char **body_data, int body_data_fields_count,
                          char **cookies, int cookies_count, char *token) {
  char *message = calloc(BUFLEN, sizeof(char));
  char *line = calloc(LINELEN, sizeof(char));
  char *body_data_buffer = calloc(LINELEN, sizeof(char));

  sprintf(line, "PUT %s HTTP/1.1", url);
  compute_message(message, line);

  sprintf(line, "Host: %s", host);
  compute_message(message, line);

  sprintf(line, "Content-Type: %s", content_type);
  compute_message(message, line);
  int i = 0;
  while (i < body_data_fields_count - 1) {
    strcat(body_data_buffer, body_data[i]);
    strcat(body_data_buffer, "&");
    i++;
  }
  strcat(body_data_buffer, body_data[i]);
  sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
  compute_message(message, line);

  if (cookies != NULL) {
    memset(line, 0, LINELEN);
    strcat(line, "Cookie: ");
    int i = 0;
    while (i < cookies_count - 1) {
      strcat(line, cookies[i]);
      strcat(line, "; ");
      i++;
    }
    strcat(line, cookies[i]);
    compute_message(message, line);
  }
  if (token != NULL) {
    sprintf(line, "Authorization: Bearer %s", token);
    compute_message(message, line);
  }

  compute_message(message, "");

  memset(line, 0, LINELEN);
  strcat(message, body_data_buffer);

  free(body_data_buffer);
  free(line);
  return message;
}