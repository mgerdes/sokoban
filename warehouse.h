typedef enum Tile { EMPTY, WALL, PLAYER, CRATE, DEST, PLAYER_ON_DEST, CRATE_ON_DEST } Tile;

typedef struct Warehouse {
    int height, width;
    int player_row, player_col;
    Tile** tiles;
} Warehouse;

void move_player_left(Warehouse*);
void move_player_right(Warehouse*);
void move_player_up(Warehouse*);
void move_player_down(Warehouse*);
Warehouse* create_warehouse(int, int);
