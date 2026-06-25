# Movie Library HTTP Client

A C/C++ command-line client for interacting with a REST API over HTTP. The project simulates a small online movie library where an admin can manage users, while regular users can access and manage movies and collections.

The client communicates directly with the server using sockets, builds HTTP requests manually, sends JSON payloads when needed, and parses the server responses.

## Overview

The application reads commands from standard input and turns them into HTTP requests. Depending on the command, it may send authentication data, request a list of resources, create new entries, update existing ones, or delete items from the library.

The project covers two main roles:

* **Admin** - logs in, creates users, lists users, deletes users, and logs out.
* **User** - logs in, requests library access, manages movies, manages collections, and logs out.

Admin users only handle normal user accounts. Movie and collection operations are available through regular user sessions after receiving access to the library.

## Main Features

* admin login and logout;
* add, list, and delete normal users;
* regular user login and logout;
* request access to the movie library;
* list all movies and view details for one movie;
* add, update, and delete movies;
* list collections and view collection details;
* create and delete collections;
* add or remove movies from a collection;
* parse JSON responses and build JSON request bodies.

## Authentication and Access

The client keeps track of the session cookie returned after login. For library operations, it also stores the JWT token received after requesting access.

The cookie is used to prove that the client is authenticated, while the JWT token is sent in the `Authorization: Bearer` header for protected library routes.

## HTTP Communication

All communication is done through raw TCP sockets. The client manually builds requests with the correct method, route, headers, body, cookies, and authorization token.

Responses are parsed to detect success or error states and to extract useful data such as cookies, tokens, users, movies, and collections.

## Input Format

Commands and their fields are entered on separate lines. Example:

```text id="3w8ftb"
login
admin_username=admin
username=test
password=test
```

For movie or collection data, values such as titles and descriptions may contain spaces, so input parsing has to preserve the full value after the field name.

## Notes

The project focuses on understanding how web clients work below higher-level libraries. It combines HTTP message formatting, JSON handling, session cookies, JWT authorization, and command-line interaction into one small client.

The implementation is kept modular so request building, response parsing, authentication state, and command handling remain easy to follow.
