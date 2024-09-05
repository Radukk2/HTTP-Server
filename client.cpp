#include <string>
#include <iostream>
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <vector>
#include "helpers.h"
#include "requests.hpp"
#include <cctype>
#include "json.hpp"

using namespace std;

char myAddr[] = "34.246.184.49";

void register_user(string username, string password, string jsonString, int sockfd) {
    string url = "/api/v1/tema/auth/register";
    string content_type = "application/json";
    nlohmann::json json_object;
    cout<<"username=";
    getline(cin, username);
    cout<<"password=";
    getline(cin, password);
    if (username == "" || password == ""  || username.find(' ') != string::npos || password.find(' ') != string::npos) {
        cout << "Error: invalid credentials";
        return;
    }
    cout<<"\n";
    json_object["username"] = username;
    json_object["password"] = password;
    jsonString = json_object.dump();
    char *myString = compute_post_request(myAddr, url.c_str(), content_type.c_str(), jsonString, nullptr, 0, NULL);
    sockfd = open_connection(myAddr, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, myString);
    char *response = receive_from_server(sockfd);
    close(sockfd);
    if (strstr(response, "is taken!"))
        cout << "Erorr : User already exists.\n";
    else
        cout << "Succesfuly added user\n";
}


void login_user(bool &loggedIn, string username, string password, int sockfd, string jsonString, vector<char *> &allcookies) {
    string url = "/api/v1/tema/auth/login";
    string content_type = "application/json";
    nlohmann::json json_object;
    cout<<"username=";
    getline(cin, username);
    cout<<"password=";
    getline(cin, password);
    cout<<"\n";
    if (loggedIn) {
        cout << "Error: You are logged in already\n";
        return;
    }
    if (username == "" || password == ""  || username.find(' ') != string::npos || password.find(' ') != string::npos) {
        cout << "Error: invalid credentials";
        return;
    }
    json_object["username"] = username;
    json_object["password"] = password;
    jsonString = json_object.dump();

    sockfd = open_connection(myAddr, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    char *myString = compute_post_request(myAddr, url.c_str(), content_type.c_str(), jsonString, nullptr, 0, NULL);
    send_to_server(sockfd, myString);
    char *response = receive_from_server(sockfd);
    close(sockfd);
    bool accept = false;
    if (strstr(response, "No account with this username!"))
        cout << "Error: No account with this username!\n";
    else if (strstr(response, "Credentials are not good!"))
        cout << "Error: Credentials are not good!\n";
    else  {
        cout << "Succesfully logged in!\n";
        accept = true;
    }
    if (accept) {
        char *copy = strstr(response, "Set-Cookie:");
        copy = copy + 12;
        char *cookies = strtok(copy, "\n");
        char *firstCookie = strtok(cookies, " ;");
        allcookies.push_back(firstCookie);
        loggedIn = true;
    } 
}

void get_books(bool &loggedIn, vector<char *> &allcookies, int sockfd, char *my_token, bool &isToken) {
    if (!loggedIn) {
        cout << "Error : You have to login first\n";
        return;
    }
    if (!isToken) {
        cout << "Error: Access denied!\n";
        return;
    }
    string url = "/api/v1/tema/library/books";
    char *myString = compute_get_request(myAddr, url.c_str(), NULL, allcookies, allcookies.size(), my_token);
    sockfd = open_connection(myAddr, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, myString);
    char *response = receive_from_server(sockfd);
    close(sockfd);
    char *books = strchr(response, '[');
    books++;
    char *tok = strtok(books, "]");
    books--;
    strcat(books, "]");
    if (tok) {
        cout << "[\n";
        nlohmann::json jsonArray = nlohmann::json::parse(books);
        if (jsonArray.is_array()) {
            for (const auto& jsonObject : jsonArray) {
                cout << jsonObject.dump(4) << "\n";
            }
        }
        cout << "]";
    } else
        cout << "[]\n";
}

void get_book(bool &loggedIn, bool &isToken, vector<char *> allcookies, char * my_token, int sockfd) {
    string url = "/api/v1/tema/library/books/";
    string id;
    cout << "id=";
    getline(cin, id);
    url += id;
    if (!loggedIn) {
        cout << "Error: You have to login first\n";
        return;
    }
    if (!isToken) {
        cout << "Error: Access denied!\n";
        return;
    }
    char *myString = compute_get_request(myAddr, url.c_str(), NULL, allcookies, allcookies.size(), my_token);
    sockfd = open_connection(myAddr, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, myString);
    char *response = receive_from_server(sockfd);
    close(sockfd);
    if (strstr(strchr(response, '{'), "No book was found!")) {
        cout << "Error: No book was found!\n";
        return;
    }
    cout << strchr(response, '{') << "\n";
}

void add_book(string title, string author, string publisher, string genre,
        string page_count, bool &loggedIn, bool &isToken, int sockfd, string jsonString, char *my_token) {
    string url = "/api/v1/tema/library/books";
    string content_type = "application/json";
    nlohmann::json json_object;
    cout<<"\ntitle=";
    getline(cin, title);
    cout<<"\nauthor=";
    getline(cin, author);
    cout << "\npublisher=";
    getline(cin, publisher);
    cout << "\ngenre=";
    getline(cin, genre);
    cout << "\npage_count=";
    getline(cin, page_count);
    cout << "\n";
    if (!loggedIn) {
        cout << "Error: You have to login first\n";
        return;
    }
    if (!isToken) {
        cout << "Error: Access denied!\n";
        return;
    }
    if (title == "" || author == "" || publisher == "" || genre == "") {
        cout << "Error: Invalid credentials";
        return;
    }
    bool ok = false;
    for (long unsigned int i = 0; i < page_count.size(); i++) {
        if (!isdigit(page_count[i])) {
            cout << "Error: Invalid credentials";
            ok = true;
            break;
        }
    }
    if (ok)
        return;
    json_object["title"] = title;
    json_object["author"] = author;
    json_object["genre"] = genre;
    json_object["publisher"] = publisher;
    json_object["page_count"] = page_count;
    jsonString = json_object.dump();
    char *myString = compute_post_request(myAddr, url.c_str(), content_type.c_str(), jsonString, nullptr, 0, my_token);
    sockfd = open_connection(myAddr, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, myString);
    char *response = receive_from_server(sockfd);
    close(sockfd);
    if (!response)
        exit(0);
    cout << "Succesfully added book\n";
}

void delete_book(bool &loggedIn, bool &isToken, char *my_token, int sockfd) {
    if (!loggedIn) {
        cout << "Error: You have to login first\n";
        return;
    }
    if (!isToken) {
        cout << "Error: Access denied!\n";
        return;
    }
    string url = "/api/v1/tema/library/books/";
    string id;
    cout << "id=";
    getline(cin, id);
    url += id;
    char *myString = compute_delete_book(myAddr,url.c_str(), my_token);
    sockfd = open_connection(myAddr, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, myString);
    char *response = receive_from_server(sockfd);
    close(sockfd);
    if (strstr(response, "No book was deleted!")) {
        cout << "Error: No book with the given ID\n";
    } else {
        cout << "Succesfully deleted the book\n";
    }
}

int main(int argc, char *argv[])
{
    vector<char *> allcookies;
    bool loggedIn = false, isToken = false;
    while (1) {
        string s1;
        string username, password, jsonString, id, title, author, publisher, genre, page_count;
        char my_token[1000000];
        getline(cin, s1);
        int sockfd;
        if (s1 == "exit") {
            break;
        } else if (s1 == "register") {
            register_user(username,password,jsonString,sockfd);
        } else if (s1 == "login") {
            login_user(loggedIn, username,password,sockfd,jsonString, allcookies);
        } else if (s1 == "enter_library") {
            if (!loggedIn) {
                cout << "Error: You have to login first\n";
                continue;
            }
            string url = "/api/v1/tema/library/access";
            char *myString = compute_get_request(myAddr, url.c_str(), NULL, allcookies, allcookies.size(), NULL);
            sockfd = open_connection(myAddr, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
            send_to_server(sockfd, myString);
            char *response = receive_from_server(sockfd);
            close(sockfd);
            char *token_project = strstr(response, "token");
            token_project += 8;
            char *token = strtok(token_project, "\"");
            strcpy(my_token, token);
            isToken = true;
            cout << "Succesfully entered library\n";
        } else if (s1 ==  "get_books") {
            get_books(loggedIn, allcookies, sockfd, my_token, isToken);
        } else if (s1 == "get_book") {
            get_book(loggedIn, isToken, allcookies, my_token, sockfd);
        } else if (s1 == "add_book") {
            add_book(title, author, publisher, genre, page_count, loggedIn, isToken, sockfd, jsonString, my_token);
        } else if (s1 == "delete_book") {
            delete_book(loggedIn, isToken, my_token, sockfd);
        } else if (s1 == "logout") {
            if (!loggedIn) {
                cout << "Error : You cannot log out before logging in\n";
                continue;
            }
            string url = "/api/v1/tema/auth/logout";
            char *myString = compute_get_request(myAddr, url.c_str(), NULL, allcookies, allcookies.size(), NULL);
            sockfd = open_connection(myAddr, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
            send_to_server(sockfd, myString);
            char *response = receive_from_server(sockfd);
            close(sockfd);
            if (!response)
                exit(0);
            cout << "Succesfully logged out\n";
            isToken = false;
            loggedIn = false;
            allcookies.clear();
        }
    }
    return 0;
}
