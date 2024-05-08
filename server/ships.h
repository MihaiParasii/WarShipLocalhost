//
// Created by Misha on 25.02.2024.
//

#ifndef SERVER_SHIPS_H
#define SERVER_SHIPS_H



typedef struct Point {
	int x;
	int y;
} Point;

typedef struct Ship {
    Point *point;
    int length;
    char orientation;
} Ship;


int count_set_ships;

const int count_max_ships;

int count_successful_shots_on_enemy_map;
int count_successful_shots_on_your_map;

const int max_count_successful_shots_on_enemy_map;
const int max_count_successful_shots_on_your_map;

int count_ships_5;
int count_ships_4;
int count_ships_3;
int count_ships_2;
int count_ships_1;


const int count_max_ships_5;
const int count_max_ships_4;
const int count_max_ships_3;
const int count_max_ships_2;
const int count_max_ships_1;

#endif //SERVER_SHIPS_H
