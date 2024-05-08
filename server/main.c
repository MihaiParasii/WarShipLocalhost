#include "define.h"
#include "func.h"
#include "ships.h"
#include <stdio.h>


#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)
#endif

#if defined(__APPLE__)

#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#endif

int main() {
    char buffer[MAX_LENGTH_MESSAGE];
    memset(buffer, 0, MAX_LENGTH_MESSAGE);

#if defined(_WIN32)
    WSADATA wsa;
    SOCKET s;
    SOCKET client;
#endif

#if defined(__APPLE__)
    int s;
    int client;
#endif

    struct sockaddr_in server;
    printf("\nInitialising Winsock...");

#if defined(_WIN32)
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
#endif

    printf("Initialised.\n");

    //Create a socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket.\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    server.sin_port = htons(PORT);

    if (bind(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	// TODO
	//  arhitectura de baza pentru network

    //Connect to remote server
    if (listen(s, 30) < 0) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    if ((client = accept(s, NULL, NULL)) < 0) {
        perror("accept error");
        exit(EXIT_FAILURE);
    }

    /* --------------------------START GAME--------------------------*/

    bool is_start = true;
    bool is_server_winner;
    int length_ship;
    char orientation_ship;

    char *map = (char *) malloc(ROWS * COLUMNS * sizeof(char));
    char *enemy_map = (char *) malloc(ROWS * COLUMNS * sizeof(char));

	Point *point_to_shot = calloc(1, sizeof(Point));


    bool is_hit_status;

    system(CLEARSCREEN);

    init_map(map);
    init_map(enemy_map);


    printf("Set ships on map...:\n");
    while (count_set_ships < count_max_ships) {
	    Point *point = calloc(1, sizeof(Point));

	    set_ship_attributes(&length_ship, point, &orientation_ship);
        Ship ship = {point, length_ship, orientation_ship};

        if (!are_valid_ship_attributes(ship)) {
            continue;
        }
        if (!check_close_coords(map, ship)) {
            continue;
        }
        set_ship_on_map(ship, map);
        update_count_sets_ships(ship);
        ++count_set_ships;
        system(CLEARSCREEN);
        print_map(map);
	    free(point);
    }


    printf("Waiting for Client...\n");


    strcpy(buffer, "ALL SHIPS ARE SET");
    send(client, buffer, MAX_LENGTH_MESSAGE, 0);
    memset(buffer, 0, MAX_LENGTH_MESSAGE);
    recv(client, buffer, MAX_LENGTH_MESSAGE, 0);


    if (strcmp(buffer, "ALL SHIPS ARE SET") != 0) {
        return 0;
    }

    strcpy(buffer, YOUR_TURN);
    system(CLEARSCREEN);

    while (true) {
        print_maps(enemy_map, map);
        printf("\n\n");

        if (!is_start) {
            recv(client, buffer, MAX_LENGTH_MESSAGE, 0);
        }
        is_start = false;

        if (strncmp(buffer, "########", 8) == 0) {
            strcpy(enemy_map, buffer);
            strcpy(buffer, YOUR_TURN);
            continue;
        }

        if (is_same(buffer, YOUR_TURN) || is_same(buffer, IS_RANEN) || is_same(buffer, IS_UBIT)) {

            system(CLEARSCREEN);

            if (is_same(buffer, IS_RANEN)) {
                ++count_successful_shots_on_enemy_map;
                is_hit_status = true;
	            update_map_after_shot(enemy_map, point_to_shot, is_hit_status);
                printf("Ranen\n");
            }
            if (is_same(buffer, IS_UBIT)) {
                ++count_successful_shots_on_enemy_map;
                is_hit_status = true;
	            update_map_after_shot(enemy_map, point_to_shot, is_hit_status);
                printf("Ubit\n");
            }
            if (count_successful_shots_on_enemy_map == max_count_successful_shots_on_enemy_map) {
                is_server_winner = true;
                break;
            }

            print_maps(enemy_map, map);
            printf("\n\n");

            do {
	            set_shot_coords(point_to_shot);

                if (point_to_shot->x == 2501 && point_to_shot->y == 2004) {
	                sprintf(buffer, "%d %d", point_to_shot->x, point_to_shot->y);
                    send(client, buffer, MAX_LENGTH_MESSAGE, 0);
                    system(CLEARSCREEN);
                    continue;
                }


                if (!are_valid_shot_coords(point_to_shot)) {
                    printf("Shot coords are wrong\n");
                    continue;
                }
                if (do_player_shot_is_this_position(enemy_map, point_to_shot)) {
                    printf("You already shot in this position\n");
                    continue;
                }

                break;
            } while (true);

	        sprintf(buffer, "%d %d", point_to_shot->x, point_to_shot->y);
            send(client, buffer, MAX_LENGTH_MESSAGE, 0);
            system(CLEARSCREEN);
            continue;
        }

        if (is_same(buffer, MIMO)) {
            is_hit_status = false;
	        update_map_after_shot(enemy_map, point_to_shot, is_hit_status);
            send(client, YOUR_TURN, MAX_LENGTH_MESSAGE, 0);
            system(CLEARSCREEN);
            printf("Mimo\n");
            continue;
        }

        // If client sent coords --> start from here
	    set_coords_from_buffer(buffer, point_to_shot);

        if (is_hit(map, point_to_shot)) {
            is_hit_status = true;
            if (is_ubit(map, point_to_shot)) {
                send(client, IS_UBIT, MAX_LENGTH_MESSAGE, 0);
            } else {
                send(client, IS_RANEN, MAX_LENGTH_MESSAGE, 0);
            }
            ++count_successful_shots_on_your_map;
        } else {
            is_hit_status = false;
            send(client, MIMO, MAX_LENGTH_MESSAGE, 0);
        }
        if (count_successful_shots_on_your_map == max_count_successful_shots_on_your_map) {
            is_server_winner = false;
            break;
        }

	    update_map_after_shot(enemy_map, point_to_shot, is_hit_status);
        system(CLEARSCREEN);
    }

//    send(client, "exit", MAX_LENGTH_MESSAGE, 0);


    system(CLEARSCREEN);
    print_maps(enemy_map, map);
    printf("\n\n");

    if (is_server_winner) {
        printf("Congratulations\nYou are winner))\n");
    } else {
        printf("Congratulations\nYou are looser))\n");
    }


    free(map);
    free(enemy_map);
    return 0;
}