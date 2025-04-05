/*
Test for Bracket-Matching
*/

#include <cstddef>
#include <fstream>
#include <stack>
#include <iostream>

class balance{
    private:
        std::ifstream fin;
        size_t current_line;

        // The number of errors
        int errors;
    struct Symbol
    {
        char token;
        int line;
    };

    enum CommentType {SlashSlash, SlashStar};

    private:
        // tool function

        /**
         * @brief Check whether the two brackets match
         * 
         * @param symb1 
         * @param symb2 
         * @param line1 
         * @param line2 
         * @return true match
         * @return false mismatch
         */
        bool checkMatch(char symb1, char symb2, int line1, int line2){
            if(symb1 == '(' && symb2 != ')' || symb1 == '{' && symb2 != '}' || symb1 == '[' && symb2 != ']'){
                // mismatch
                std::cout << "Character " << symb2 << " in line " << line2 << " mismatches with " << symb1 << " in line " << line1 << '\n' ;
                return false;
            }
            return true;
        }

        /**
         * @brief Get the Next Symbol object
         * 
         * @return char 
         */
        char getNextSymbol() {
            char ch;
            while(ch = NextChar()){
                if(ch == '/'){
                    // maybe a comment!
                    ch = NextChar();
                    if(ch == '*'){
                        // comments like /**/ 
                        skipComments(SlashStar);
                    }else if(ch == '/'){
                        skipComments(SlashSlash);
                    }else{
                        putBackChar(ch);
                        // Not a comment!
                    }
                }else if (ch == '\'' || ch == '"' ){
                    skipQuote(ch);
                }else if (ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == '(' || ch == ')'){
                    return ch;
                }
            }

            // when the file ends
            return char(NULL);
        }

        /**
         * @brief put the char back into the fstream
         * 
         */
        void putBackChar(char ch) {
            fin.putback(ch);
            if(ch == '\n'){
                -- current_line;
            }
        }

        void skipComments(enum CommentType type) {
            char ch, flag;
            // dealing with //
            if(type == SlashSlash){
                while(ch = NextChar() && ch != '\n'){
                    return;
                }
            }

            flag = char(NULL);
            while(!(ch = NextChar())){
                if(flag == '*' || flag == '/'){
                    // cpmments end
                    return;
                }
                flag = ch;
            }
            // if the comments doesn't end...
            ++ errors;
            std::cout << "Comments are unterminated!" << '\n';
        }

        /**
         * @brief skip the constant string value of char value
         * !remember the escape characters
         * 
         * @param type 
         */
        void skipQuote(char type) {
            char ch;
            while(ch = NextChar()){
                if(ch == type){
                    return;
                }else if (ch == '\n'){
                    ++ errors;
                    std::cout << "Missing quote at line " << current_line << '\n';
                    return;
                }
                else if(ch == '\\'){
                    ch = NextChar();
                }
            }
        }

        /**
         * @brief Read the next character of the file
         * 
         * @return char 
         */
        char NextChar() {
            char ch;
            if((ch = fin.get()) == EOF){
                // reach the end of the file
                return char(NULL);
            }else{
                if(ch == '\n'){
                    ++ current_line;
                }
            }
            return ch;
        }

    public:
        balance(const char* s) {
            fin.open(s);
            if(!fin){
                // The file open unsuccessfully
                throw NoFile();
            }

            current_line = 1; // 1-based
            errors = 0;
        }

        int checkBalance() {
            struct Symbol Node;
            std::stack<Symbol> st; // the stack
            char LastChar, Match;
            // LastChar is the read character, while Match is the top element of the stack

            while(LastChar = getNextSymbol()){
                // get symbols (comments excluded)
                switch (LastChar)
                {
                case '(': case '{': case '[':
                    // open brackets
                    Node.token = LastChar;
                    Node.line = current_line;

                    // input the stack
                    st.push(Node);
                    break;
                case ')': case '}': case ']':
                    // close brackets
                    if(st.empty()){
                        // no matching, getting error
                        ++ errors;
                        std::cout << "Error caught at line " << current_line << " with a single " << LastChar << '\n' ;
                    }else{
                        // get the top of the stack
                        Node = st.top();
                        st.pop();

                        Match =Node.token;
                        if(!checkMatch(Match, LastChar, Node.line, current_line)){
                            ++ errors;
                        }
                    }
                    break;
                }
            }

            // if the stack is not empty, then errors occur!
            while(!st.empty()){
                ++ errors;
                Node = st.top();
                st.pop();

                std::cout << "Error caught at line " << Node.line << " with a single " << Node.token << '\n' ;
            }
            return errors;
        }

        // exceptions
        class NoFile {};

};

int main() {
    try {
        // Create a balance object with the test file
        balance checker("test.txt");

        // Check for bracket balance
        int errors = checker.checkBalance();

        // Output the result
        if (errors == 0) {
            std::cout << "All brackets are balanced!" << std::endl;
        } else {
            std::cout << "Total errors found: " << errors << std::endl;
        }
    } catch (const balance::NoFile&) {
        std::cerr << "Error: Unable to open the file!" << std::endl;
    }

    return 0;
}