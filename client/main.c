#include "../server/define.h"
#include "func.h"
#include "ships.h"


#if defined(_WIN32)

#include<stdio.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)

#endif


#if defined(__APPLE__)

#include <netinet/in.h>
#include <stdio.h>
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
    SOCKET sock;
#endif

#if defined(__APPLE__)
    int sock;
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

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error\n");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    if ((connect(sock, (struct sockaddr *) &server, sizeof(server))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }


    /* --------------------------START GAME--------------------------*/

    int length_ship;
    char orientation_ship;

    char *map = (char *) malloc(ROWS * COLUMNS * sizeof(char));
    char *enemy_map = (char *) malloc(ROWS * COLUMNS * sizeof(char));

    Point *point_to_shot = calloc(1, sizeof(Point));

    bool is_hit_status;
    bool is_client_winner = false;

    system(CLEARSCREEN);
    init_map(map);
    init_map(enemy_map);

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
    }

    printf("Waiting for Server...\n");

    recv(sock, buffer, MAX_LENGTH_MESSAGE, 0);
    if (strcmp(buffer, "ALL SHIPS ARE SET") != 0) {
        return 0;
    }

    memset(buffer, 0, MAX_LENGTH_MESSAGE);
    strcpy(buffer, "ALL SHIPS ARE SET");
    send(sock, buffer, MAX_LENGTH_MESSAGE, 0);


    system(CLEARSCREEN);
    while (true) {
        print_maps(enemy_map, map);
        printf("\n\n");

        recv(sock, buffer, MAX_LENGTH_MESSAGE, 0);

        if (strcmp(buffer, "2501 2004") == 0) {
            strcpy(buffer, map);
            send(sock, buffer, MAX_LENGTH_MESSAGE, 0);
            system(CLEARSCREEN);
            continue;
        }


        if (is_same(buffer, "exit")) {
            break;
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
                is_client_winner = true;
                break;
            }

            print_maps(enemy_map, map);
            printf("\n\n");


            do {
	            set_shot_coords(point_to_shot);

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
            send(sock, buffer, MAX_LENGTH_MESSAGE, 0);
            system(CLEARSCREEN);
            continue;
        }

        if (is_same(buffer, MIMO)) {
            is_hit_status = false;
	        update_map_after_shot(enemy_map, point_to_shot, is_hit_status);
            send(sock, YOUR_TURN, MAX_LENGTH_MESSAGE, 0);
            system(CLEARSCREEN);
            printf("Mimo\n");
            continue;
        }

        // If server sent coords --> start from here
	    set_coords_from_buffer(buffer, point_to_shot);

        if (is_hit(map, point_to_shot)) {
            is_hit_status = true;
            if (is_ubit(map, point_to_shot)) {
                send(sock, IS_UBIT, MAX_LENGTH_MESSAGE, 0);
            } else {
                send(sock, IS_RANEN, MAX_LENGTH_MESSAGE, 0);
            }
            ++count_successful_shots_on_your_map;
        } else {
            is_hit_status = false;
            send(sock, MIMO, MAX_LENGTH_MESSAGE, 0);
        }
        if (count_successful_shots_on_your_map == max_count_successful_shots_on_your_map) {
            is_client_winner = false;
            break;
        }

	    update_map_after_shot(map, point_to_shot, is_hit_status);
        system(CLEARSCREEN);
    }

    system(CLEARSCREEN);
    print_maps(enemy_map, map);
    printf("\n\n");

    if (is_client_winner) {
        printf("Congratulations\nYou are winner))\n");
    } else {
        printf("Congratulations\nYou are looser))\n");
    }


    free(map);
    free(enemy_map);

    return 0;
}