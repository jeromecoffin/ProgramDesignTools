//
//  main.cpp
//  ProgramDesignTools_TP
//
//  Created by Jérôme COFFIN on 26/11/2018.
//  Copyright © 2018 Jérôme COFFIN. All rights reserved.
//

#include <iostream>
#include "Exercise_7.hpp"
#include "Exercise_8.hpp"
#include "Exercise_9.hpp"

using namespace std;


//main function
int main(int argc, const char * argv[]) {
    
    cout << "--- CHOISIR UN EXERCICE : ---" <<endl;
    cout << "\n7 : pour exercice 7\n8 : pour exercice 8\n9 : pour exercice 9" <<endl;
    int choix = 0;
    cin >> choix;
    cout << "\n";
    
    switch (choix) {
        case 7:
            exercice_7();
            break;
            
        case 8:
            exercice_8();
            break;
            
        case 9:
            exercise_9();
            break;
            
        default:
            break;
    }
    
    cout << "\n";
    return 0;
}
