#include "Token.hpp"
// Reverse a list of tokens:
/**
 * Evaluate the value of an expression contained in the string parameter.
 **/
double tok::eval(std::string expr)
{
    std::vector<tok::Token *> tokens = tokenization(expr);
    print(tokens);
    tokens = infixtopostfix(tokens);
    return evaluate(tokens);
}
// TODO: Replace with match method utilizing the individual match methods of the individual classes.
std::vector<tok::Token *> tok::tokenization(std::string expr)
{
    std::vector<tok::Token *> tokens{};
    unsigned len = expr.length();
    for (unsigned i = 0; i < len; i++)
    {
        switch (expr.at(i))
        {
        case '&':
            if (tok::lookup("&&", expr, i, tokens))
            {
                (new tok::LAND("&&", i))->consume(tokens);
                std::cout << "!" << std::endl;
                i++;
            }
            else
            {
                (new tok::BAND("&", i))->consume(tokens);
            }

            break;
        case '%':
            (new tok::MOD("%", i))->consume(tokens);
        break;
        case '|':
            if (tok::lookup("||", expr, i, tokens))
            {
                (new tok::LOR("||", i))->consume(tokens);
                i++;
            }
            else
            {
                (new tok::BOR("|", i))->consume(tokens);
            }

            break;
        case '!':
            (new tok::LNOT("!", i))->consume(tokens);
            break;
        case '*':
            (new tok::MULT("*", i))->consume(tokens);
            break;
        case '/':
            (new tok::DIV("/", i))->consume(tokens);
            break;
        case '+':
            if (i == 0 || (tokens.size() != 0 && (tokens.back()->isBinaryOperation() || tokens.back()->isLeftParen())))
            {
                (new tok::UNADD("+", i))->consume(tokens);
            }
            else
            {
                (new tok::BINADD("+", i))->consume(tokens);
            }

            break;
        case '-':
            if (i == 0 || (tokens.size() != 0 && (tokens.back()->isBinaryOperation() || tokens.back()->isLeftParen())))
            {
                (new tok::UNSUB("-", i))->consume(tokens);
            }
            else
            {
                (new tok::BINSUB("-", i))->consume(tokens);
            }
            break;
        case ',':
            (new tok::Comma(",", i))->consume(tokens);
            break;
        case '(':
        case '[':
        case '{':
            (new tok::LPAREN("(", i))->consume(tokens);
            break;
        case ')':
        case '}':
        case ']':
            (new tok::RPAREN(")", i))->consume(tokens);
            break;
        case ' ':
        case '\r':
        case '\n':
            // Just skip this.
            break;
        case '_':
            // Can be the start of a variable:
            i = consumeVar(expr, i, tokens);
            break;
        case '.':
            // Can be the start of a floating point literal.
            // \d*.?\d*e-?\d+ for ieee numbers.
            i = consumeLit(expr, i, tokens);
            break;
        //case '0':
        // Might be a normal literal, but if it is followed by an x it is a hex digit, or if it is followed by a normal non-zero digit it is an oktal number.
        // TODO!
        //break;
        default:
            if (isLetter(expr.at(i)))
            {
                i = consumeVar(expr, i, tokens);
            }
            else if (isDigit(expr.at(i)))
            {
                i = consumeLit(expr, i, tokens);
            }
            else
            {
                std::cout << "There is no such token!" << std::endl;
            }
            break;
        }
    }
    return tokens;
}
std::vector<tok::Token *> tok::infixtopostfix(std::vector<tok::Token *> tokens)
{
    std::vector<tok::Token *> posfix;
    std::deque<tok::Token *> operators;

    if (tokens.size() == 0)
        return std::vector<tok::Token *>();
    for (tok::Token *&tok : tokens)
    {
        tok->parsetoInfix(tok, tokens, posfix, operators);
    }
    while (!operators.empty())
    {
        posfix.push_back(operators.front());
        operators.pop_front();
    }
    return posfix;
}
std::vector<tok::Token *> tok::infixtopostfixO(std::vector<tok::Token *> tokens)
{
    std::vector<tok::Token *> posfix;
    std::deque<tok::Token *> operators;

    if (tokens.size() == 0)
        return std::vector<tok::Token *>();
    for (tok::Token *&tok : tokens)
    {
        if (tok->isBinaryOperation())
        {
            // operator:
            // While the stack is not empty AND the front operator is not a parenthesis AND the current operator has a higher precedence than the one on the front:
            while (!operators.empty() && (!(operators.front()->isParenthesis()) && (isHigherPrecedenceThan(tok, operators.front()))))
            {
                posfix.push_back(operators.front());
                operators.pop_front();
            }
            operators.push_front(tok);
        }
        else if (tok->isUnaryOperation())
        {
            operators.push_front(tok);
        }
        else if (tok->isLeftParen())
        {
            // Left parenthesis:
            operators.push_front(tok);
        }
        else if (tok->isRightParen())
        {
            // Right parenthesis:
            while (!(operators.front()->isLeftParen()))
            {
                posfix.push_back(operators.front());
                operators.pop_front();
            }
            operators.pop_front();
        }
        else
        {
            // Constant/Variable:
            posfix.push_back(tok);
        }
    }
    while (!operators.empty())
    {
        posfix.push_back(operators.front());
        operators.pop_front();
    }
    return posfix;
}
double tok::evaluate(std::vector<tok::Token *> rpn)
{ // http://www-stone.ch.cam.ac.uk/documentation/rrf/rpn.html
    std::deque<double> stack;
    std::reverse(rpn.begin(), rpn.end());
    double operand1, operand2;
    while (!rpn.empty())
    {

            stack.push_front(rpn.back()->evaluate(stack));
            rpn.pop_back();
        
    }
    return stack.front();
}
unsigned tok::consumeVar(std::string expr, unsigned pos, std::vector<tok::Token *> &tokens)
{
    unsigned long long skip = pos;
    for (unsigned itr = pos; (expr.length() != itr) && (isLetter(expr.at(itr)) || (expr.at(itr) == '_') || (isDigit(expr.at(itr)))); itr++)
    {
        skip++;
    }
    if ((expr.size() != skip) && (expr.at(skip) == '('))
    {
        // It is a function: (only two operands right now)
        (new tok::Function{expr.substr(pos, skip - pos), pos})->consume(tokens);
    }
    else
    {
        // It is a variable:
        (new tok::VARIABLE{expr.substr(pos, skip - pos), pos})->consume(tokens);
    }
    return skip - 1;
    return 1;
}
// Check, if the element at pos matches to a literal and if it does,
unsigned tok::consumeLit(std::string expr, unsigned pos, std::vector<tok::Token *> &tokens)
{
    unsigned long long skip = pos;
    for (unsigned itr = pos; (expr.length() != itr) && (isDigit(expr.at(itr)) || (expr.at(itr) == '.')); itr++)
    {
        skip++;
    }
    (new tok::Literal{expr.substr(pos, skip - pos), pos})->consume(tokens);
    return skip - 1;
}
// Print the whole list front the element 0 to size-1;
void tok::print(std::vector<tok::Token *> tokens)
{
    for (tok::Token *&tok : tokens)
    {
        std::cout << tok->toString() << std::endl;
    }
}
void tok::print(std::deque<tok::Token *> tokens)
{
    for (tok::Token *&tok : tokens)
    {
        std::cout << tok->toString() << std::endl;
    }
}
inline bool tok::lookup(std::string match, std::string expr, int pos, std::vector<tok::Token *> &tokens)
{
    std::string str = expr.substr(pos, match.size());
    return !match.compare(str);
}