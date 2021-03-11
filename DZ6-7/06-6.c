#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0;

void *mythread(void *dummy)

/*
 *   Parameter void *dummy is used only for type compatibility, and will not be used
 *   at the function now. At the same reason the return value of function has type 
 *   void *, but it wiil be not used at this program.
 */

{

   pthread_t mythid;
   
   mythid = pthread_self();

   a = a+1;

   printf("Thread %u, Calculation result = %d\n", (unsigned int)mythid, a);

   return NULL;
}

void *mythread_second(void *dummy)
{

   pthread_t mythid;
   
   mythid = pthread_self();

   a = a+1;

   printf("Thread %u, Calculation result = %d\n", (unsigned int)mythid, a);

   return NULL;
}

int main()
{
   pthread_t thid, mythid, thid_second; 
   int       result;

   result = pthread_create( &thid, (pthread_attr_t *)NULL, mythread, NULL);
   
   if(result != 0){
      printf ("Error on first thread create, return value = %d\n", result);
      exit(-1);
   }   

   printf("First thread created, thid = %u\n", (unsigned int)thid);

   result = pthread_create( &thid_second, (pthread_attr_t *)NULL, mythread_second, NULL);
   
   if(result != 0){
      printf ("Error on second thread create, return value = %d\n", result);
      exit(-1);
   } 

   printf("Second thread created, thid = %u\n", (unsigned int)thid_second);

   mythid = pthread_self();
   
   a = a+1;
   
   printf("Thread %u, Calculation result = %d\n", (unsigned int)mythid, a);

   pthread_join(thid, (void **)NULL);
   pthread_join(thid_second, (void **)NULL);

   return 0;
}
