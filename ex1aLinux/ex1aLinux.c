/*
* ex1.c
*
*  Created on: Nov 5, 2017
*      Author: eliyhots ,danna
*/
// --------------------- Include Section -----------------------
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>  // for wait()
#include <unistd.h>    // for sleep()
#include <sys/types.h>
#include <stdbool.h>
#include <time.h>
// ------------------------- Define ----------------------------
#define SIZE 10
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define THREE 3
// ------------------------ Prototype --------------------------
void set_numbers (int my_array[]);
void sort_array (int my_array[]);
void bubble_sort (int my_array[], int *min, int *max);
void quick_sort (int my_array[], int from, int to, int *min, int *max);
void binary_search (int my_array[]);
int partition (int my_array[], int from, int to);
void swap (int my_array[], int big, int small);
void serial_search (int my_array[]);
void do_parent (pid_t status[]);
void copy_array (int my_array[], int temp_array[]);
// -------------------------- Main -----------------------------
int main ()
{
	int my_array[SIZE];       // for numbers
	set_numbers (my_array);   // set numbers on array
	sort_array (my_array);
	return (EXIT_SUCCESS);
}
// ---------------------- set_numbers --------------------------
/*
* this function set numbers on the array whit srand/rand functions
*/
void set_numbers (int my_array[SIZE])
{
	int index;

	srand (time (NULL));
	for (index = 0; index < SIZE; index++)
		my_array[index] = rand () % SIZE;
}
// ---------------------- sort_array ---------------------------
/*
* this function getting int numbers and with fork function getting birth
* the childs sorting the array with bubble sort,quick_sort and serial_search
* the functions sort the array and print the time, min val , max val
*/
void sort_array (int my_array[SIZE])
{
	pid_t status[THREE];
	time_t start, end;
	int temp_array[SIZE];
	int index = 0;
	int min = SIZE, max = 0;

	copy_array (my_array, temp_array);

	for (index = 0; index < THREE; index++)
	{
		status[index] = fork ();
		if (status[index] < 0)
		{
			perror ("error in fork");
			exit (EXIT_FAILURE);
		}
		if (index == 0 && status[index] == 0)
		{
			start = time (NULL);
			bubble_sort (my_array, &min, &max);
			end = time (NULL);
			printf ("Bubble Sort: %d %d %d \n", (int)end - (int)start, min, max);
			binary_search (my_array);
			exit (EXIT_SUCCESS);
		}
		if (index == 1 && status[index] == 0)
		{
			start = time (NULL);
			quick_sort (temp_array, 0, SIZE - 1, &min, &max);
			end = time (NULL);
			printf ("Quick Sort: %d %d %d \n", (int)end - (int)start, min, max);
			exit (EXIT_SUCCESS);
		}
		if (index == 2 && status[index] == 0)
		{
			start = time (NULL);
			serial_search (my_array);
			end = time (NULL);
			printf ("Serial Search: %d\n", (int)end - (int)start);
			exit (EXIT_SUCCESS);
		}
	}
	do_parent (status);
}
// ---------------------- bubble sort --------------------------
/*
* this function getting array, min, max numbers,
* the function sorting the array whit bubble sort
* the function return the min,max numbers
*/
void bubble_sort (int my_array[], int *min, int *max)
{
	int round, place;

	for (round = 0; round < SIZE; round++)
	{
		for (place = 0; place < SIZE - round - 1; place++)
			if (my_array[place] > my_array[place + 1])
				swap (my_array, place, place + 1);
	}
	for (round = 0; round < SIZE; round++)
	{
		if (*min > my_array[round])
			*min = my_array[round];
		if (*max < my_array[round])
			*max = my_array[round];
	}
}
// ---------------------- binary_search ------------------------
/*
* this function getting array, min, max numbers,
* the function sorting the array whit bubble sort
* the function return the min,max numbers
*/
void binary_search (int my_array[])
{
	time_t start, end;
	pid_t status;
	int index;
	int hi = SIZE - 1;
	bool found = false;


	int wanted = rand () % SIZE;
	status = fork ();

	if (status < 0)
	{
		perror ("error in fork");
		exit (EXIT_FAILURE);
	}
	if (status == 0)
	{
		start = time (NULL);

		while (index <= SIZE - 1 && !found)
		{
			int mid = (index + hi) / 2;
			if (my_array[mid] == wanted)
				found = true;
			else if (my_array[mid] < wanted)
				index = mid + 1;
			else
				hi = mid - 1;
		}
		end = time (NULL);
		printf ("Binary Search: %d\n", (int)end - (int)start);
		exit (EXIT_SUCCESS);
	}
	wait (NULL);
}
// ----------------------- quick_sort --------------------------
/*
* this function getting array, min, max numbers,
* the function sorting the array whit quick_sort
* the function return the min,max numbers
*/
void quick_sort (int my_array[], int from, int to, int *min, int *max)
{
	int pivot_place = partition (my_array, from, to);

	if (from < pivot_place - 1)
		quick_sort (my_array, from, pivot_place - 1, min, max);

	if (to > pivot_place + 1)
		quick_sort (my_array, pivot_place + 1, to, min, max);

	if (*min > my_array[to])
		*min = my_array[to];
	if (*max < my_array[to])
		*max = my_array[to];

	if (*min > my_array[from])
		*min = my_array[from];
	if (*max < my_array[from])
		*max = my_array[from];
}
// ----------------------- partition ---------------------------
/*
* this function called by quick sort the function swap the numbers
*/
int partition (int my_array[], int from, int to)
{
	int index;
	int pivot_place = from;

	for (index = from + 1; index <= to; index++)
		if (my_array[index] < my_array[pivot_place])
		{
			swap (my_array, index, pivot_place + 1);
			swap (my_array, pivot_place + 1, pivot_place);
			pivot_place++;
		}
	return pivot_place;
}
// ------------------------- swap ------------------------------
/*
* this function getting int array, small number, big number
* the function swap the small and big numbers
*/
void swap (int my_array[], int big, int small)
{
	int temp = my_array[big];
	my_array[big] = my_array[small];
	my_array[small] = temp;
}
// ---------------------- serial_search ------------------------
/*
* this function getting int array, rand number,
* and search number on the array
*/
void serial_search (int my_array[])
{
	int index;
	bool found = false;

	int wanted = rand () % SIZE;

	for (index = 0; index < SIZE; index++)
		if (my_array[index] == wanted)
		{
			found = true;
			break;
		}
}
// ----------------------- do_parent ---------------------------
/*
* this function getting pid array and the function
* wait for allthe pid on the array[index]
*/
void do_parent (pid_t status[])
{
	int index;
	for (index = 0; index < THREE; index++)
		waitpid (status[index], NULL, 0);
	exit (EXIT_SUCCESS);
}
// ---------------------- copy_array ---------------------------
/*
* this function getting 2 int array and copy the
* "my array" to  temp array for the quick sort
*/
void copy_array (int my_array[], int temp_array[])
{
	int index;
	for (index = 0; index < SIZE; index++)
		temp_array[index] = my_array[index];
}