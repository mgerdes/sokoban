typedef enum Tile { EMPTY, WALL, PLAYER, CRATE, DEST, PLAYER_ON_DEST, CRATE_ON_DEST } Tile;

typedef struct Warehouse {
    int height, width;
    int player_row, player_col;
    int num_of_crates, num_of_crates_on_dests; 
    Tile** tiles;
    Tile** original_tiles;
} Warehouse;

void move_player_left(Warehouse*);
void move_player_right(Warehouse*);
void move_player_up(Warehouse*);
void move_player_down(Warehouse*);
void reset_warehouse(Warehouse*);
int is_warehouse_won(Warehouse*);
Warehouse* create_warehouse(int, int);
