#pragma once

 #include <iostream>     //  input / output Stream
 #include <conio.h>		 //	 To get input while runtime 
 #include <cstdlib>      //  To Use System Commands like = system("CLS");
 #include <string>		 //	 String Support (Array of char)
 #include <thread>       //  MultiThreading
 #include <vector>       //  Dynamic Fast Access Array
 #include <chrono>       //  Time Management Header
 #include <mutex>        //  Stoping Data Access for a shared data between threads
 #include <map>          //  Arrays with key possiblity to access the elements 

class Multi 
{
public:
	// declares a mutex to ensure thread-safe access.
	std::mutex mtx;     

	// only Taking 2 Arguments one for Key , one for Value and we can not assign value to it while not in runtime .
	std::map <std::string , int > Results;	

	// A Thread only to control Keyboard input While Runtime .
	std::thread Input = {};

	// Method for threads to add two values .
	void add ( int a , int b , std::string index );

	// Method to get Keyboard input while not blocking Processing .
	void input ( );

	// Merthod for outputing all Texts to Console .
	void Output (double DeltaTime , std::map <std::string , int >  Results , std::map <std::string , int >  VeciResults );

	// Needed Variables .
	bool IsRunning = true;
	bool Run = false;
	char Key = 'a';
};



class MultiVec : Multi
{
public:
	//	Constructor To set times 
	MultiVec () : Start ( std::chrono::steady_clock::now () ) , End ( std::chrono::steady_clock::now ()) {}

	//	Vectors
	std::vector <std::thread> VecThread = {}; 
	std::vector <std::thread> TimeVecThread = {}; 
	std::vector <int> FirstNums = {};
	std::vector <int> SecondNums = {};
	std::vector <std::chrono::steady_clock::time_point> DelayStartVec = {};

	//	Maps
	std::map <std::string , int> Results = {};	// only Taking 2 Arguments and we can not assign value to it while not in runtime .
	std::map <int , std::string> IndexController = {};

	// Function for threads to Subtract two values
	void Subtraction ( int a , int b , std::string index );

	// Method to Claculate Delta Time for user CPU
	double DeltaTime ();

	// Method for Simulating Delay to Start without blocking main processes
		// Using ( std::this_thread::sleep_for ( std::chrono::milliseconds ( 1000 ) ) )	=> will stop our programs main processes till time get ended , but its not good for a game engine or
			// softwares that need to do main task while having delay for some tasks .
	void SetDelay ( std::chrono::steady_clock::time_point & DelayStartVec );
	void Delay (int MilliSeconds , std::chrono::steady_clock::time_point & DelayStartVec , int VectorValue );

	// Friend Method to Tell us if our Delay is finished or not
	bool GetDelayStatus ( int VectorValue ) { return this->MoreThanDuration[VectorValue]; }

	// Variable to set how many threads do we want to simulate , it will only work for Vectors setup , See the ( For Loops ) inside Main function .
	unsigned const int ThreadNum = 3u;

	//	Destructor To Release all of our Vectors data lead to have more memory friendly Software .
	~MultiVec ()
	{
		this->VecThread.clear ();
		this->TimeVecThread.clear ();
		this->FirstNums.clear ();
		this->SecondNums.clear ();
		this->DelayStartVec.clear ();
	}

private:
	//	Chrono Timers
	std::chrono::steady_clock::time_point Start = {} , End = {};

	bool MoreThanDuration [2] = { false , false };	// if we want to have more delay in our loop we should increase it .
};

						/*			MIT License			*/
/* (( Thank you for Reading this Code , I hope it Helps you undrestand new things )) */
					  /*    By Mahmoud Karimi Zarandi 	  */
/*    Contact => Linkedin = www.linkedin.com/in/mahmoud-karimi-zarandi-7631a61bb     */	
					/*  Wish you all the BESTS Dear Reader  */