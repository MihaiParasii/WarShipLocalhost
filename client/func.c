//
// Created by Misha on 25.02.2024.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "ships.h"
#include "../server/define.h"
#include "func.h"

bool in_list(const char *list, const char item, const int length_list) {
	for (int i = 0; i < length_list; ++i) {
		if (item == *(list + i)) {
			return true;
		}
	}
	return false;
}

void init_map(char *map) {
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLUMNS; ++j) {
			*(map + i * ROWS + j) = ' ';
			if (i == 0) {
				*(map + i * ROWS + j) = BORDER_SYMBOL;
			}
			if (i == ROWS - 1) {
				*(map + i * ROWS + j) = BORDER_SYMBOL;
			}
			if (j == 0) {
				*(map + i * ROWS + j) = BORDER_SYMBOL;
			}
			if (j == COLUMNS - 1) {
				*(map + i * ROWS + j) = BORDER_SYMBOL;
			}
		}
	}
}

void print_map(const char *map) {
	int only_one_time = 1;
	for (int i = 0; i < ROWS; ++i) {
		if (only_one_time == 1) {
			printf("     1 2 3 4 5 6 7 8 9 10\n");
			only_one_time = 0;
		}
		printf("%2d ", i);
		for (int j = 0; j < COLUMNS; ++j) {
			printf("%c ", *(map + i * ROWS + j));
		}
		printf("\n");
	}
}

void set_ship_attributes(int *length, Point *point, char *orientation) {
	*orientation = 'H';
	printf("Input ship length: ");
	scanf("%d", length);

	if (*length != 1) {
		printf("H - Horizontal\nV - Vertical\nInput ship's orientation: ");
		scanf(" %c", orientation);
	}
	printf("Input row: ");
	scanf("%d", &point->x);
	printf("Input column: ");
	scanf("%d", &point->y);
}

void set_ship_on_map(const Ship ship_item, char *map) {
	if (ship_item.length == 1) {
		*(map + ship_item.point->x * ROWS + ship_item.point->y) = SHIP_SYMBOL;
		return;
	}
	if (ship_item.orientation == 'H') {
		for (int i = 0; i < ship_item.length; ++i) {
			*(map + ship_item.point->x * ROWS + ship_item.point->y + i) = SHIP_SYMBOL;
		}
		return;
	}
	if (ship_item.orientation == 'V') {
		for (int i = 0; i < ship_item.length; ++i) {
			*(map + (ship_item.point->x + i) * ROWS + ship_item.point->y) = SHIP_SYMBOL;
		}
		return;
	}
}

bool are_valid_ship_attributes(const Ship ship_item) {
	if ((ship_item.length == 1) && (count_ships_1 >= count_max_ships_1)) {
		printf("A maximum of 1 long ships are placed\n");
		return false;
	}
	if ((ship_item.length == 2) && (count_ships_2 >= count_max_ships_2)) {
		printf("A maximum of 2 long ships are placed\n");
		return false;
	}
	if ((ship_item.length == 3) && (count_ships_3 >= count_max_ships_3)) {
		printf("A maximum of 3 long ships are placed\n");
		return false;
	}
	if ((ship_item.length == 4) && (count_ships_4 >= count_max_ships_4)) {
		printf("A maximum of 4 long ships are placed\n");
		return false;
	}
	if ((ship_item.length == 5) && (count_ships_5 >= count_max_ships_5)) {
		printf("A maximum of 5 long ships are placed\n");
		return false;
	}

	if ((ship_item.length < 1) || (ship_item.length > 5)) {
		printf("Length is wrong\n");
		return false;
	}

	if ((ship_item.point->x < 1) || (ship_item.point->x >= ROWS - 1)) {
		printf("The ship leaves the map\n");
		return false;
	}
	if ((ship_item.point->y < 1) || (ship_item.point->y >= COLUMNS - 1)) {
		printf("The ship leaves the map\n");
		return false;
	}

	if (ship_item.orientation == 'H') {
		if (ship_item.length + ship_item.point->y > COLUMNS - 1) {
			printf("The ship leaves the map\n");
			return false;
		}
	}
	if (ship_item.orientation == 'V') {
		if (ship_item.length + ship_item.point->x > ROWS - 1) {
			printf("The ship leaves the map\n");
			return false;
		}
	}
	if ((ship_item.orientation != 'H') && (ship_item.orientation != 'V')) {
		printf("Orientation is wrong\n");
		return false;
	}

	return true;
}

void update_count_sets_ships(const Ship ship_item) {

	if (ship_item.length == 1) {
		count_ships_1++;
		return;
	}
	if (ship_item.length == 2) {
		count_ships_2++;
		return;
	}
	if (ship_item.length == 3) {
		count_ships_3++;
		return;
	}
	if (ship_item.length == 4) {
		count_ships_4++;
		return;
	}
	if (ship_item.length == 5) {
		count_ships_5++;
		return;
	}
}

bool check_close_coords(const char *map, const Ship ship_item) {
	if (ship_item.length == 1) {
		for (int i = -1; i < 2; ++i) {
			if (*(map + ship_item.point->x * ROWS + ship_item.point->y + i) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
			if (*(map + (ship_item.point->x + i) * ROWS + ship_item.point->y) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
			if (*(map + (ship_item.point->x + i) * ROWS + ship_item.point->y + i) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
			if (*(map + (ship_item.point->x + i) * ROWS + ship_item.point->y - i) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
		}
	}

	if ((ship_item.orientation == 'H') && (ship_item.length > 1)) {
		for (int i = -1; i < ship_item.length + 1; ++i) {
			if (*(map + ship_item.point->x * ROWS + ship_item.point->y + i) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
			if (*(map + (ship_item.point->x - 1) * ROWS + ship_item.point->y + i) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
			if (*(map + (ship_item.point->x + 1) * ROWS + ship_item.point->y + i) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
		}
	}

	if ((ship_item.orientation == 'V') && (ship_item.length > 1)) {
		for (int i = -1; i < ship_item.length + 1; ++i) {
			if (*(map + (ship_item.point->x + i) * ROWS + ship_item.point->y) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
			if (*(map + (ship_item.point->x + i) * ROWS + ship_item.point->y - 1) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
			if (*(map + (ship_item.point->x + i) * ROWS + ship_item.point->y + 1) == SHIP_SYMBOL) {
				printf("The ship is in the area of other ships\n");
				return false;
			}
		}
	}

	return true;
}

void set_shot_coords(Point *point) {
	printf("Input row to shot: ");
	scanf("%d", &point->x);
	printf("Input column to shot: ");
	scanf("%d", &point->y);
}

bool are_valid_shot_coords(Point *point) {
	if ((point->x < 1) || (point->x > ROWS - 2)) {
		return false;
	}
	if ((point->y < 1) || (point->y > COLUMNS - 2)) {
		return false;
	}
	return true;
}

bool is_hit(const char *map, Point *point) {
	if (*(map + point->x * ROWS + point->y) == SHIP_SYMBOL) {
		return true;
	}
	return false;
}

void update_map_after_shot(char *map, Point *point, const bool is_hit_status) {
	if (is_hit_status) {
		*(map + point->x * ROWS + point->y) = HIT_SYMBOL;
		return;
	}
	if (*(map + point->x * ROWS + point->y) == HIT_SYMBOL) {
		return;
	}

	*(map + point->x * ROWS + point->y) = MIMO_SYMBOL;
}

bool is_ubit(char *map, Point *point) {
	bool is_ship_N = false;
	bool is_ship_S = false;
	bool is_ship_E = false;
	bool is_ship_W = false;

	char list_symbols_no_ship[] = {' ', BORDER_SYMBOL, MIMO_SYMBOL};

	int length_list;
	length_list = sizeof(list_symbols_no_ship) / sizeof(list_symbols_no_ship[0]);


	/*
	 * X X if == C return true else if == % or - or ' ' -> return false
	 */

	// Check is_ship_E
	for (int i = 1; i < 5; ++i) {
		if (in_list(list_symbols_no_ship, *(map + point->x * ROWS + point->y + i), length_list)) {
			is_ship_E = false;
			break;
		}
		if (*(map + point->x * ROWS + point->y + i) == SHIP_SYMBOL) {
			is_ship_E = true;
			break;
		}
	}

	// Check is_ship_W
	for (int i = 1; i < 5; ++i) {
		if (in_list(list_symbols_no_ship, *(map + point->x * ROWS + point->y - i), length_list)) {
			is_ship_W = false;
			break;
		}
		if (*(map + point->x * ROWS + point->y - i) == SHIP_SYMBOL) {
			is_ship_W = true;
			break;
		}
	}

	// Check is_ship_N
	for (int i = 1; i < 5; ++i) {
		if (in_list(list_symbols_no_ship, *(map + (point->x - i) * ROWS + point->y), length_list)) {
			is_ship_N = false;
			break;
		}
		if (*(map + (point->x - i) * ROWS + point->y) == SHIP_SYMBOL) {
			is_ship_N = true;
			break;
		}
	}

	// Check is_ship_S
	for (int i = 1; i < 5; ++i) {
		if (in_list(list_symbols_no_ship, *(map + (point->x + i) * ROWS + point->y), length_list)) {
			is_ship_S = false;
			break;
		}
		if (*(map + (point->x + i) * ROWS + point->y) == SHIP_SYMBOL) {
			is_ship_S = true;
			break;
		}
	}

	if (is_ship_N) {
		return false;
	}
	if (is_ship_S) {
		return false;
	}
	if (is_ship_E) {
		return false;
	}
	if (is_ship_W) {
		return false;
	}

	return true;
}

void print_maps(char *enemy_map, char *map) {
	printf("Enemy map:\n");
	print_map(enemy_map);
	printf("\nYour map:\n");
	print_map(map);
	printf("\n\n");
}

void set_coords_from_buffer(char *buffer, Point *point) {
	char *x = strtok(buffer, " ");
	char *y = strtok(NULL, " ");

	point->x = atoi(x);
	point->y = atoi(y);
}

bool is_same(char *str1, char *str2) {
	if (strcmp(str1, str2) == 0) {
		return true;
	}
	return false;
}

bool do_player_shot_is_this_position(char *map, Point *point) {
	if (*(map + point->x * ROWS + point->y) == ' ') {
		return false;
	}
	return true;
}