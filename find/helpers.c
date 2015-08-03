/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int upper = n - 1;
    int lower = 0;
    
    while ((upper - lower) > 0)
    {
        int current = (upper + lower) / 2;
        if (values[current] == value)
        {
            return true;
        }
        else if (values[current] > value)
        {
            upper = current;
        }
        else
        {
            lower = current;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int swap;
    // Iterate over array until no swaps are made.
    do
    {
        swap = 0;
        // Iterate over entire array each time
        for (int i = 1; i < n; i++)
        {
            // If the left value is greater than the right, swap!
            if (values[i] < values[i - 1])
            {
                int temp = values[i];
                values[i] = values[i-1];
                values[i-1] = temp;
                
                swap++;
            }
        }
    }
    while (swap > 0);   
}
