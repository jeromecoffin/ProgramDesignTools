//
//  Exercise_7.cpp
//  ProgramDesignTools_TP
//
//  Created by Jérôme COFFIN on 26/11/2018.
//  Copyright © 2018 Jérôme COFFIN. All rights reserved.
//

#include "Exercise_7.hpp"

//factorial function
double factorial(int n) {
    
    if(n>1){
        return n*factorial(n-1);
    }
    return 1;
}

//power function
double pow(int x, int n)
{
    if (n != 0)
        return (x*pow(x, n-1));
    else
        return 1;
}

//exponential function
float exp(int x, int n){
    
    if(n>0){
        return exp(x, n-1)+ (pow(x, n)/factorial(n));
    }
    return 1;
}

int exercice_7(){
    
    float exponential = 0;
    exponential = exp(1, 20);
    std::cout << exponential << std::endl;
    
    return 0;
}
