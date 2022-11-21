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
  Car cars[MAX_SIZE];
  int length;
} CarList;

CarList make_car_list() {
  CarList car_list;
  car_list.length = 0;
  return car_list;
}

void add_car(CarList *car_list, Car *car) {
  car_list->cars[car_list->length++] = *car;
}

bool remove_car(CarList *car_list, int car_index) {
  if (car_list->length == 0) {
    return false;
  }
  if (car_index >= car_list->length) {
    return false;
  }
  for (int position = car_index; position < car_list->length - 1; position++) {
    car_list->cars[position] = car_list->cars[position + 1];
  }
  car_list->length--;
  return true;
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

typedef struct ExitList {
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

bool is_car_part_in_park(CarPark *carpark, int col, int row) {
  for(int position = 0; position < carpark->car_list.length; position++) {
    Car *current_car = &(carpark->car_list.cars[position]);
    if (is_car_part(current_car, col, row)) {
      return true;
    }
  }
  return false;
}

bool is_car_touching_edge(CarPark *carpark, Car* car) {
  if (is_edge(carpark, car->start_col, car->start_row)) {
    return true;
  }
  if (car->orientation == Vertical &&
      is_edge(carpark, car->start_col,
	      car->start_row + car->length - 1)) {
    return true;
  }
  if (car->orientation == Horizontal &&
      is_edge(carpark, car->start_col + car->length - 1,
	      car->start_row)) {
    return true;
  }
  return false;
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

// putchar can be used instead of printf
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
  CarPark el[MAX_SIZE];
  int length;
  int solved_position;
} CarParkTree;

CarParkTree make_carpark_tree(CarPark *init_park) {
  CarParkTree tree;
  tree.el[0] = *init_park;
  tree.length = 1;
  tree.solved_position = -1;
  return tree;
}

typedef enum {
  Up,
  Down,
  Left,
  Right
} Direction;

Car move_car(CarPark *carpark, int car_index, Direction direction) {
  Car car = carpark->car_list.cars[car_index];
  switch (direction) {
  case Up: {
    if (car.orientation != Vertical) {
      return car;
    }
    if (is_car_part_in_park(carpark, car.start_col, car.start_row -1)) {
      return car;
    }
    if (is_edge(carpark, car.start_col, car.start_row - 1) &&
	!is_exit(carpark, car.start_col, car.start_row - 1)) {
      return car;
    }
    Car new_car = car;
    new_car.start_row--;
    return new_car;
  }
    
  case Down: {
    if (car.orientation != Vertical) {
      return car;
    }
    if (is_car_part_in_park(carpark, car.start_col, car.start_row + car.length)) {
      return car;
    }
    if (is_edge(carpark, car.start_col, car.start_row + car.length) &&
	!is_exit(carpark, car.start_col, car.start_row + car.length)) {
      return car;
    }
    Car new_car = car;
    new_car.start_row++;
    return new_car;
  }
    
  case Left: {
    if (car.orientation != Horizontal) {
      return car;
    }
    if (is_car_part_in_park(carpark, car.start_col - 1, car.start_row)) {
      return car;
    }
    if (is_edge(carpark, car.start_col - 1, car.start_row) &&
	!is_exit(carpark, car.start_col - 1, car.start_row)) {
      return car;
    }
    Car new_car = car;
    new_car.start_col--;
    return new_car;
  }
    
  case Right: {
    if (car.orientation != Horizontal) {
      return car;
    }
    if (is_car_part_in_park(carpark, car.start_col + car.length, car.start_row)) {
      return car;
    }
    if (is_edge(carpark, car.start_col + car.length, car.start_row) &&
	!is_exit(carpark, car.start_col + car.length, car.start_row)) {
      return car;
    }
    Car new_car = car;
    new_car.start_col++;
    return new_car;
  }
    
  }
}

bool are_lists_equal(CarList *list, CarList *other) {
  if (list->length != other->length) {
    return false;
  }
  for (int car_index = 0; car_index < list->length; car_index++) {
    Car *car = &(list->cars[car_index]);
    int matching_position = find_car_index(other, car->label);
    if (matching_position == -1) {
      return false;
    }
    Car *matching_car = &(other->cars[matching_position]);
    if ((matching_car->start_col != car->start_col) ||
	(matching_car->start_row != car->start_row)) {
      return false;
    }
  }
  return true;
}

bool is_car_list_unique(CarParkTree *tree, CarList *car_list) {
  for (int park_index = 0; park_index < tree->length; park_index++) {
    CarList *current_car_list = &(tree->el[park_index].car_list);
    if (are_lists_equal(car_list, current_car_list)) {
      return false;
    }
  }
  return true;
}

void create_carpark_children(CarParkTree *tree, int parent_id) {
  CarPark *parent = &(tree->el[parent_id]);
  for (int position = 0; position < parent->car_list.length; position++) {
    Car *car = &(parent->car_list.cars[position]);
    for (Direction direction = Up; direction <= Right; direction++) {
      Car moved_car = move_car(parent, position, direction);
      if ((moved_car.start_row != car->start_row) ||
	  (moved_car.start_col != car->start_col)) {
	CarList updated_list;
	memcpy(&updated_list, &(parent->car_list), sizeof parent->car_list);
	updated_list.cars[position] = moved_car;
	CarPark new_park = make_carpark(parent->height, parent->width, parent_id);
	new_park.exit_list = parent->exit_list;
	new_park.car_list = updated_list;
	if (is_car_touching_edge(&new_park, &moved_car)) {
	  remove_car(&(new_park.car_list), position);
	}
	if (is_car_list_unique(tree, &(new_park.car_list))) {
	  tree->el[tree->length++] = new_park;
	}
      }      
    }
  }
}

CarParkTree solve_carpark(CarPark *carpark) {
  CarParkTree tree = make_carpark_tree(carpark);
  for (int tree_index = 0; tree_index < tree.length; tree_index++) {
    if (tree.el[tree_index].car_list.length == 0) {
      tree.solved_position = tree_index;
      break;
    }
    create_carpark_children(&tree, tree_index);
  }
  return tree;
}

void print_solved_path(CarParkTree *tree) {
  if (tree->solved_position == -1) {
    printf("This tree is not solved\n");
    for (int idx = 0; idx < tree->length; idx++) {
      print_park(&tree->el[idx]);
      printf("\n");
    }
    return;
  }
  int steps = 1;
  int positions[MAX_SIZE] = {tree->solved_position, 0};
  CarPark *current_park = &tree->el[tree->solved_position];
  do {
    positions[steps++] = current_park->parent;
    current_park = &tree->el[current_park->parent];
  } while (current_park->parent != -1);
  for (int step = steps - 1; step >= 0; step--) {
    CarPark *park = &tree->el[positions[step]];
    printf("\nStep %i:\n", steps - step);
    print_park(park);
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


  //print_park(&second_park);

  /* Car car1 = move_car(&second_park, 1, Up); */
  /* assert(car1.start_row == 2); */

  /* Car car1_left = move_car(&second_park, 0, Left); */
  /* assert(car1_left.start_col == 0); */

  /* Car car1_right  = move_car(&second_park, 0, Right); */
  /* assert(car1_right.start_col == 2); */

  /* assert(remove_car(&second_park.car_list, 0)); */
  /* assert(second_park.car_list.length == 1); */
  /* assert(second_park.car_list.cars[0].label == 'B'); */

  CarParkTree tree = solve_carpark(&second_park);
  assert(tree.solved_position != -1);
  print_solved_path(&tree);

  /* printf("======\n"); */

  /* create_carpark_children(&tree, 1); */
  /* printf("%i\n", tree.length); */
  /* for (int i = 0; i < tree.length; i++) { */
  /*   print_park(&tree.el[i]); */
  /*   printf("\n"); */
  /* } */

  /* printf("======\n"); */

  /* create_carpark_children(&tree, 2); */
  /* printf("%i\n", tree.length); */
  /* for (int i = 0; i < tree.length; i++) { */
  /*   print_park(&tree.el[i]); */
  /*   printf("\n"); */
  /* } */
  
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
