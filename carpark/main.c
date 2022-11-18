#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_SIZE 20
#define BOLLARD '#'
#define SPACE '.'

typedef enum {
  Horizontal,
  Vertical
} Orientation;

typedef struct {
  char  label;
  Orientation orientation;
  int length;
  int start_row;
  int start_col;
} Car;

Car make_car(char label, int start_row, int start_col) {
  Car car;
  car.label = label;
  car.start_row = start_row;
  car.start_col = start_col;
  car.length = 1;
  return car;
}

typedef struct {
  Car  cars[MAX_SIZE];
  int  length;
} CarList;

CarList make_car_list() {
  CarList car_list;
  car_list.length = 0;
  return car_list;
}

void add_car(CarList *car_list, Car *car) {
  car_list->cars[car_list->length++] = *car;
}

int find_car_index(CarList *car_list, char label) {
  for (int position = 0; position < car_list->length; position++) {
    if (car_list->cars[position].label == label) {
      return position;
    }
  }
  return -1;
}

typedef struct {
  int row;
  int col;
} Exit;

Exit make_exit(int row, int col) {
  Exit exit;
  exit.row = row;
  exit.col = col;
  return exit;
}

typedef struct {
  Exit exits[MAX_SIZE];
  int length;
} ExitList;

ExitList make_exit_list() {
  ExitList exit_list;
  exit_list.length = 0;
  return exit_list;
}

void add_exit(ExitList *exit_list, Exit *exit) {
  exit_list->exits[exit_list->length++] = *exit;
}

typedef struct {
  int parent;
  int height;
  int width;
  ExitList exit_list;
  CarList car_list;
} CarPark;

CarPark make_carpark(int height, int width, int parent) {
  CarPark carpark;
  carpark.parent = parent;
  carpark.height = height;
  carpark.width = width;
  carpark.exit_list = make_exit_list();
  carpark.car_list = make_car_list();
  return carpark;
}

bool is_corner(CarPark *carpark, int col, int row) {
  return ((col == 0 || col == carpark->width -1) &&
	  (row == 0 || row == carpark->height -1));
}
  
bool is_edge(CarPark *carpark, int col, int row) {
  if ((col == 0 || col == carpark->width - 1) ^
      (row == 0 || row == carpark->height - 1)) {
    return true;
  }
  return false;
}

bool is_exit(CarPark *carpark, int col, int row) {
  for (int position = 0; position < carpark->exit_list.length; position++) {
    Exit exit = carpark->exit_list.exits[position];
    if (exit.col == col && exit.row == row) {
      return true;
    }
  }
  return false;
}

bool is_horizontal_car_part(Car *car, int col, int row) {
  if (car->orientation != Horizontal) {
    return false;
  }
  if (car->start_row != row) {
    return false;
  }
  if (col < car->start_col) {
    return false;
  }
  if (col >= car->start_col + car->length) {
    return false;
  }
  return true;
}

bool is_vertical_car_part(Car *car, int col, int row) {
  if (car->orientation != Vertical) {
    return false;
  }
  if (car->start_col != col) {
    return false;
  }
  if (row < car->start_row) {
    return false;
  }
  if (row >= car->start_row + car->length) {
    return false;
  }
  return true;
}

bool is_car_part(Car *car, int col, int row) {
  return (is_horizontal_car_part(car, col, row) ||
	  is_vertical_car_part(car, col, row));
}

char get_car_label(CarPark *carpark, int col, int row) {
  for (int position = 0; position < carpark->car_list.length; position++) {
    Car *car = &(carpark->car_list.cars[position]);
    if (is_car_part(car, col, row)) {
      return car->label;
    }
  }
  return SPACE;
}

bool add_carpark_row(CarPark *carpark, char *str, int row) {
  if (strlen(str) != carpark->width) {
    return false;
  }
  if (row >= carpark->height) {
    return false;
  }
  for(int col = 0; col < carpark->width; col++) {
    if (str[col] == SPACE && is_edge(carpark, col, row)) {
      Exit new_exit = make_exit(row, col);
      add_exit(&carpark->exit_list, &new_exit);
    }
    if ((str[col] != SPACE) && (str[col] != BOLLARD)) {
      int car_index = find_car_index(&carpark->car_list, str[col]);
      if (car_index == -1) {
	Car new_car = make_car(str[col], row, col);
	add_car(&carpark->car_list, &new_car);
      } else {
	Car *car = &(carpark->car_list.cars[car_index]);
	car->length++;
	if (car->start_col < col) {
	  car->orientation = Horizontal;
	} else {
	  car->orientation = Vertical;
	}
      }
    }
  }  return true;
}

void print_park(CarPark *carpark) {
  for (int row = 0; row < carpark->height; row++) {
    for (int col = 0; col < carpark->width; col++) {
      if (is_corner(carpark, col, row)) {
	printf("#");
	continue;
      }
      if (is_edge(carpark, col, row)) {
	if (is_exit(carpark, col, row)) {
	  printf("%c", SPACE);
	} else {
	  printf("%c", BOLLARD);
	}
	continue;
      }
      char label = get_car_label(carpark, col, row);
      printf("%c", label);
    }
    printf("\n");
  }
}

typedef struct {
  CarPark *el[MAX_SIZE];
  int length;
} CarParkTree;

CarParkTree make_carpark_tree(CarPark *init_park) {
  CarParkTree tree;
  tree.el[0] = init_park;
  tree.length = 1;
  return tree;
}

typedef enum {
  Up,
  Down,
  Left,
  Right
} Direction;

CarPark move_car(CarPark *carpark, int car_index, Direction direction) {
  Car *car = &(carpark->car_list.cars[car_index]);
  switch (direction) {
  case Up: {
    if (car->orientation != Vertical) {
      return *carpark;
    }
    for(int position = 0; position < carpark->car_list.length; position++) {
      if (position == car_index) {
	// Ignore car to be moved
	continue;
      }
      Car *current_car = &(carpark->car_list.cars[position]);
      if (is_car_part(current_car, car->start_col, car->start_row -1)) {
	return *carpark;
      }
    }
    if (is_edge(carpark, car->start_col, car->start_row - 1) &&
	!is_exit(carpark, car->start_col, car->start_row - 1)) {
      return *carpark;
    }
    CarPark new_carpark;
    memcpy(&new_carpark, carpark, sizeof *carpark);
    Car *duplicated_car = &(new_carpark.car_list.cars[car_index]);
    duplicated_car->start_row--;
    return new_carpark;
  }
    
  case Down:
    return *carpark;
    
  case Left: {
  
    if (car->orientation != Horizontal) {
      return *carpark;
    }
    for(int position = 0; position < carpark->car_list.length; position++) {
      if (position == car_index) {
	// Ignore car to be moved
	continue;
      }
      Car *current_car = &(carpark->car_list.cars[position]);
      if (is_car_part(current_car, car->start_col - 1, car->start_row)) {
	return *carpark;
      }
    }
    if (is_edge(carpark, car->start_col - 1, car->start_row) &&
	!is_exit(carpark, car->start_col - 1, car->start_row)) {
      return *carpark;
    }
    CarPark new_carpark;
    memcpy(&new_carpark, carpark, sizeof *carpark);
    Car *duplicated_car = &(new_carpark.car_list.cars[car_index]);
    duplicated_car->start_col--;
    return new_carpark;
  }
    
  case Right:
    return *carpark;
  }
}
      


void test(void) {
  CarPark carpark = make_carpark(3, 3, -1);
  add_carpark_row(&carpark, "###", 0);
  add_carpark_row(&carpark, "#.#", 1);
  add_carpark_row(&carpark, ".##", 2);
  assert(carpark.height == 3);
  assert(carpark.width == 3);
  assert(carpark.exit_list.length == 0);

  Car car;
  car.label = 'A';
  car.orientation = Vertical;
  car.length = 2;
  car.start_col = 1;
  car.start_row = 1;

  CarList car_list = make_car_list();
  add_car(&car_list, &car);

  assert(car_list.length == 1);
  assert(car_list.cars[0].label == 'A');

  CarPark second_park = make_carpark(5, 5, -1);
  int row = 0;
  assert(add_carpark_row(&second_park, "##.##", row));
  assert(add_carpark_row(&second_park, ".AA.#", ++row));
  assert(add_carpark_row(&second_park, "#.B.#", ++row));
  assert(add_carpark_row(&second_park, "#.B.#", ++row));
  assert(add_carpark_row(&second_park, "#####", ++row));
  
  assert(second_park.exit_list.length == 2);
  assert(second_park.car_list.length == 2);
  assert(second_park.car_list.cars[0].label == 'A');
  assert(second_park.car_list.cars[0].length == 2);
  assert(second_park.car_list.cars[1].label == 'B');
  assert(second_park.car_list.cars[1].length == 2);

  print_park(&second_park);

  CarParkTree tree = make_carpark_tree(&second_park);

  CarPark cp = move_car(&second_park, 1, Up);
  assert(cp.car_list.cars[1].start_row == 2);

  CarPark cp2 = move_car(&second_park, 0, Left);
  print_park(&cp2);
  
}

int main(void)
{
  test();
}

/* ##.### */
/* #..AA. */
/* #.B..# */
/* #.B..# */
/* #.B..# */
/* ###### */
