#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <locale.h>
int gener = 0;
#define N 80
#define M 25

void input(int **gen);
int find_neigh(int **gen, int i, int j);
void new_generation(int **gen, int ** new_gen);
void copy(int **gen, int ** new_gen);
void output(int **gen, int *speed);
void free_matrix(int **mat);
int **create_matrix();
void init_ncurses();
void make_terminal_great_again();
int speed_check(int *speed);

int main(){
  int** gen = create_matrix();
  int** new_gen = create_matrix();
  int speed = 5;
  input(gen);
  init_ncurses();
  make_terminal_great_again();
  while(1){
    clear();
    output(gen, &speed);
    speed_check(&speed);
    new_generation(gen, new_gen);
    copy(gen, new_gen);
    napms(10 * speed);
    refresh();
  }
  free_matrix(gen);
  free_matrix(new_gen);
  return 0;
}

int speed_check(int *speed){
  char c;
  c = getch();
  if (c=='z'){
    *speed -= 1;
  }
  else if (c=='a'){
    *speed += 1;
  }
}

void input(int **gen){
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      scanf("%d", &gen[i][j]);
    }
  }
}

int find_neigh(int **gen, int i, int j){
  int count = 0;
  for (int x = 0; x < 3; x++)
  {
    for (int y = 0; y < 3; y++)
    {
      int i1 = (i+x+M-1)%M;
      int j1 = (j+y+N-1)%N;
      if ((gen[i1][j1] == 1 && (x != 1 || y != 1))) count++;
    }
  }
  return count;
}

void new_generation(int **gen, int ** new_gen){
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      if(find_neigh(gen, i, j)==3 && gen[i][j]==0){
        new_gen[i][j] = 1;
      }
      else if ((find_neigh(gen, i, j)==2 || find_neigh(gen, i, j)==3) && gen[i][j]==1){
        new_gen[i][j] = 1;
      }
      else new_gen[i][j] = 0;
    }
  }
}

void copy(int **gen, int ** new_gen){
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++){
      gen[i][j] = new_gen[i][j];
    }
  }
}

void output(int **gen, int *speed){
  gener+=1;
  printw("GENERATION: %d      SPEED: %d\n", gener, *speed);
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      if (gen[i][j] == 0) printw("░");
      else printw("█");
    }
    printw("\n");
  }
  curs_set(0);
}

void free_matrix(int **mat){
  for (int i = 0; i < M; i++)
  {
    free(mat[i]);
  }
  free(mat);
}

int **create_matrix(){
  int **mat = malloc(M * sizeof(int*));
  for (int i = 0; i < M; i++)
  {
    mat[i] = (int*)malloc(N * sizeof(int));
  }
  return mat;
}

void init_ncurses() {
    setlocale(LC_ALL, "");
    initscr();
    nodelay(stdscr, TRUE);
    noecho();
    keypad(stdscr, TRUE);
}

void make_terminal_great_again() { (void)freopen("/dev/tty", "rw", stdin); }
