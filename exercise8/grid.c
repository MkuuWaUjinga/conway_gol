#include "grid.h"

/*
 * Allocate a game-of-life-grid with the given width (number of cells per row)
 * and height (number of rows). Store the pointer to the new grid in 'result'.
 *
 */
void make_grid(int width, int height, struct grid_t ** result) {
    cell_t **cells;
    unsigned int i;
	struct grid_t *grid;
	
	// allocate the struct
	grid = (struct grid_t*)malloc(sizeof(struct grid_t));
	
	// allocate the array of row-pointers
    cells = (cell_t**)calloc(height, sizeof(cell_t*));

	// allocate each row
    for (i = 0; i < height; ++i) {
        cells[i] = (cell_t*)calloc(width, sizeof(cell_t));
    }
	
	// put all values into the struct
	grid->width = width;
	grid->height = height;
	grid->cells = cells;
	
	// write the pointer to the struct at the address specified by 'result'
	*result = grid;
}

/*
 * Free all memory associated with a game-of-life-grid.
 *
 */
void free_grid(struct grid_t * grid) {
	int row;
    for (row = 0; row < grid->height; ++row) {
        free(grid->cells[row]);
    }
    free(grid->cells);
	free(grid);
}

/*
 * Read a text file, formatted like this:
 *
 *         4      <- 1st line: width as decimal integer
 *         3      <- 2nd line: height as decimal integer
 *         ....   <- rows (. = dead cell, # = living cell)
 *         .##.
 *         ....		
 *                <- final row ending with a linefeed '\n'
 *
 *	If successful, returns SUCCESS and stores the grid in
 *  the location given by result. Otherwise returns INVALID_GRID_FILE
 *  and does not change result.
 *
 */
int read_grid_file(char* filename, struct grid_t ** result) {
    int row, cell, width, height;
    FILE * file;
    int read_result;
	struct grid_t * grid;

    file = fopen(filename, "r");
	if (file == NULL) {
		printf("Error: Cannot open file\n");
		return INVALID_GRID_FILE;
	}
	

	// read first line as width
    read_result = fscanf(file, "%d ", &width);
	
	// check for errors during read 
	// (positive result = number of bytes read, negative = error code)
	if (read_result <= 0) {
		printf("Error: Cannot read width from file\n");
		
		// close file in case of error!
		fclose(file);
		return INVALID_GRID_FILE;
	}
	
	// read second line as height
	read_result = fscanf(file, "%d ", &height);
	if (read_result <= 0) {
		printf("Error: Cannot read height from file\n");
		fclose(file);
		return INVALID_GRID_FILE;
	}
	
	// allocate grid with the given dimensions
	
    make_grid(width, height, &grid);
	
	// read rows
    for (row = 0; row < height; ++row) {
		
		// for each row, read cells
        for (cell = 0; cell < width; ++cell) {
			
			// read character from file
            read_result = fgetc(file);
			
			// if there are not enough characters
			// write error message and release all resources used so far
			if (read_result == EOF) {
				free_grid(grid);
				fclose(file);
				printf("Error: Unexpected end of file\n");
				return INVALID_GRID_FILE;
			}
			
			// translate . and # to dead and living cells
			switch ((char)read_result) {
				case '.':
					grid->cells[row][cell] = CELL_DEAD;
					break;
				case '#':
					grid->cells[row][cell] = CELL_ALIVE;
					break;
					
				// other characters lead to an error
				// and we need to release/free file and memory again
				default:
					free_grid(grid);
					fclose(file);
					printf("Error: Unexpected character in grid cell: %c\n", (char)read_result);
					return INVALID_GRID_FILE;
			}
        }
		
        // read the newline character at the end of each row
		read_result = fgetc(file);
		
		// if that was'nt a newline, there is something wrong:
		if ((char)read_result != '\n') {
			free_grid(grid);
			fclose(file);
			printf("Error: Expected new line, but found character or end of file\n");
			return INVALID_GRID_FILE;
		}
    }
	
    fclose(file);
	*result = grid;	// write the grid pointer behind the given result pointer
	return SUCCESS;
}

/*
 * Output a formatted view on the grid,
 * representing live cells as '#' and dead cells as '.'
 */
void print_grid(struct grid_t * grid) {
    int row, cell;
    for (row = 0; row < grid->height; ++row) {
        for (cell = 0; cell < grid->width; ++cell) {
			
			// Bonus Tip: the magic string "\e[1;31m#\e[0m" instead of "#"
			// will give you a colored # symbol.
			// For more coloring and highlighting in bash, see:
			//   http://misc.flogisoft.com/bash/tip_colors_and_formatting
			
            printf(grid->cells[row][cell] == CELL_ALIVE ? "#" : ".");
        }
		printf("\n");
    }
}