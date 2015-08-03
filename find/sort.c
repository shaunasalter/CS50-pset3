void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    int swap;
    // Iterate over array until no swaps are made.
    do
    {
        swap = 0;
        // Iterate over entire array each time
        for (int i = 0; i < n; i++)
        {
            // If the left value is greater than the right, swap!
            if (values[i + 1] < values[i])
            {
                int temp = values[i];
                values[i] = values[i+1];
                values[i+1] = temp;
                
                swap++;
            }
        }
    }
    while (swap > 0);   
}
