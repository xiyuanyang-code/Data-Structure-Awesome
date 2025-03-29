/*
Implementatin of expression of Binary Tree
*/
#include <string>
#include <iostream>

class calc{
    enum Type{DATA, ADD, SUB, MULTI, DIV, OPAREN, CPAREN, EOL};

    struct node
    //define the binary tree
    {
        Type type;
        int data;
        node *lchild, *rchild;
        node (Type t, int d = 0, node *lc = nullptr, node *rc = nullptr){
            type = t;
            data = d;
            lchild = lc;
            rchild = rc;
        }
    };
    
    private:
        node* root;
    
        //several tool function
        node* create(const char*& s){
            node *p = nullptr;
            node *root = nullptr;
            Type returntype;
            int value;

            //creating the new tree
            while(*s){
                returntype = gettoken(s, value);
                switch (returntype)
                {
                    case DATA:
                    case OPAREN:
                        if (returntype == DATA) {
                            p = new node(DATA, value);
                        } else {
                            p = create(s);
                            //if is a OPAREN, then we need to create a new subtree recursively!
                        }

                        if (root != nullptr) {
                            if(root -> rchild == nullptr){
                                root -> rchild = p;
                            }else{
                                //it must be * or /
                                root -> rchild -> rchild = p;
                            }
                        }
                        break;
                    case CPAREN:
                    case EOL:
                        // the creation ends
                        return root;
                    case ADD:
                    case SUB:
                        if (root == nullptr){
                            root = new node(returntype, 0, p);
                            //attention, in this case, the data 0 is meaningless for non-data operators;
                        } else {
                            root = new node(returntype, 0, root);
                        }
                        break;
                    case MULTI:
                    case DIV:
                        if(root == nullptr){
                            root = new node(returntype, 0, p);
                        } else if (root -> type == MULTI || root -> type == DIV) {
                            root = new node(returntype, 0, root);
                        } else {
                            root -> rchild = new node(returntype, 0, root -> rchild);
                        }
                }
            }

            //return the root node of the tree
            return root;
        }

        Type gettoken(const char *&s, int &data)
        {
            char type;

            while (*s == ' ') ++s;          // Skip spaces in the infix expression

            if (*s >= '0' && *s <= '9') {   // Encountered a number
                data = 0;
                while (*s >= '0' && *s <= '9') {
                    data = data * 10 + *s - '0';
                    ++s;
                }
                return DATA;
            }

            if (*s == '\0') return EOL;

            type = *s; ++s;
            switch (type) {
                case '+': return ADD;
                case '-': return SUB;
                case '*': return MULTI;
                case '/': return DIV;
                case '(': return OPAREN;
                case ')': return CPAREN;
                default: return EOL;
            }
        }

        int result(calc::node *t)
        {
            int num1, num2;
        
            if (t->type == DATA) return t->data;
            num1 = result(t->lchild);  // Calculate the left sub-expression
            num2 = result(t->rchild);  // Calculate the right sub-expression
            switch (t -> type) {
                case ADD: t->data = num1 + num2; break;
                case SUB: t->data = num1 - num2; break;
                case MULTI: t->data = num1 * num2; break;
                case DIV: t->data = num1 / num2; break;
            }
            return t->data;
        }

    public:
        calc(const char *s){
            root = create(s);
        }
        int result(){
            if(root == nullptr){
                return 0;
            }

            return result(root);
        }
};

int main(){
    std::string exp;
    std::cin >> exp;
    calc cl(exp.c_str());

    std::cout << cl.result() << std::endl;
}