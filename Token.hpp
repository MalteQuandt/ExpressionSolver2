#pragma once
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>
#include <memory>

namespace tok
{
    class Token
    {
    public:
        std::string value;
        unsigned position;
        Token()
        {
            this->value = std::string("");
            this->position = 0;
        }
        Token(std::string &value, unsigned pos)
        {
            this->value = value;
            this->position = pos;
        }
        ~Token() = default;

    public:
        // Inlined methods:
        /**
         * Return the numerical value of the token
         **/
        inline std::string &getValue()
        {
            return this->value;
        }
        inline unsigned getPosition()
        {
            return this->position;
        }
        inline std::string toString()
        {
            return this->value + " at " + std::to_string(this->position);
        }
        // Overloadable functions:
        virtual inline unsigned getPrecedence() { return 0; };
        virtual inline bool isBinaryOperation() { return false; };
        virtual inline bool isUnaryOperation() { return false; };
        virtual inline bool isFunction() { return false; };
        virtual inline bool isLiteral() { return false; };
        virtual inline bool isParenthesis() { return false; };
        virtual inline bool isRightParen() { return false; };
        virtual inline bool isLeftParen() { return false; };
        virtual inline bool isTernaryOperation() { return false; };
        /** 
         * Take a vector of numbers and evaluate the value of it using the 
         * specified operation this class consists of.
         * */
        virtual double evaluate(std::vector<double>) { return 0x0; };
        inline unsigned consume(std::vector<tok::Token *> &tokens)
        {
            tokens.push_back(this);
            return 1;
        }
    };
}

namespace ev
{
    double eval(std::string);
    std::vector<tok::Token *> tokenization(std::string);
    std::vector<tok::Token *> infixtopostfix(std::vector<tok::Token *>);
    double evaluate(std::vector<tok::Token *>);
    unsigned consumeVar(std::string, unsigned, std::vector<tok::Token *> &);
    unsigned consumeLit(std::string, unsigned, std::vector<tok::Token *> &);
    void print(std::vector<tok::Token *>);
    inline bool isLetter(char pos)
    {
        return (pos >= 'a' && pos <= 'z') || (pos >= 'A' && pos <= 'Z');
    }
    inline bool isDigit(char pos)
    {
        return pos >= '0' && pos <= '9';
    }
    std::vector<tok::Token> tokenizationS(std::string expr);
    inline bool isHigherPrecedenceThan(tok::Token *one, tok::Token *two)
    {
        return one->getPrecedence() >= two->getPrecedence();
    }
    class Literal : public tok::Token
    {
    public:
        Literal(std::string value, unsigned position)
        {
            this->value = value;
            this->position = position;
        }
        bool inline isBinaryOperation() override
        {
            return false;
        }
        bool inline isUnaryOperation() override
        {
            return false;
        }
        bool inline isFunction() override
        {
            return false;
        }
        bool inline isLiteral() override
        {
            return true;
        }
        bool inline isParenthesis() override
        {
            return false;
        }
        bool inline isRightParen() override
        {
            return false;
        }
        bool inline isLeftParen() override
        {
            return false;
        }
        unsigned inline getPrecedence() override
        {
            return 0;
        }
        bool inline isTernaryOperation() override
        {
            return false;
        }
        double inline evaluate(std::vector<double> operands) override
        {
            return 0;
        }
    };
    class Parenthesis : public tok::Token
    {
    public:
        bool inline isBinaryOperation() override
        {
            return false;
        }
        bool inline isUnaryOperation() override
        {
            return false;
        }
        bool inline isFunction() override
        {
            return false;
        }
        bool inline isLiteral() override
        {
            return false;
        }
        bool inline isParenthesis() override
        {
            return true;
        }
        unsigned inline getPrecedence() override
        {
            return 0;
        }
        bool inline isTernaryOperation() override
        {
            return false;
        }
        double inline evaluate(std::vector<double> operands) override
        {
            return 0;
        }
    };
    class LPAREN : public Parenthesis
    {
    public:
        LPAREN(std::string value, unsigned position)
        {
            this->value = value;
            this->position = position;
        }
        bool inline isRightParen() override
        {
            return false;
        }
        bool inline isLeftParen() override
        {
            return true;
        }
    };
    class RPAREN : public Parenthesis
    {
    public:
        RPAREN(std::string value, unsigned position)
        {
            this->value = value;
            this->position = position;
        }
        bool inline isRightParen() override
        {
            return true;
        }
        bool inline isLeftParen() override
        {
            return false;
        }
    };
    class BinaryOp : public tok::Token
    {
    public:
        BinaryOp(std::string value, unsigned position) : tok::Token(value, position)
        {
        }
        bool inline isBinaryOperation() override
        {
            return true;
        }
        bool inline isUnaryOperation() override
        {
            return false;
        }
        bool inline isFunction() override
        {
            return false;
        }
        bool inline isLiteral() override
        {
            return false;
        }
        bool inline isParenthesis() override
        {
            return false;
        }
        bool inline isRightParen() override
        {
            return false;
        }
        bool inline isLeftParen() override
        {
            return false;
        }
        bool inline isTernaryOperation() override
        {
            return false;
        }
    };
    class ADD : public BinaryOp
    {
    public:
        ADD(std::string value, unsigned position) : BinaryOp::BinaryOp(value, position)
        {
        }
        unsigned inline getPrecedence() override
        {
            return 6;
        }
        double inline evaluate(std::vector<double> operands) override
        {
            if (operands.size() != 2)
                return 0.0;
            return operands.at(0) + operands.at(1);
        }
    };
    class SUB : public BinaryOp
    {
    public:
        SUB(std::string value, unsigned position) : BinaryOp::BinaryOp(value, position)
        {
        }
        unsigned inline getPrecedence() override
        {
            return 6;
        }
        double inline evaluate(std::vector<double> operands) override
        {
            if (operands.size() != 2)
                return 0.0;
            return operands.at(0) - operands.at(1);
        }
    };
    class MULT : public BinaryOp
    {
    public:
        MULT(std::string value, unsigned position) : BinaryOp::BinaryOp(value, position)
        {
        }
        unsigned inline getPrecedence() override
        {
            return 5;
        }
        double inline evaluate(std::vector<double> operands) override
        {
            if (operands.size() != 2)
                return 0.0;
            return operands.at(0) * operands.at(1);
        }
    };
    class DIV : public BinaryOp
    {
    public:
        DIV(std::string value, unsigned position) : BinaryOp::BinaryOp(value, position)
        {
        }
        unsigned inline getPrecedence() override
        {
            return 5;
        }
        double inline evaluate(std::vector<double> operands) override
        {
            if (operands.size() != 2)
                return 0.0;
            return operands.at(0) / operands.at(1);
        }
    };
}

#endif