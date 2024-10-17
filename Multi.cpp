/* This example demonstrates how to use multiple threads with mutex to safely access and modify shared data
           & Vector and Map for dynamic array and accessing arrays with keyes
                      & conio.h for Keyboard input while runtime                                                                    
                             & Chrono for time management                                                        */

#include "Multi.h"



//  Addition Method from Multi
void Multi :: add ( int a , int b , std::string index ) {

    // Lock_gaurd is like smart ptr , they will unlock when ever they go out of scope / if not using lock_guard , we should manually call .lock() and .unlock() on our mutex .
    // out of scope = When the execution leaves its block (e.g., a function, loop, or conditional statement) , the variable goes out of scope, and its destructor is automatically called .
    // Locking the mutex , ensures the mutex is locked then no other Thread could not access the shared data = protecting the shared data.
    std::lock_guard<std::mutex> lock ( this-> mtx );
                                              
    this->Results [index] = a + b;
}



//  Keyboard input Method , while we are using it from a Thread the data inside this method will setted to the object we sent via Thread calling this method (Multi)
//  Hold Key System applied
void Multi::input ()
{
    while ( Multi::IsRunning )
    {
        std::lock_guard<std::mutex> lock ( this->mtx );     // This is not Necessary Here , Because we need it only when there is more than one Thread Accessing A Method .
        if ( _kbhit () )    // _kbhit() is for getting input while runtime without stopping the Console , means = Keyboard Hit .
        {
            Multi::Key = _getch ();   // Get Character from Keyboard hit

            if ( Multi::Key == 's' )
            {
                Multi::Run = true;
            }
            
            else if ( Multi::Key == 27 ) // 27 is Equivalent to Esc Key on keyboard , this will let program to quit .
            {
                Multi::IsRunning = false;
            }
        }
        else
        {
          Multi::Run = false;
        }
    }

}



//  Merthod for output Texts
void Multi::Output (double DeltaTime , std::map <std::string , int >  Results , std::map <std::string , int >  VeciResults )
{
    //  Calculate Delta Time for user systems CPU .
    std::cout << "   ===== " << "Systems Delta Time =>" << DeltaTime << " ===== \n";
    std::cout << "   * " << "*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*" << " * \n";

    //  Show Addition
    for ( auto i = Results.begin (); i != Results.end (); i++ ) {
        std::cout << "   *  Result of addition    " << i->first << i->second << "  *\n";
    }

    //  line 
    std::cout << "   * " << "*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*" << " * \n";
    std::cout << "   *************************************** \n";
    std::cout << "   * " << "*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*" << " * \n";

    //  Show Subtraction
    for ( auto i = VeciResults.begin (); i != VeciResults.end (); i++ )
    {
        std::cout << "   *  Result of Subtraction " << i->first << i->second << "  *\n";
    }

    //  line 
    std::cout << "   * " << "*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*" << " * \n\n";
}





//  MultiVec Methods
//  Subtracion Method from MultiVec
void MultiVec::Subtraction ( int a , int b , std::string index ){

    std::lock_guard<std::mutex> lock ( this-> mtx );

    this->Results [index] = a - b;
}



//  Calculate DeltaTime
double MultiVec::DeltaTime ()
{
    // For Calculating Delta Time
    // The Theory is => when we start two times , the time that will spend till the second call by system Hardware will be Delta time of the System .
    std::chrono::duration<double , std::milli> DeltaSecond = std::chrono::duration<double , std::milli> ( this->End - this->Start );
    return DeltaSecond.count ();
}



//  By This Method we will set a Time
//  Using Refrenced Method is so much important , Because it will change data from variable we pass to it .
void MultiVec::SetDelay ( std::chrono::steady_clock::time_point & DelayStart )
{
    std::lock_guard<std::mutex> lock ( this->mtx );

    DelayStart = std::chrono::high_resolution_clock::now ();
}



//  By This Method we will Calculate Delay from our Setted time and the exact moment we are when we call this
void MultiVec::Delay ( int MilliSeconds , std::chrono::steady_clock::time_point & DelayStartVec , int VectorValue )
{
    std::lock_guard<std::mutex> lock ( this->mtx );
    // I had hard time to undrestand how Chrono works exactly , But finally now i know = > Those Time points will work with System time , by setting them with new() we will not set a ( 0 ) time , ...
        // Instead we will save our system time at that point , in here by SetDelay() we will set a specific time from that moment of our system , then we will subtract times from system times
            // at each frame from time point we setted earlier , after we will count it and convert it to milliseconds , we will check if it reaches MilliSeconds we want or not .
    std::chrono::steady_clock::time_point Now = std::chrono::high_resolution_clock::now ();

    auto Duration = std::chrono::duration_cast<std::chrono::milliseconds> ( Now - DelayStartVec ).count();

    if ( Duration >= MilliSeconds )
        this->MoreThanDuration[VectorValue] = true;
    else
        this->MoreThanDuration[VectorValue] = false;
}



int main () {
    //  Objects
    Multi Multi = {};
    MultiVec Veci = {};

    // Modify This To = > Set Data for Numbers inside Calculations
    int First1 = 5 , First2 = 15 , First3 = 25;
    int Second1 = 8 , Second2 = 18 , Second3 = 28;

    // Create three threads, each calling the add function with different arguments.
    std::thread t1 ( &Multi::add , &Multi , First1 , Second1 , "First  : "  );  // for Calling methods inside Classes
    std::thread t2 ( &Multi::add , &Multi , First2 , Second2 , "Second : "  );
    std::thread t3 ( &Multi::add , &Multi , First3 , Second3 , "Third  : "  );

    // Ensure the main thread waits for each of the three threads to complete.
    // '.join ()' on a thread is a way to wait for that thread to finish its execution before the program continues.
    t1.join ();    
    t2.join ();
    t3.join ();
    
    // Setting Veci Index Data
    Veci.IndexController [0] = "First  : ";
    Veci.IndexController [1] = "Second : ";
    Veci.IndexController [2] = "Third  : ";

    // Setting Veci Vector Data by push_back()
    Veci.FirstNums.push_back ( First1 );
    Veci.FirstNums.push_back ( First2 );
    Veci.FirstNums.push_back ( First3 );

    // Setting Veci Vector Data by insert()
    Veci.SecondNums.insert ( Veci.SecondNums.begin().operator+(0) , Second1 );
    Veci.SecondNums.insert ( Veci.SecondNums.begin().operator+(1) , Second2 );
    Veci.SecondNums.insert ( Veci.SecondNums.begin().operator+(2) , Second3 );

    //  Create Vec for Threads to subtract
    for ( unsigned int i = 0u; i < Veci.ThreadNum ; i++ )
    {
        Veci.VecThread.push_back( std::thread ( &MultiVec::Subtraction , &Veci , Veci.FirstNums[i] , Veci.SecondNums [i] , Veci.IndexController [i]));
    }

    //  VecThreads Wating
    for ( unsigned int i = 0u; i < Veci.ThreadNum ; i++ )
    {
        Veci.VecThread.at(i).join();
    }

    // Input taker Thread
    Multi.Input = std::thread (&Multi::input , &Multi);
 
    //  Setup Delay system 
    std::chrono::steady_clock::time_point timepoint = {};
    Veci.DelayStartVec.push_back ( timepoint );
    Veci.DelayStartVec.push_back ( timepoint );

    Veci.TimeVecThread.push_back ( std::thread ( &MultiVec::SetDelay , &Veci , std::ref ( Veci.DelayStartVec.at ( 0 ) ) ) );  //  Dont Forget to use std::ref() while Method wants a Refrence as parameter
    Veci.TimeVecThread.push_back ( std::thread ( &MultiVec::SetDelay , &Veci , std::ref ( Veci.DelayStartVec.at ( 1 ) ) ) );  //  with Threads .
    
    Veci.TimeVecThread.at ( 0 ).join ();    // We can not use our previous threads again for different method call , so we need to create new ones for calling Different Methods
    Veci.TimeVecThread.at ( 1 ).join ();
    
    Veci.TimeVecThread.erase ( Veci.TimeVecThread.begin () + 0 );   //  To clear our previous Threads and add new ones but not with 3 & 4 , and with same previous Memory Space .
    Veci.TimeVecThread.erase ( Veci.TimeVecThread.begin () + 0 );   //  We also Could use .Clear() on our vectors , but this is more Readable .

    //Veci.TimeVecThread.erase ( Veci.TimeVecThread.begin () + 1 );   Using this will give us out of range error , because when we did earase our first vector then the second will be our new first .

    //Veci.SetDelay ( Veci.DelayStartVec.at ( 0 ) );    setup without MultiThreading
    //Veci.SetDelay ( Veci.DelayStartVec.at ( 1 ) );

    while ( Multi.IsRunning )
    {
        std::cout << std::flush;       //  important , other wise program will use previuous couts again , it will Empty cout buffer .

        //  Delay with Time setup as Vector to have different Delaies in our Program !
        //  Using Vectors here is not wise and we will face lots of Memory leak because using of .push_back() and increasing our Vectors
        //  Veci.TimeVecThread.push_back ( std::thread ( &MultiVec::Delay , &Veci , 800 , std::ref ( Veci.DelayStartVec.at ( 0 ) ) , 0 ) );
        
        //  Also Using Threads in a While loop was not a good idea and make program Glitchy
        //std::thread TempT ( &MultiVec::Delay , &Veci , 800 , std::ref ( Veci.DelayStartVec.at ( 0 ) ) , 0 );  
        //TempT.join ();

        Veci.Delay ( 800 , Veci.DelayStartVec.at ( 0 ) , 0 );
        if ( Veci.GetDelayStatus( 0 ) )
        {
            std::cout << "            | Delay is Working | \n         | Use 'S' Key On Keyboard | \n\n";
            Veci.SetDelay ( Veci.DelayStartVec.at ( 0 ) );
        }

        Veci.Delay ( 8000 , Veci.DelayStartVec.at ( 1 ) , 1 );
        if ( Veci.GetDelayStatus ( 1 ) )
        {
            system ("CLS");
            std::cout << "  Console Will Clear every 8 Seconds to avoid Text Overflow :) \n\n";
            Veci.SetDelay ( Veci.DelayStartVec.at ( 1 ) );
        }

        // Use S key on keyboard while run time to Enter Loop
        while ( Multi.Run )
        {
            //  Method to Call Couts for Texts
            Multi.Output ( Veci.DeltaTime () , Multi.Results , Veci.Results );
        
            if ( ! Multi.IsRunning ) //  this if is important , because when inside the loop if we hit Esc , it would be possible for software to crash and not know what should it do .
            {
                break;
                break;
            }
        }
    }

    Multi.Input.join (); // Be sure to join() the threads after using them and before Quiting the Software we make .

    return 0;
}