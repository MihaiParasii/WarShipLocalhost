//
// Created by Misha on 25.02.2024.
//

#include <stdbool.h>

#ifndef SERVER_FUNC_H
#define SERVER_FUNC_H

#include "ships.h"

bool in_list(const char *list, char item, int length_list);

void init_map(char *map);

void print_map(const char *map);

void set_ship_attributes(int *length, Point *point, char *orientation);

void set_ship_on_map(Ship ship_item, char *map);

bool are_valid_ship_attributes(const Ship ship_item);

void update_count_sets_ships(const Ship ship_item);

bool check_close_coords(const char *map, Ship ship_item);

void set_shot_coords(Point *point);

bool are_valid_shot_coords(Point *point);

bool is_hit(const char *map, Point *point);

void update_map_after_shot(char *map, Point *point, const bool is_hit_status);

bool is_ubit(char *map, Point *point);

void print_maps(char *enemy_map, char *map);

void set_coords_from_buffer(char *buffer, Point *point);

bool is_same(char *str1, char *str2);

bool do_player_shot_is_this_position(char *map, Point *point);

#endif //SERVER_FUNC_H
