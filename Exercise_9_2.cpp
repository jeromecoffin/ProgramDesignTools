//
//  Exercise_9_2.cpp
//  ProgramDesignTools_TP
//
//  Created by Jérôme COFFIN on 21/12/2018.
//  Copyright © 2018 Jérôme COFFIN. All rights reserved.
//

#include "Exercise_9_2.hpp"
#include <string>


class analyseur_lexical;

struct unite {
    
    char *identificateur;
    double nb;
    
};

class analyseur_lexical {
    
public :
    double lire_string (const std::string& expr) {
        
        return 0;
    }
    
};


int exercise_9_2() {
    
    analyseur_lexical analyseur;
    std::string ligne = "A1 = A * 100 + 2 * B";
    analyseur.lire_string(ligne);
    
    return 0;
}
