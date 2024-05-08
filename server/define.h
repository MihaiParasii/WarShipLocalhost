//
// Created by Misha on 16.02.2024.
//

#ifndef SERVER_DEFINE_H
#define SERVER_DEFINE_H

#if defined(_WIN32)
#define CLEARSCREEN "cls"
#endif

#if defined(__APPLE__)
#define CLEARSCREEN "clear"
#endif

#define IP_ADDRESS "192.168.0.51"
#define PORT 51111
#define MAX_LENGTH_MESSAGE 1000

#define ROWS 12
#define COLUMNS 12
#define SHIP_SYMBOL 'C'
#define BORDER_SYMBOL '#'
#define MIMO_SYMBOL '-'
#define HIT_SYMBOL 'X'

#define IS_UBIT "is_ubit"
#define IS_RANEN "is_ranen"
#define MIMO "mimo"
#define YOUR_TURN "your_turn"

#endif //SERVER_DEFINE_H
