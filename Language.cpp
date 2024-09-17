#include <cstdio>
#include <cctype>
#include <string>

//----------------------------------------------------------------
// Lexer
//----------------------------------------------------------------

enum Tokens {
    EOF_TOKEN = -1,

    DEF = -2,
    EXTERN = -3,

    IDENTIFIERS = -4,
    NUMBER = -5,
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

static int GetToken() {
    static int LastChar = ' ';

    // Skip whitespace
    while (isspace(LastChar))
        LastChar = getchar();

    // Handle keywords
    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return DEF;
        if (IdentifierStr == "extern")
            return EXTERN;
    }

    // Handle Numbers
    if (isalnum(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return NUMBER;
    }

    // Handle comments
    if (LastChar == '#') {
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
    }

    // Handle EOF and other special cases
    if (LastChar == EOF)
        return EOF_TOKEN;

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

//----------------------------------------------------------------
// Parser
//----------------------------------------------------------------

// Base class for all expressions nodes.
class ExprAST {
    public:
        virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST {
    double Number;

    public:
        NumberExprAST(double num) : Number(num) {}
};
