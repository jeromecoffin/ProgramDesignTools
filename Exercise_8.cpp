//
//  Exercise_8.cpp
//  ProgramDesignTools_TP
//
//  Created by Jérôme COFFIN on 26/11/2018.
//  Copyright © 2018 Jérôme COFFIN. All rights reserved.
//


#include <iostream>
#include "Exercise_8.hpp"
using namespace std;

//lire l'entree
void lecture(){
    
    string calcul;
    cout << "Input : " << endl;
    cin >> calcul;
    
    char *array = new char;
    
    strcpy(array, calcul.c_str());
    
    cout << array;
    
}

//addition
int addition(int a, int b){
    return a+b;
}

//soustraction
int soustraction(int a, int b){
    return a-b;
}

//multiplication
int multiplication(int a, int b){
    return a*b;
}

//division
int division(int a, int b){
    if(b==0){
        return 0;
    }
    return a/b;
}

