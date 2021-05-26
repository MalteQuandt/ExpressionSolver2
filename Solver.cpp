#include "Token.hpp"
// Reverse a list of tokens:
/**
 * Evaluate the value of an expression contained in the string parameter.
 **/
double tok::eval(std::string expr)
{
    std::vector<tok::Token *> tokens = tokenization(expr);
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
            (new tok::AND(std::to_string('*'), i))->consume(tokens);
            break;
        case '|':
            (new tok::OR(std::to_string('*'), i))->consume(tokens);
            break;
        case '!':
            (new tok::NOT(std::to_string('*'), i))->consume(tokens);
            break;
        case '*':
            (new tok::MULT(std::to_string('*'), i))->consume(tokens);
            break;
        case '/':
            (new tok::DIV(std::to_string('/'), i))->consume(tokens);
            break;
        case '+':
            if (i == 0 || (tokens.size() != 0 && tokens.back()->isBinaryOperation()))
            {
                (new tok::UNADD(std::to_string('+'), i))->consume(tokens);
            }
            else
            {
                (new tok::BINADD(std::to_string('+'), i))->consume(tokens);
            }

            break;
        case '-':
            if (i == 0 || (tokens.size() != 0 && tokens.back()->isBinaryOperation()))
            {
                (new tok::UNSUB(std::to_string('-'), i))->consume(tokens);
            }
            else
            {
                (new tok::BINSUB(std::to_string('-'), i))->consume(tokens);
            }
            break;
        case '(':
        case '[':
        case '{':
            (new tok::LPAREN(std::to_string('('), i))->consume(tokens);
            break;
        case ')':
        case '}':
        case ']':
            (new tok::RPAREN(std::to_string(')'), i))->consume(tokens);
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
        case '0':
            // Might be a normal literal, but if it is followed by an x it is a hex digit, or if it is followed by a normal non-zero digit it is an oktal number.
            // TODO!
            break;
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
        if (rpn.back()->isBinaryOperation() || rpn.back()->isUnaryOperation())
        {

            stack.push_front(rpn.back()->evaluate(stack));
            rpn.pop_back();
        }
        else
        {
            // It is an operand:
            stack.push_front(stod(rpn.back()->getValue()));
            rpn.pop_back();
        }
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
    (new tok::VARIABLE{expr.substr(pos, skip - pos), pos})->consume(tokens);
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
inline bool tok::lookup(std::string match, std::string expr, int pos, std::vector<tok::Token> &tokens)
{
    return match.compare(expr.substr(pos, match.size()));
}