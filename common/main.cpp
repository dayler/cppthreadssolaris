/* 
 * File:   main.cpp
 * Author: dayler
 *
 * Created on May 4, 2015, 4:31 PM
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include "Runnable.hpp"
#include "SimpleRunnable.hpp"
#include "Thread.hpp"

using namespace std;

/*
 * 
 */
int main() {
    cout<<"Begin main test class..."<<endl;
    
    Runnable* r1 = new SimpleRunnable(1, 3);
    Runnable* r2 = new SimpleRunnable(2, 8);
    
    Thread* t1 = new Thread(r1, false);
    Thread* t2 = new Thread(r2, false);
    
    // Start threads
    t1->start();
    t2->start();
    
    // Join threads
    long res1 = reinterpret_cast<long>(t1->join());
    long res2 = reinterpret_cast<long>(t2->join());
    
    cout<<"Result t1 = "<<res1<<endl;
    cout<<"Result t2 = "<<res2<<endl;
    
    // Delete objects
    delete r1;
    delete t1;
    
    delete r2;
    delete t2;
    
    cout<<"Finish main test class..."<<endl;
    return 0;
}

