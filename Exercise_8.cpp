//
//  Exercise_8.cpp
//  ProgramDesignTools_TP
//
//  Created by Jérôme COFFIN on 26/11/2018.
//  Copyright © 2018 Jérôme COFFIN. All rights reserved.
//

#include "Exercise_8.hpp"
#include <iostream> //Standard input/output
#include <stack> //Constructs a stack container adaptor object
#include <vector> //Construit un vecteur
#include <string> //Construit un objet de type String
#include <cstdlib> //Defines several general purpose functions
#include <map> //Constructs a map (associative containers that store elements) container object
#include <stdexcept> //Defines a set of standard exceptions


//On utilise C++ pour la POO
//On a un objet calculatrice qui admet des equations en NPI
//Ces equations sont presentees sous forme de pile
// NPI : notation polonaise inverse



class Calculatrice; //Objet calculatrice
class NPIExpression; //Objet de forme NPI
class ShuntingYard; //Objet avec les proprietes de Shunting Yard




//represente une unite pour NPI
struct BaseUnit {
    
    //BaseToken est une dependance de Calculatrice
    virtual void evaluer (Calculatrice *) = 0;
    virtual ~BaseUnit() {}
    
};



//Les operateurs sont de type Unit< char >
//Les operands sont de type Unit< double >
//Template car on ne connait pas le format
template< class U > class Unit : public BaseUnit {
    
    U unit_;
    
public:
    //Permettre a la calculatrice d'utiliser un unit
    void evaluer (Calculatrice  *c);
    Unit (U u) : unit_(u) {}
    
};


//Represente une expression en NPI
//Cette objet est une queue FIFO de tokens

class NPIExpression {
    
    std::vector< BaseUnit* > pile_;
    
public:
    /* Ajout d'un unit */
    void push (BaseUnit *u) {
        
        pile_.push_back (u);
        
    }
    
    /* Utiliser le unit suivent depuis le debut de l'expression */
    BaseUnit* pop () {
        BaseUnit *u = pile_.front ();
        pile_.erase (pile_.begin ());
        return u;
    }
    
    bool empty () const {
        
        return pile_.empty ();
        
    }
};


//convertir en NPI avec la methode de Shunting-Yard

class ShuntingYard {
    
    const std::string expr_; //constante non modifiee
    NPIExpression npi_;
    std::stack< char > op_stack_;
    mutable std::map< char, int > op_precedent_; //permet de modifier sans alterer le const
    
    int precedent (char op) const {
        
        return op_precedent_[op];
        
    }
    
    int stack_precedent () const {
        
        if (op_stack_.empty ()) {
            
            return -1;
            
        }
        return precedent (op_stack_.top ());
        
    }
    
    //effectuer un push pour reset
    void gestion_parenthese_ouvrante () {
        
        op_stack_.push ('(');
        
    }
    
    //Ici meme operation pour parenthese fermante
    void gestion_parenthese_fermante () {
        
        while ('(' != op_stack_.top ()) {
            
            npi_.push (new Unit< char >(op_stack_.top ()));
            op_stack_.pop ();
            
        }
        
        op_stack_.pop ();
    }
    
    //Ici on lit la pile operateur et on push jusqu'au bout
    void gestion_operateur (char op) {
        
        while (! op_stack_.empty () &&
               precedent (op) <= stack_precedent ()) {
            
            npi_.push (new Unit< char >(op_stack_.top ()));
            op_stack_.pop ();
            
        }
        
        op_stack_.push(op);
    }
    
    //Convertir en NPI grâce à ShuntingYard
    //On est vraiment sur l'adaptation de l'algorithme ShuntingYard en C avec la NPI
    NPIExpression convert(const std::string &infix) {
        
        const char * unit = infix.c_str ();
        
        while (unit && *unit) {
            
            while (*unit && isspace (*unit)) {
                
                ++unit;
                
            }
            
            if (! *unit) {
                
                break;
                
            }
            
            if (isdigit (*unit)) {
                
                char * unit_suivant = 0;
                npi_.push (new Unit< double >(strtod (unit, &unit_suivant)));
                unit = unit_suivant;
                
            }
            
            //si ce n'est pas un digit, alors c'est une parenthese ou un operateur
            else {
                
                char op = *unit;
                switch (op) {
                    case '(':
                        gestion_parenthese_ouvrante();
                        break;
                    case ')':
                        gestion_parenthese_fermante();
                        break;
                    default:
                        gestion_operateur(op);
                }
                ++unit;
            }
        }
        while (! op_stack_.empty ()) {
            
            npi_.push (new Unit< char >(op_stack_.top ()));
            op_stack_.pop ();
            
        }
        
        return npi_;
    }
    
public:
    
    ShuntingYard (const std::string& infix) : expr_(infix) {
        
        op_precedent_['('] = -1;
        op_precedent_['+'] = 2;
        op_precedent_['-'] = 2;
        op_precedent_['*'] = 3;
        op_precedent_['/'] = 3;
        
    }
    
    NPIExpression vers_npi () {
        
        return convert (expr_);
        
    }
};


/*
 Une calculatrice qui calcule l'expression en convertissant en NPI
 puis passe au calcul.
 */
class Calculatrice {
    
    //pile d'operands
    std::stack< double > operands_;
    
    double pop () {
        
        double d = operands_.top ();
        operands_.pop ();
        return d;
        
    }
    
    void push (double d) {
        
        operands_.push (d);
        
    }
    
    //retourner le dernier resultat - le haut de la pile operand
    double result () const {
        
        return operands_.top ();
        
    }
    
    //vider la pile operand
    void flush () {
        while (! operands_.empty ()) {
            operands_.pop ();
            
        }
    }
    
protected:
    
    //ajoute un opernd
    void consume(double value) {
        
        push (value);
        
    }
    
    //ajoute un operateur
    void consume(char op) {
        switch (op) {
            case '+':
                push (pop () + pop ());
                break;
            case '*':
                push (pop () * pop ());
                break;
            case '-':
            {
                double right = pop ();
                push (pop () - right);
            }
                break;
            case '/':
            {
                double right = pop ();
                push (pop () / right);
            }
                break;
            default:
                throw std::domain_error("Unknown Operator");
        }
    }
public:
    
    //place au calcul
    double calculate (const std::string& expr) {
        
        ShuntingYard shunting_yard(expr);
        NPIExpression npi = shunting_yard.vers_npi ();
        flush ();
        
        while (! npi.empty ()) {
            
            BaseUnit * unit = npi.pop ();
            unit->evaluer (this);
            delete unit;
            
        }
        
        return result ();
    }
    
    //Utilisation de la methode consumme au unit
    template< class U > friend class Unit;
};



template< class U > void Unit< U >::evaluer (Calculatrice * c) {
    
    c->consume (unit_);
    
}



int exercice_8 () {
    
    Calculatrice calcul;
    std::string equation = "5+5+3/6+(2/(1+4))*6";
    equation = "0"+equation; //ajouter un zero au debut pour valeur negative
    std::cout << calcul.calculate (equation) << std::endl;
    return 0;
    
}
