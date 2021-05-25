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
    struct Token
    {

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
        virtual void parsetoInfix(tok::Token *&tok, std::vector<tok::Token*>& tokens, std::vector<tok::Token*>& posfix, std::deque<tok::Token*>& operators) {
            posfix.push_back(tok);
        }
        /** 
         * Take a vector of numbers and evaluate the value of it using the 
         * specified operation this class consists of.
         * */
        virtual double evaluate(std::deque<double> &) { return 0x0; };
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
    std::vector<tok::Token *> infixtopostfixO(std::vector<tok::Token *> tokens);
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

    struct Value : public tok::Token
    {

        Value(std::string value, unsigned position)
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
    };
    struct Literal : public ev::Value
    {

        Literal(std::string value, unsigned position) : ev::Value(value, position)
        {
        }
        double inline evaluate(std::deque<double> &operands) override
        {
            return 0;
        }
    };
    struct VARIABLE : public ev::Value
    {

        VARIABLE(std::string value, unsigned position) : ev::Value(value, position)
        {
        }
        double inline evaluate(std::deque<double> &operands) override
        {
            return 0;
        }
    };
    struct Parenthesis : public tok::Token
    {

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
        double inline evaluate(std::deque<double> &operands) override
        {
            return 0;
        }
    };
    struct LPAREN : public Parenthesis
    {

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
        virtual void parsetoInfix(tok::Token *&tok, std::vector<tok::Token*>& tokens, std::vector<tok::Token*>& posfix, std::deque<tok::Token*>& operators) {
            operators.push_front(tok);
        }

    };
    struct RPAREN : public Parenthesis
    {

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
        virtual void parsetoInfix(tok::Token *&tok, std::vector<tok::Token*>& tokens, std::vector<tok::Token*>& posfix, std::deque<tok::Token*>& operators) {
            while (!(operators.front()->isLeftParen()))
            {
                posfix.push_back(operators.front());
                operators.pop_front();
            }
            operators.pop_front();
        }
    };
    struct Operation : public tok::Token {
        Operation(std::string value, unsigned position) : tok::Token(value, position)
        {
        }
         inline bool isFunction() { return false; };
         inline bool isLiteral() { return false; };
         inline bool isParenthesis() { return false; };
         inline bool isRightParen() { return false; };
         inline bool isLeftParen() { return false; };
         inline bool isTernaryOperation() { return false; };
    };
    struct UnaryOp : public Operation
    {

        UnaryOp(std::string value, unsigned position) : Operation(value, position)
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
        virtual void parsetoInfix(tok::Token *&tok, std::vector<tok::Token*>& tokens, std::vector<tok::Token*>& posfix, std::deque<tok::Token*>& operators) {
            operators.push_front(tok);
        }
    };
    struct BinaryOp : public Operation
    {

        BinaryOp(std::string value, unsigned position) : Operation(value, position)
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
        virtual void parsetoInfix(tok::Token *&tok, std::vector<tok::Token*>& tokens, std::vector<tok::Token*>& posfix, std::deque<tok::Token*>& operators) {
             while (!operators.empty() && (!(operators.front()->isParenthesis()) && (isHigherPrecedenceThan(tok, operators.front()))))
            {
                posfix.push_back(operators.front());
                operators.pop_front();
            }
            operators.push_front(tok);
        }
    };
    struct ADD : public BinaryOp
    {

        ADD(std::string value, unsigned position) : BinaryOp::BinaryOp(value, position)
        {
        }
        unsigned inline getPrecedence() override
        {
            return 6;
        }
        double inline evaluate(std::deque<double> &operands) override
        {
            double operand1, operand2;
            if (operands.size() < 2)
                return 0.0;
            // The rightmost operand in the binary operation:
            operand2 = operands.front();
            operands.pop_front();
            // The leftmost operand in the binary operation:
            operand1 = operands.front();
            operands.pop_front();
            return operand1 + operand2;
        }
    };
    struct SUB : public BinaryOp
    {

        SUB(std::string value, unsigned position) : BinaryOp::BinaryOp(value, position)
        {
        }
        unsigned inline getPrecedence() override
        {
            return 6;
        }
        double inline evaluate(std::deque<double> &operands) override
        {
            double operand1, operand2;
            if (operands.size() < 2)
                return 0.0;
            // The rightmost operand in the binary operation:
            operand2 = operands.front();
            operands.pop_front();
            // The leftmost operand in the binary operation:
            operand1 = operands.front();
            operands.pop_front();
            return operand1 - operand2;
        }
    };
    struct MULT : public BinaryOp
    {

        MULT(std::string value, unsigned position) : BinaryOp::BinaryOp(value, position)
        {
        }
        unsigned inline getPrecedence() override
        {
            return 5;
        }
        double inline evaluate(std::deque<double> &operands) override
        {
            double operand1, operand2;
            if (operands.size() < 2)
                return 0.0;
            // The rightmost operand in the binary operation:
            operand2 = operands.front();
            operands.pop_front();
            // The leftmost operand in the binary operation:
            operand1 = operands.front();
            operands.pop_front();
            return operand1 * operand2;
        }
    };
    struct DIV : public BinaryOp
    {

        DIV(std::string value, unsigned position) : BinaryOp::BinaryOp(value, position)
        {
        }
        unsigned inline getPrecedence() override
        {
            return 5;
        }
        double inline evaluate(std::deque<double> &operands) override
        {
            double operand1, operand2;
            if (operands.size() < 2)
                return 0.0;
            // The rightmost operand in the binary operation:
            operand2 = operands.front();
            operands.pop_front();
            // The leftmost operand in the binary operation:
            operand1 = operands.front();
            operands.pop_front();
            return operand1 / operand2;
        }
    };
}

#endif