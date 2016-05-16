   #include <stdio.h>
   #include <stdlib.h>
   #define max_rows 10000000

   int array[max_rows];

   void main(int argc, char **argv) 
   {
      int i, num_rows;
      long int sum;

      //printf("please enter the number of numbers to sum: ");
      //scanf("%i", &num_rows);
      num_rows = 10000;
      if(num_rows > max_rows) {
         printf("Too many numbers.\n");
         exit(1);
      }

      /* initialize an array */

      for(i = 0; i < num_rows; i++) {
         array[i] = i+1;
      }

      /* compute sum */

      sum = 0;
      for(i = 0; i < num_rows; i++) {
         sum += array[i];
      }

      printf("The grand total is: %ld\n", sum);
   }