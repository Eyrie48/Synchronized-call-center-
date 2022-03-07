//Edward Aleman 006884989
//CSE 4600 section 1 
//11/27/2020
 
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctime>

using namespace std;

sem_t operators;
pthread_mutex_t mutex;
int operatorSize = 3;
const int SIZE = 5;

//check for operator availability
void check_OperatorSize()
{
   if(operatorSize > 0){
      cout << "Operator availability: " << operatorSize << endl;
      //sem_post(&available);     
   }
   else{
      cout << "All operators are currently with other customers...pls wait " << endl; 
   }
}

//simulate call going through 
void* answer(string *name)
{
   sem_wait(&operators);
   cout << *name << " is getting the conection to the operator..." << endl;
   
   pthread_mutex_lock(&mutex);
   --operatorSize;
   pthread_mutex_unlock(&mutex);
   
   check_OperatorSize();
}

//simulate finishing call 
void* finished(string *name){
   sem_post(&operators);
   
   pthread_mutex_lock(&mutex);
   ++operatorSize;
   pthread_mutex_unlock(&mutex);
   
   cout << *name << " is now ending the call..." << endl;
   check_OperatorSize();
}

void* callCenter(void *people)
{
   int seconds = (rand() % 5 + 1);
   string *name = (string *)people;
   answer(name);
   
   //random time for call
   sleep(seconds);
   //cout << "\nThis is the time it took " << seconds << endl;
   
   finished(name);
   //check_OperatorSize();
   //sem_post(&available);
}

int main()
{
   //initialize call center
   sem_init(&operators, 0, operatorSize);
   pthread_mutex_init(&mutex, NULL);
   
   string people[] = {"Alice", "Bob", "John", "Mark", "Alex"};
   
   cout << "Welcome to the call center. Please wait for the setup to start..." << endl; 
   
   check_OperatorSize();

   //threads
   pthread_t tid[SIZE];
   
   //create threads
   for(int i = 0; i < SIZE; i++)
   {   
      pthread_create(&tid[i], NULL, callCenter, &people[i]);
      cout << people[i] << " is waiting for an operator...." << endl;
   }
   
   check_OperatorSize();
   
   //wait for threads 
   for(int i = 0; i < SIZE; i++)
   {
      pthread_join(tid[i], NULL);
   }
   
   return 0;
}   








