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
#include <unordered_map>
#include <nlohmann/json.hpp>

using namespace std;
using json=nlohmann::json;

// clasa pentru stocarea caracteristicilor unei carti
class Carte {
public:
    string titlu;
    string autor;
    string gen;
    string nrPagini;
    string editor;
    json data;

    // constructor
    Carte(char *titlu, char *autor, char *gen, char *nrPagini, char* editor) {
        this->titlu = string(titlu);
        this->autor = string(autor);
        this->gen = string(gen);
        this->nrPagini = string(nrPagini);
        this->editor = string(editor);
    }

    // construieste jsonul cu datele cartii
    void makeJson() {
        this->data = {{"title", this->titlu}, 
                      {"author", this->autor},
                      {"genre", this->gen},
                      {"page_count", this->nrPagini},
                      {"publisher", this->editor}};
    }
};

// verifica daca un input este de tip numeric
bool valid(char* id) {
    for (unsigned int i = 0; i < strlen(id); i++) {
        if (id[i] != '0' && id[i] != '1' && id[i] != '2' && id[i] != '3' && id[i] != '4' && id[i] != '5' && id[i] != '6' &&
        id[i] != '7' && id[i] != '8' && id[i] != '9') {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;
    char username[2000], password[2000];
    string user, parola;
    json data;
    json acces = NULL;
    string cookie;
    string raspuns;
    string autorizatie;
    string adresa;
    char id[2000];
    string numar;
    char titlu[2000], autor[2000], gen[2000], nrPagini[2000], editor[2000];
    char *cookie_start;
    char *cookie_end;

    // pentru switch-case ---> mai elegant decat cascade if-else
    unordered_map<string, int> comenzi {{"register", 1},
                                        {"login", 2},
                                        {"logout", 3},
                                        {"enter_library", 4},
                                        {"get_books", 5},
                                        {"get_book", 6},
                                        {"delete_book", 7},
                                        {"add_book", 8},
                                        {"exit", 9}};
    bool pornit = true;

    while (1) {
        if (pornit == false) {
            break;
        }

        // ma conectez la server
        sockfd = open_connection((char *)"3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
        printf("introduceti comanda: ");
        char comanda_chr[2000];

        // citesc comanda
        fgets(comanda_chr, 2000, stdin);
        comanda_chr[strlen(comanda_chr) - 1] = '\0';
        string comanda(comanda_chr);
    
        switch (comenzi[comanda]) {

            // register
            case 1:
                // citesc datele
                printf("username=");
                fgets(username, 2000, stdin);
                username[strlen(username) - 1] = '\0';
                printf("password=");
                fgets(password, 2000, stdin);
                password[strlen(password) - 1] = '\0';
                user = string(username);
                parola = string(password);

                // compun jsonul
                data = {{"username", username}, {"password", password}};

                // compun si trimit mesajul
                message = compute_post_request("3.8.116.10", "/api/v1/tema/auth/register", "application/json", data.dump(), "");
                send_to_server(sockfd, message);
                printf("%s\n", message);
                response = receive_from_server(sockfd);
                printf("%s\n", response);
                break;

            // login
            case 2:
                // verific ca nu sunt logat
                if (cookie == "") {
                    // citesc datele
                    printf("username=");
                    fgets(username, 2000, stdin);
                    username[strlen(username) - 1] = '\0';
                    printf("password=");
                    fgets(password, 2000, stdin);
                    password[strlen(password) - 1] = '\0';
                    user = string(username);
                    parola = string(password);

                    // compun jsonul
                    data = {{"username", username}, {"password", password}};

                    // compun si trimit mesajul
                    message = compute_post_request("3.8.116.10", "/api/v1/tema/auth/login", "application/json", data.dump(), "");
                    send_to_server(sockfd, message);
                    printf("%s\n", message);
                    response = receive_from_server(sockfd);
                    printf("%s\n", response);

                    // salvez cookie ul de login
                    cookie_start = NULL;
                    cookie_start = strstr(response, "connect");
                    if (cookie_start != NULL) {
                        cookie_end = strstr(cookie_start, ";");
                        cookie_start[(strlen(cookie_start) - strlen(cookie_end))] = '\0';
                        cookie = string(cookie_start);
                    }
                } else {
                    printf("sunteti deja logat\n");
                }
                break;

            // logout
            case 3:
                // verific daca sunt logat
                if (cookie != "") {

                    // compun si trimit mesajul
                    message = compute_get_request("3.8.116.10", "/api/v1/tema/auth/logout", "", cookie);
                    send_to_server(sockfd, message);
                    printf("%s\n", message);
                    response = receive_from_server(sockfd);
                    printf("%s\n", response);

                    // sterg cookie ul de login si pe cel de acces
                    cookie = "";
                    acces = NULL;
                } else {
                    printf("nu sunteti logat\n");
                }
                break;

            // enter_library
            case 4:
                // verific daca sunt logat
                if (cookie != "") {

                    // compun si trimit mesajul
                    message = compute_get_request("3.8.116.10", "/api/v1/tema/library/access", "", cookie);
                    send_to_server(sockfd, message);
                    printf("%s\n", message);
                    response = receive_from_server(sockfd);
                    printf("%s\n", response);
                    raspuns = string(response);

                    // salvez cookie ul de acces
                    acces = json::parse(raspuns.substr(raspuns.find('{'), raspuns.find('}')));                    
                } else {
                    printf("nu sunteti logat\n");
                }
                break;

            // get_books
            case 5:

                // verific daca am acces
                if (acces != NULL) {

                    // parsez cookie ul de acces
                    autorizatie = acces["token"];

                    // compun si trimit mesajul
                    message = compute_get_authorization_request("3.8.116.10", "/api/v1/tema/library/books", "", autorizatie);
                    send_to_server(sockfd, message);
                    printf("%s\n", message);
                    response = receive_from_server(sockfd);
                    printf("%s\n", response);
                } else {
                    printf("nu aveti acces in biblioteca\n");
                }
                break;
            
            // get_book
            case 6:

            // verific daca am acces
                if (acces != NULL) {

                    // parsez cookie ul de acces
                    autorizatie = acces["token"];
                    adresa = "/api/v1/tema/library/books/";

                    // citesc id ul
                    printf("id=");
                    fgets(id, 2000, stdin);
                    id[strlen(id) - 1] = '\0';
                    if (valid(id)) {
                        adresa = adresa + std::string(id);

                        // compun si trimit mesajul
                        message = compute_get_authorization_request("3.8.116.10", adresa, "", autorizatie);
                        send_to_server(sockfd, message);
                        printf("%s\n", message);
                        response = receive_from_server(sockfd);
                        printf("%s\n", response);
                    } else {
                        printf("id invalid\n");
                    }
                } else {
                    printf("nu aveti acces in biblioteca\n");
                }
                break;

            // delete_book
            case 7:

                // verific daca am acces
                if (acces != NULL) {

                    //parsez cookie ul de acces
                    autorizatie = acces["token"];
                    adresa = "/api/v1/tema/library/books/";

                    // citesc id ul
                    printf("id=");
                    fgets(id, 2000, stdin);
                    id[strlen(id) - 1] = '\0';
                    if (valid(id)) {
                        adresa = adresa + std::string(id);

                        // compun si trimit mesajul
                        message = compute_delete_authorization_request("3.8.116.10", adresa, "", autorizatie);
                        send_to_server(sockfd, message);
                        printf("%s\n", message);
                        response = receive_from_server(sockfd);
                        printf("%s\n", response);
                    } else {
                        printf("id invalid\n");
                    }
                } else {
                    printf("nu aveti acces in biblioteca\n");
                }
                break;

            // add_book
            case 8:

                // verific daca am acces
                if (acces != NULL) {

                    // citesc datele despre carte
                    printf("title=");
                    fgets(titlu, 2000, stdin);
                    titlu[strlen(titlu) - 1] = '\0';
                    printf("author=");
                    fgets(autor, 2000, stdin);
                    autor[strlen(autor) - 1] = '\0';
                    printf("genre=");
                    fgets(gen, 2000, stdin);
                    gen[strlen(gen) - 1] ='\0';
                    printf("page_count=");
                    fgets(nrPagini, 2000, stdin);
                    nrPagini[strlen(nrPagini) - 1] = '\0';
                    printf("publisher=");
                    fgets(editor, 2000, stdin);
                    editor[strlen(editor) - 1] = '\0';
                    if (valid(nrPagini)) {

                        // parsez tokenul de acces
                        autorizatie = acces["token"];

                        // creez o noua carte
                        Carte carte(titlu, autor, gen, nrPagini, editor);
                        carte.makeJson();

                        // parsez datele cartii
                        json data = carte.data;

                        // compun si trimit mesajul
                        message = compute_post_authorization_request("3.8.116.10",
                            "/api/v1/tema/library/books", "application/json", data.dump(), autorizatie);
                        send_to_server(sockfd, message);
                        printf("%s\n", message);
                        response = receive_from_server(sockfd);
                        printf("%s\n", response);
                    } else {
                        printf("nr pagini incorect\n");
                    }

                } else {
                    printf("Nu aveti acces in biblioteca\n");
                }
                break;

            // exit
            case 9:
                printf("exit\n");
                pornit = false;
                break;
            default:
                printf("comanda gresita\n");
        }

    }

    // ma deconectez de la server
    close_connection(sockfd);
    return 0;
}
