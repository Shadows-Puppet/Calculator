#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <stack>
#include <stdexcept>

using namespace std;

class Token {
    public:
        string type;
        double value;
        char symbol;
        Token(string str, double val)
        {
            type = str;
            value = val;
        }

        Token(string str, char sym)
        {
            type = str;
            symbol = sym;
        }

        Token(string str)
        {
            type = str;
        }
};

unordered_map<string, int> precedence_table;

int get_precedence(Token x)
{
    return precedence_table.find(x.type)->second;
}

double calculate(Token op, double num1, double num2)
{
    double a = num1;
    double b = num2;

    switch(op.symbol)
    {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            throw logic_error("Unrecognized Operation");

    }
}


double polish_solve(queue<Token> q)
{
    stack<double> s;
    while(!q.empty())
    {

        if(q.front().type == "num")
        {
            s.push(q.front().value);
            q.pop();
        }
        else
        {
            double b, a;
            if(!s.empty())
            {
            b = s.top();
            s.pop();
            }
            if(!s.empty())
            {
            a = s.top();
            s.pop();
            }

            s.push(calculate(q.front(), a, b));
            q.pop();
        }
    }
    if(!s.empty())
    {
        return s.top();
    }
    return 0;
}
queue<Token> to_reverse_polish(vector<Token> vec)
{
    queue<Token> q;
    stack<Token> s;

    for(int i = 0; i < vec.size(); i++)
    {
        if(vec[i].type == "num")
        {
            q.push(vec[i]);
        }
        else if(vec[i].type == "add" || vec[i].type == "sub" || vec[i].type == "mul" || vec[i].type == "div")
        {
            while(!s.empty() && s.top().type != "l_paren" && get_precedence(s.top()) >= get_precedence(vec[i].type))
            {
                q.push(s.top());
                s.pop();
            }
            s.push(vec[i]);
        }

        else if(vec[i].type == "l_paren")
        {
            s.push(vec[i]);
        }
        else if(vec[i].type == "r_paren")
        {
            while(!s.empty() && s.top().type != "l_paren")
            {
                q.push(s.top());
                s.pop();
            }

            if(s.empty())
            {
                throw logic_error("Missing parenthesis");
            }
            else
            {
                s.pop();
            }

        }
        else
        {
            throw invalid_argument("Unsupported Operation");
        }
    }

    while(!s.empty())
    {
        if(s.top().type == "l_paren")
        {
           throw logic_error("Missing parenthesis"); 
        }
        else
        {
            q.push(s.top());
            s.pop();
        }
    }

    return q;
}

vector<Token> get_tokens()
{
    string expression;
    cout << "Please enter your expression: ";
    getline(cin, expression);
    vector<Token> gaming;

    for (int i = 0; i < expression.length(); i++)
    {
        if(isdigit(expression[i]))
        {
            string number = "";
            while(isdigit(expression[i]))
            {
                number += expression[i];
                i++;
            }
            i--;
            gaming.emplace_back("num", stod(number));
        }
        else
        {
            switch((expression[i]))
            {
                case ' ':
                    break;
                case '+':
                    gaming.emplace_back("add", '+');
                    break;
                case '-':
                    gaming.emplace_back("sub", '-');
                    break;
                case '*':
                    gaming.emplace_back("mul", '*');
                    break;
                case '/':
                    gaming.emplace_back("div", '/');
                    break;
                case '(':
                    gaming.emplace_back("l_paren");
                    break;
                case ')':
                    gaming.emplace_back("r_paren");
                
            }
        }
    }

    return gaming;
}

int main()
{
    precedence_table["add"] = 1;
    precedence_table["sub"] = 1;
    precedence_table["mul"] = 2;
    precedence_table["div"] = 2;




    queue <Token> polish = to_reverse_polish(get_tokens());

    double result = polish_solve(polish);

    cout << "The result is: " << result;
}
