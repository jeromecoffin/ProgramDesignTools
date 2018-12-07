//
//  Exercise_8.cpp
//  ProgramDesignTools_TP
//
//  Created by Jérôme COFFIN on 26/11/2018.
//  Copyright © 2018 Jérôme COFFIN. All rights reserved.
//


#include <iostream> //Standard input/output
#include <stack> //Constructs a stack container adaptor object
#include <vector> //Constructs a vector
#include <string> //Constructs a String object
#include <cstdlib> //Defines several general purpose functions
#include <map> //Constructs a map (associative containers that store elements) container object
#include <stdexcept> //Defines a set of standard exceptions


//On utilise C++ pour la POO
//On a un objet calculatrice qui admet des equations en NPI
//Ces equations sont presentees sous forme de pile




class Calculatrice;


//represente un Token pour NPI
// NPI : notation polonaise inverse
struct BaseToken {
    
    //BaseToken est une dependance de Calculatrice
    virtual void evaluer (Calculatrice *) = 0;
    virtual ~BaseToken() {}
    
};



//Les operateurs sont de type Token< char >
//Les operands sont de type Token< double >
//Template car on ne connait pas le format
template< class Tok > class Token : public BaseToken {
    
    Tok token_;
public:
    //Permettre a la calculatrice d'utiliser un token
    void evaluer (Calculatrice  *c);
    Token (Tok t) : token_(t) {}
    
};



//Represente une expression en NPI
//Cette objet est une queue FIFO de tokens

class NPIExpression {
    
    std::vector< BaseToken* > pile_;
    
public:
    /* Ajout d'un token */
    void push (BaseToken *t) {
        
        pile_.push_back (t);
        
    }
    
    /* Utiliser le token suivent depuis le debut de l'expression */
    BaseToken* pop () {
        BaseToken *t = pile_.front ();
        pile_.erase (pile_.begin ());
        return t;
    }
    
    bool empty () const {
        
        return pile_.empty ();
        
    }
};


//convertir en NPI avec la methode de Shunting-Yard

class ShuntingYard {
    
    const std::string expr_;
    NPIExpression npi_;
    std::stack< char > op_stack_;
    mutable std::map< char, int > op_precedent_;
    
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
    void handle_left_paren () {
        
        op_stack_.push ('(');
        
    }
    /* Consume all operators in current scope and restore previous scope */
    void handle_right_paren () {
        
        while ('(' != op_stack_.top ()) {
            
            npi_.push (new Token< char >(op_stack_.top ()));
            op_stack_.pop ();
            
        }
        
        op_stack_.pop ();
    }
    
    /* Consume operators with precedence >= than op then add op */
    void handle_op (char op) {
        
        while (! op_stack_.empty () &&
               precedent (op) <= stack_precedent ()) {
            
            npi_.push (new Token< char >(op_stack_.top ()));
            op_stack_.pop ();
            
        }
        
        op_stack_.push(op);
    }
    
    //Convert infix to NPI via shunting-yard algorithm
    NPIExpression convert(const std::string &infix) {
        
        const char * token = infix.c_str ();
        
        while (token && *token) {
            
            while (*token && isspace (*token)) {
                
                ++token;
                
            }
            
            if (! *token) {
                
                break;
                
            }
            
            if (isdigit (*token)) {
                
                char * next_token = 0;
                npi_.push (new Token< double >(strtod (token, &next_token)));
                token = next_token;
                
            }
            else {
                
                char op = *token;
                switch (op) {
                    case '(':
                        handle_left_paren ();
                        break;
                    case ')':
                        handle_right_paren ();
                        break;
                    default:
                        handle_op (op);
                }
                ++token;
            }
        }
        while (! op_stack_.empty ()) {
            
            npi_.push (new Token< char >(op_stack_.top ()));
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
    
    NPIExpression to_npi () {
        
        return convert (expr_);
        
    }
};


/*
 Une calculatrice qui calcule l'expression en convertissant en NPI
 puis passe au calcul.
 */
class Calculatrice {
    
    std::stack< double > operands_;
    
    double pop () {
        
        double d = operands_.top ();
        operands_.pop ();
        return d;
        
    }
    
    void push (double d) {
        
        operands_.push (d);
        
    }
    
    /* Returns the most recent operation result (top of the operand stack) */
    double result () const {
        
        return operands_.top ();
        
    }
    
    /* Empty the operand stack */
    void flush () {
        while (! operands_.empty ()) {
            operands_.pop ();
            
        }
    }
    
protected:
    /* Process an operand token from the input stream */
    void consume(double value) {
        
        push (value);
        
    }
    
    /* Process an operator token from the input stream */
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
    /*
     Evaluate expression
     Note: Expression is expected to be in infix form.
     */
    double calculate (const std::string& expr) {
        
        ShuntingYard shunting(expr);
        NPIExpression npi = shunting.to_npi ();
        flush ();
        
        while (! npi.empty ()) {
            
            BaseToken * token = npi.pop ();
            token->evaluer (this);
            delete token;
            
        }
        
        return result ();
    }
    
    /* Expose the consume() methods to the Tokens */
    template< class T > friend class Token;
};


template< class T >
void Token< T >::evaluer (Calculatrice * c) { c->consume (token_); }





int exercice_8 () {
    
    Calculatrice calcul;
    std::string equation = "-4+(4-2)*3";
    equation = "0"+equation; //ajouter un zero au debut pour valeur negative
    std::cout << calcul.calculate (equation) << std::endl;
    return 0;
    
}
