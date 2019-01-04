//
//  Exercise_9.cpp
//  ProgramDesignTools_TP
//
//  Created by Jérôme COFFIN on 21/12/2018.
//  Copyright © 2018 Jérôme COFFIN. All rights reserved.
//

#include "Exercise_9.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <ctype.h>


class analyseur_lexical;



class analyseur_lexical {
    
private:
    int is_conditionnelle; //est mis a 1 si conditionnelle, sinon 0
    int is_affectation; //est mis a 1 si affectation, sinon 0
    std::vector<std::string> vec; //vecteur contenant chaque terme du string d'entree

public:
    
    //afficher un tableau 2D de string
    int print_stringArray(const std::string& expr){
        
        for (int i = 0; i < 4; i++) {
            
            std::cout << expr[i] << "\n";
            
        }
        
        return 0;
        
    }
    
    
    //initialiser les attributs is_affectation et is_conditionnelle
    int affectation_conditionnelle(const std::string& expr){
        
        if(expr[0]=='?'){
            
            is_conditionnelle = 1;
            is_affectation = 0;
            return 1;
            
        }
        
        else {
            
            int i;
            for(i=0; i<expr.length(); i++){
                
                if(expr[i]=='='){
                    
                    is_conditionnelle = 0;
                    is_affectation = 1;
                    return 1;
                    
                }
                
            }
            
        }
        
        return 0;
    }
    
    
    //nombre de terme dans l'instruction
    long count_words(std::string const& str){
        
        std::stringstream stream(str);
        return std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
    }
    
    //afficher chaque terme de l'instruction
    void display_words(std::string const& str){
        
        std::istringstream iss(str);
        copy(std::istream_iterator<std::string>(iss),
             std::istream_iterator<std::string>(),
             std::ostream_iterator<std::string>(std::cout, "\n"));
    }
    
    //mettre tous les termes d'une instruction dans un vecteur
    void store_words(std::string const& str){
        
        std::istringstream ss(str);
        std::string token;
        
        while(std::getline(ss, token, ' ')) {
            vec.push_back(token);
        }
        
    }
    
    //retourne un terme de l'instruction grace au vecteur
    std::string display_vec(int val){
        
        if(val>vec.size()-1){
            
            std::cout << "trop loin\n";
            return "NULL";
            
        }
        
        return vec[val];
        
    }
    
    //retourne 1 si un terme est un identifcateur
    int is_identificateur(std::string const& terme){
        
        if(isalpha(terme[0])!=0 && isalpha(terme[1])==0){
            
            return 1;
        }
        
        return 0;
    }
    
    //retourne 1 si un terme est un nombre
    int is_nombre(std::string const& terme){
        
        int i=0;
        for(i=0; i<terme.size(); i++){
            
            if(isdigit(terme[i])==0){
                return 0;
            }
        }
        
        return 1;
    }
    
    //lancer une analyse lexicale
    void run(std::string const& instruction){
        
        store_words(instruction);
        int i=0;
        for(i=0; i<count_words(instruction)-1; i++){
            
            std::cout << display_vec(i);
            if (is_identificateur(vec[i])==1){
                
                std::cout << " ::= identificateur\n";
            }
            
            else if (is_nombre(vec[i])==1){
                
                std::cout << " ::= nombre\n";
            }
            
            else{
                
                std::cout << " ::= autre\n";
            }
            
        }
        
    }
    
};




int exercise_9() {
    
    analyseur_lexical analyseur;
    std::string instruction = "A1 = A * 100 + 2 * B";
    
    analyseur.store_words(instruction);
    
    analyseur.run(instruction);
    
    return 0;
}
