#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// the states of a cell
#define CELL_ALIVE 1
#define CELL_DEAD 0

// return values used in read_grid_file
#define INVALID_GRID_FILE 1
#define SUCCESS 0

// the type of a cell
typedef uint8_t cell_t;

struct grid_t {
	int width;		// number of columns
	int height;		// number of rows
	cell_t **cells;	// pointer to rows
};


void make_grid(int width, int height, struct grid_t ** result);

void free_grid(struct grid_t * grid);

int read_grid_file(char* filename, struct grid_t ** result);

void print_grid(struct grid_t * grid);