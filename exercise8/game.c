#define _BSD_SOURCE

#include <unistd.h>
#include "grid.h"
#include <time.h>
// The cellular automaton "Game of Life"

int count_neighbours(struct grid_t * grid, int row, int cell) {

	int neigbours = 0;
	if (grid->cells[row][cell] == CELL_ALIVE)
	{	
		neigbours=-1;
	}
	//printf("%d, %d\n", grid->cells[row][cell], neigbours);
	for (int i = row-1; i <= row+1; i++)
	{
		for (int j = cell-1; j <= cell+1; j++)
		{
			if (grid->cells[i][j]==CELL_ALIVE)
			{
				neigbours++;			
			}
		}
	}
	return neigbours;
}

cell_t next_cell_state1(cell_t current_cell_state, int neighbours) {

	switch(neighbours){
		case 2:
			return current_cell_state;
			break;
		case 3:
			return CELL_ALIVE;
			break;
		default:
			return CELL_DEAD;
	}
}

cell_t next_cell_state2(cell_t current_cell_state, int neigbours){
	if (neigbours == 3 || neigbours == 4)
	{
		return CELL_ALIVE;
	}
	else if(neigbours==2) {
		return current_cell_state;
	}
	return CELL_DEAD;
}

void next_generation(struct grid_t * source, struct grid_t * destination, cell_t (*next_function) (cell_t, int)) {
	
	for (int i = 1; i < source->height-1; i++)
	{
		for (int j = 1; j < source->width-1; j++)
		{
			destination->cells[i][j] = next_function(source->cells[i][j],count_neighbours(source,i,j));
		}
	}
}

int main(int argc, char **argv) {

	if (argc < 3)
	{
		printf("Argument missing!\n");
		return -1;
	}
	struct grid_t *last_grid;
	struct grid_t *next_grid;
	struct grid_t *temp;
	int generations;

	if((read_grid_file(argv[1],&last_grid))!=SUCCESS){
		return -1;
	}

	make_grid(last_grid->width,last_grid->height,&next_grid);

	printf("Initial grid:\n");
	print_grid(last_grid);

	sscanf(argv[2], "%d", &generations);
	if (generations < 0)
	{
		printf("Wrong argument type\n");
		return -1;
	}
	
	int rule;
	cell_t (*next) (cell_t, int);
	if (argc == 3)			
	{
		next = next_cell_state1;
	}
	else {
		sscanf(argv[3], "%d", &rule);
		switch(rule) {
			case 2:	 
				next = next_cell_state2;
				break; 
			default: 
				next = next_cell_state1;		
		}
	}
	
	
	for (int i = 0; i < generations; i++)
	{
		printf("%d. Generation:\n",i+1);
		next_generation(last_grid,next_grid, next);
		print_grid(next_grid);
		temp = last_grid;
		last_grid = next_grid;
		next_grid = temp;
		usleep(100000);
	}
	free_grid(next_grid);
	free_grid(last_grid);
	return 0;
}