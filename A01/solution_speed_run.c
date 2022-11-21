//
// AED, August 2022 (Tomás Oliveira e Silva)
//
// First practical assignement (speed run)
//
// Compile using either
//   cc -Wall -O2 -D_use_zlib_=0 solution_speed_run.c -lm
// or
//   cc -Wall -O2 -D_use_zlib_=1 solution_speed_run.c -lm -lz
//
// Place your student numbers and names here
//   N.Mec. XXXXXX  Name: XXXXXXX
//

//
// What algorithm to use:
#define PROFREC
#define FASTWAY
#define PROFREC2
#define DYNAPR
//
// static configuration
//

#define _max_road_size_  800  // the maximum problem size
#define _min_road_speed_   2  // must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_   9  // must not be larger than 9 (only because of the PDF figure)


//
// include files --- as this is a small project, we include the PDF generation code directly from make_custom_pdf.c
//

#include <math.h>
#include <stdio.h>
#include "../P02/elapsed_time.h"
#include "make_custom_pdf.c"


//
// road stuff
//

static int max_road_speed[1 + _max_road_size_]; // positions 0.._max_road_size_

static void init_road_speeds(void)
{
  double speed;
  int i;

  for(i = 0;i <= _max_road_size_;i++)
  {
    speed = (double)_max_road_speed_ * (0.55 + 0.30 * sin(0.11 * (double)i) + 0.10 * sin(0.17 * (double)i + 1.0) + 0.15 * sin(0.19 * (double)i));
    max_road_speed[i] = (int)floor(0.5 + speed) + (int)((unsigned int)random() % 3u) - 1;
    if(max_road_speed[i] < _min_road_speed_)
      max_road_speed[i] = _min_road_speed_;
    if(max_road_speed[i] > _max_road_speed_)
      max_road_speed[i] = _max_road_speed_;
  }
}


//
// description of a solution
//

typedef struct
{
  int n_moves;                         // the number of moves (the number of positions is one more than the number of moves)
  int positions[1 + _max_road_size_];  // the positions (the first one must be zero)
}
solution_t;


//
// the (very inefficient) recursive solution given to the students
//

static solution_t solution_1,solution_1_best;
static double solution_1_elapsed_time; // time it took to solve the problem
static unsigned long solution_1_count; // effort dispended solving the problem


static void solution_1_recursion(int move_number,int position,int speed,int final_position)
{
  int i,new_speed;

  // record move
  solution_1_count++;
  solution_1.positions[move_number] = position;
  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < solution_1_best.n_moves)
    {
      solution_1_best = solution_1;
      solution_1_best.n_moves = move_number;
    }
    return;
  }
  // no, try all legal speeds
  for(new_speed = speed - 1;new_speed <= speed + 1;new_speed++)
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0;i <= new_speed && new_speed <= max_road_speed[position + i];i++)
        ;
      if(i > new_speed)
        solution_1_recursion(move_number + 1,position + new_speed,new_speed,final_position);
    }
}

static void solve_1(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_1: bad final_position\n");
    exit(1);
  }
  solution_1_elapsed_time = cpu_time();
  solution_1_count = 0ul;
  solution_1_best.n_moves = final_position + 100;
  solution_1_recursion(0,0,0,final_position);
  solution_1_elapsed_time = cpu_time() - solution_1_elapsed_time;
}




//
// Student solution(s):
//

static solution_t solution_2,solution_2_best;
static double solution_2_elapsed_time; // time it took to solve the problem
static unsigned long solution_2_count; // effort dispended solving the problem

static void solve_2_recursion(int move_number,int position,int speed,int final_position)
{
  solution_2_count++;  // increment counter per check
  solution_2.positions[move_number] = position;
  if(position == final_position && speed == 1)
  {
    solution_2_best = solution_2;
    solution_2_best.n_moves = move_number;
    return;
  } else {
    // loop through speeds in order of importance
    // +1, = or -1 in that order
    for(int newspeed = speed + 1; newspeed >= speed-1 ; newspeed--)
    {
      if (max_road_speed[position] < newspeed)
      {
        continue;
      } 
      int speedchecker = position;
      int possibleMove = 1;
      if (newspeed > _max_road_speed_)
      {
        continue;
      }
      // Check if the next newspeed*(newspeed+1)/2 indexes are above speed limit
      for(int j = 0; j <= newspeed; j++){
        if (possibleMove == 0)
        { 
          break;          
        }
        int maxspeed = newspeed - j; // calculate max speed to check for the next maxspeed tiles
        if (maxspeed < _min_road_speed_){
          maxspeed = _min_road_speed_;
        }
        for(int k = 0; k < newspeed-j; k++){
          speedchecker++; // speedchecker will be used as the index to access the array of the road
          if(max_road_speed[speedchecker] < maxspeed || speedchecker > final_position)
          {
            possibleMove = 0;
            break;
          } 
        }
      }
      if (possibleMove == 1){
        solve_2_recursion(move_number+1, position+newspeed, newspeed, final_position);
        return;
      } 
    }
  }
}




static void solve_2(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_2: bad final_position\n");
    exit(1);
  }
  
  solution_2_elapsed_time = cpu_time();
  solution_2_count = 0ul;
  solution_2_best.n_moves = final_position + 100;
  solve_2_recursion(0,0,0,final_position);  
  solution_2_elapsed_time = cpu_time() - solution_2_elapsed_time;

}

// professor algorithm but better

static solution_t solution_3,solution_3_best;
static double solution_3_elapsed_time; // time it took to solve the problem
static unsigned long solution_3_count; // effort dispended solving the problem

static void solution_3_recursion(int move_number,int position,int speed,int final_position)
{
  int i,new_speed;

  // record move
  solution_3_count++;
  solution_3.positions[move_number] = position;
  // is it a solution?
  if(position == final_position && speed == 1)
  {
    // is it a better solution?
    if(move_number < solution_3_best.n_moves)
    {
      solution_3_best = solution_3;
      solution_3_best.n_moves = move_number;
    }
    return;
  }
  // no, try all legal speeds
  if (solution_3_best.positions[move_number] > position)
  {
    return;
  }
    for(new_speed = speed + 1;new_speed >= speed - 1;new_speed--)
    if(new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for(i = 0;i <= new_speed && new_speed <= max_road_speed[position + i];i++)
        ;
      if(i > new_speed)
        solution_3_recursion(move_number + 1,position + new_speed,new_speed,final_position);
    }
}

static void solve_3(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_3: bad final_position\n");
    exit(1);
  }
  solution_3_elapsed_time = cpu_time();
  solution_3_count = 0ul;
  solution_3_best.n_moves = final_position + 100;
  solution_3_recursion(0,0,0,final_position);
  solution_3_elapsed_time = cpu_time() - solution_3_elapsed_time;
}

static solution_t solution_4, solution_4_best, solution_4_before;
static double solution_4_elapsed_time; // time it took to solve the problem
static unsigned long solution_4_count; // effort dispended solving the problem
static int last_speed = 0, last_position = 0;
static int saveArr = 0,saved = 0;

static void solve_4_dyn(int move_number,int position,int speed,int final_position)
{
  
  solution_4_count++;  // increment counter per check
  solution_4.positions[move_number] = position;
  if(position == final_position && speed == 1)
  {
    solution_4_best = solution_4;
    solution_4_best.n_moves = move_number;
    return;
  } else {
    for(int newspeed = speed + 1; newspeed >= speed-1 ; newspeed--)
    {      
      if (max_road_speed[position] < newspeed)
      {
        continue;
      }
      int speedchecker = position;
      int possibleMove = 1;
      if (newspeed > _max_road_speed_)
      {
        continue;
      }  
      // Check if the next newspeed*(newspeed+1)/2 indexes are above speed limit  
      for(int j = 0; j <= newspeed; j++){
        if (possibleMove == 0)
        { 
          break;          
        }
        int maxspeed = newspeed - j; // calculate max speed to check for the next maxspeed tiles  
        if (maxspeed < _min_road_speed_){
          maxspeed = _min_road_speed_;
        }  
        for(int k = 0; k < newspeed-j; k++){
          speedchecker++; // speedchecker will be used as the index to access the array of the road
          if (speedchecker > final_position && final_position > 1)
          {
            saveArr = 1;
          }  
          if(max_road_speed[speedchecker] < maxspeed || speedchecker > final_position)
          {
            possibleMove = 0;
          }
          
        }
      }
      if (saveArr == 1 && saved == 0)
      { 
        saved = 1;
        for (int i = 0; i <= final_position; i++)
        {
          solution_4_before.positions[i] = i < move_number ? solution_4.positions[i] : 0;
        }
        solution_4_before.n_moves = move_number-1; 
        last_position = solution_4_before.positions[move_number-1];
        last_speed = solution_4_before.positions[move_number-1]-solution_4_before.positions[move_number-2];
      }
      if (possibleMove == 1){
        solve_4_dyn(move_number+1, position+newspeed, newspeed, final_position);
        return;
      } 
    }
  }
}

static void solve_4(int final_position)
{
  if(final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr,"solve_4: bad final_position\n");
    exit(1);
  }
  solution_4_elapsed_time = cpu_time();
  solution_4_count = 0ul;
  solution_4_best.n_moves = final_position + 100;
  solution_4.n_moves = solution_4_before.n_moves;
  for (int i = 0; i <= final_position; i++)
  {
    solution_4.positions[i] = solution_4_before.positions[i];
  }
  saveArr = 0;
  saved = 0;
  solve_4_dyn(solution_4_before.n_moves,last_position,last_speed,final_position);
  solution_4_elapsed_time = cpu_time() - solution_4_elapsed_time;
}
//
// main program
//

//
// example of the slides
//

static void example(void)
{
  int i,final_position;

  srandom(0xAED2022);
  init_road_speeds();
  final_position = 30;
  solve_1(final_position);
  make_custom_pdf_file("example.pdf",final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
  printf("mad road speeds:");
  for(i = 0;i <= final_position;i++)
    printf(" %d",max_road_speed[i]);
  printf("\n");
  printf("positions:");
  for(i = 0;i <= solution_1_best.n_moves;i++)
    printf(" %d",solution_1_best.positions[i]);
  printf("\n");
}


int main(int argc,char *argv[argc + 1])
{
# define _time_limit_  3600.0
  int n_mec,final_position,print_this_one;
  char file_name[64];

  // generate the example data
  if(argc == 2 && argv[1][0] == '-' && argv[1][1] == 'e' && argv[1][2] == 'x')
  {
    example();
    return 0;
  }
  // initialization
  n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
  srandom((unsigned int)n_mec);
  init_road_speeds();
  // run all solution methods for all interesting sizes of the problem
  final_position = 1;
  solution_1_elapsed_time = 0.0;
  printf("    + --- ---------------- --------- +\n");
  printf("    |                plain recursion |\n");
  printf("--- + --- ---------------- --------- +\n");
  printf("  n | sol            count  cpu time |\n");
  printf("--- + --- ---------------- --------- +\n");
  while(final_position <= _max_road_size_/* && final_position <= 20*/)
  {
    
    print_this_one = (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
    printf("%3d |",final_position);
    // first solution method (very bad)
    #ifdef PROFREC
    if(solution_1_elapsed_time < _time_limit_)
    {
      solve_1(final_position);
      if(print_this_one != 0)
      {
        sprintf(file_name,"%03d_1_%s.pdf",final_position,argv[1]);
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_1_best.n_moves,&solution_1_best.positions[0],solution_1_elapsed_time,solution_1_count,"Plain recursion");
      }
      printf(" %3d %16lu %9.3e |",solution_1_best.n_moves,solution_1_count,solution_1_elapsed_time);
      FILE *fp1;
      char file_name1[64];
      sprintf(file_name1,"results_1_%s.txt",argv[1]);
      fp1 = fopen(file_name1, "w+");
      fprintf(fp1, "%d %f\n", final_position, solution_1_elapsed_time);
      fclose(fp1);
    }
    else
    {
      solution_1_best.n_moves = -1;
      printf("                                |");
    }
    #endif
    // second solution method (less bad)
    // ...
    #ifdef FASTWAY
      solve_2(final_position);
      if(print_this_one != 0)
        {
          sprintf(file_name,"%03d_2_%s.pdf",final_position,argv[1]);
          make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_2_best.n_moves,&solution_2_best.positions[0],solution_2_elapsed_time,solution_2_count,"Fastest Way Possible");
        }
        printf(" %3d %16lu %9.3e |",solution_2_best.n_moves,solution_2_count,solution_2_elapsed_time);
        FILE *fp2;
        char file_name2[64];
        sprintf(file_name2,"results_2_%s.txt",argv[1]);
        fp2 = fopen(file_name2, "a");
        fprintf(fp2, "%d %f\n", final_position, solution_2_elapsed_time);
        fclose(fp2);
    #endif

    #ifdef PROFREC2
        if(solution_3_elapsed_time < _time_limit_)
    {
      solve_3(final_position);
      if(print_this_one != 0)
      {
        sprintf(file_name,"%03d_3_%s.pdf",final_position,argv[1]);
        make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_3_best.n_moves,&solution_3_best.positions[0],solution_3_elapsed_time,solution_3_count,"Plain recursion 2.0");
      }
      printf(" %3d %16lu %9.3e |",solution_3_best.n_moves,solution_3_count,solution_3_elapsed_time);
      FILE *fp3;
      char file_name3[64];
      sprintf(file_name3,"results_3_%s.txt",argv[1]);
      fp3 = fopen(file_name3, "a");
      fprintf(fp3, "%d %f\n", final_position, solution_3_elapsed_time);
      fclose(fp3);
    }
    else
    {
      solution_3_best.n_moves = -1;
      printf("                                |");
    }
    #endif
    #ifdef DYNAPR
      solve_4(final_position);
      if(print_this_one != 0)
        {
          sprintf(file_name,"%03d_4_%s.pdf",final_position,argv[1]);
          make_custom_pdf_file(file_name,final_position,&max_road_speed[0],solution_4_best.n_moves,&solution_4_best.positions[0],solution_4_elapsed_time,solution_4_count,"Dynamic Approach");
        }
        printf(" %3d %16lu %9.3e |",solution_4_best.n_moves,solution_4_count,solution_4_elapsed_time);
      FILE *fp4;
      char file_name4[64];
      sprintf(file_name4,"results_4_%s.txt",argv[1]);
      fp4 = fopen(file_name4, "a");
      fprintf(fp4, "%d %f\n", final_position, solution_4_elapsed_time);
      fclose(fp4);
    #endif
    // done-
    printf("\n");
    fflush(stdout);
    // new final_position
    if(final_position < 50)
      final_position += 1;
    else if(final_position < 100)
      final_position += 5;
    else if(final_position < 200)
      final_position += 10;
    else
      final_position += 20;
  }
  printf("--- + --- ---------------- --------- + --- ---------------- --------- \n");
  return 0;
# undef _time_limit_
}
