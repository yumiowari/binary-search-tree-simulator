#include <iostream>
#include <vector>
#include <string>

struct node{
    int value;
    node *left;
    node *right;
    node(int v) : value(v), left(nullptr), right(nullptr) {}
};

class BinarySearchTree{
    public:
        node *root = nullptr;

        node *insert(node* n, int v){
            if(n == nullptr)return new node(v);

            if(v < n->value){
                n->left = insert(n->left, v);
            }else if(v > n->value){
                n->right = insert(n->right, v);
            }

            return n;
        }

        node *remove(node *n, int v){
            if(n == nullptr)return n;

            if(v < n->value){
                n->left = remove(n->left, v);
            }else if(v > n->value){
                n->right = remove(n->right, v);
            }else{
                // nó com apenas um filho ou nenhum filho
                if(n->left == nullptr){
                    node *aux = n->right;
                    delete n;
                    return aux;
                }else if(n->right == nullptr){
                    node *aux = n->left;
                    delete n;
                    return aux;
                }

                // nó com dois filhos: obtém o sucessor (menor nó da subárvore direita)
                node *aux = n->right;
                while(aux && aux->left != nullptr)aux = aux->left;
                n->value = aux->value;
                n->right = remove(n->right, aux->value);
            }

            return n;
        }

        void generateDOTconnections(node *n){
            if(n == nullptr)return;

            if(n->left != nullptr){
                std::cout << "    " << n->value << " -> " << n->left->value << ";" << std::endl;
                generateDOTconnections(n->left);
            }
            
            if(n->right != nullptr){
                std::cout << "    " << n->value << " -> " << n->right->value << ";" << std::endl;
                generateDOTconnections(n->right);
            }

            if(n->left == nullptr && n->right == nullptr){
                std::cout << "    " << n->value << ";" << std::endl;
            }
        }

        void printDOTcode(){
        // o grafo será renderizado a partir do código DOT gerado a partir das conexões em generateDOTconnections()

            std::cout << "digraph G {" << std::endl;

            // Configurações de estilo do grafo
                // A. Fundo transparente (visualização compatível com temas claros e escuros)
            std::cout << "    bgcolor=\"transparent\";\n";
                // B. Vértices preenchidos com egyptian blue, borda com charcoal e texto com offwhite
            std::cout << "    node [style=filled, fillcolor=\"#1434A4\", color=\"#333333\", fontcolor=\"#f9f9f9\", fontname=\"Helvetica\", shape=circle];\n";
                // C. Arestas com cor charcoal, setas em forma de V
            std::cout << "    edge [color=\"#333333\", arrowhead=vee];\n";
            
            generateDOTconnections(root);
            
            std::cout << "}" << std::endl;
        }

        void inOrder(node *n, std::vector<int> &V){
            if(n == nullptr)return;

            inOrder(n->left, V);
            V.push_back(n->value);
            inOrder(n->right, V);
        }

        void preOrder(node *n, std::vector<int> &V){
            if(n == nullptr)return;

            V.push_back(n->value);
            preOrder(n->left, V);
            preOrder(n->right, V);
        }

        void postOrder(node *n, std::vector<int> &V){
            if(n == nullptr)return;

            postOrder(n->left, V);
            postOrder(n->right, V);
            V.push_back(n->value);
        }

        bool search(node *n, int v){
            if(n == nullptr)return false;

            if(v == n->value)return true;
            else if(v < n->value)return search(n->left, v);
            else return search(n->right, v);
        }
};

void defaultOutput(){
    std::cout << "digraph G {}" << std::endl;
}

int main(int argc, char **argv){
    if(argc < 4){
        defaultOutput();

        return 0;
    }

    bool found;

    std::string original_in_order = argv[1];
    std::string original_pre_order = argv[2];
    std::string original_post_order = argv[3];
    std::string operation = argv[4];
    int value = std::stoi(argv[5]);

    BinarySearchTree bst;

    // 1. Reconstrói a árvore a partir do array original em pré-ordem
    if(original_pre_order != "empty" && !original_pre_order.empty()){
        std::string token;
        size_t pos = 0;

        while((pos = original_pre_order.find(',')) != std::string::npos){
            token = original_pre_order.substr(0, pos);
            if(!token.empty())
                bst.root = bst.insert(bst.root, std::stoi(token));
            original_pre_order.erase(0, pos + 1);        
        }

        if(!original_pre_order.empty())
            bst.root = bst.insert(bst.root, std::stoi(original_pre_order));
    }

    // 2. Aplica a operação de inserção, remoção ou busca na árvore
    if(operation == "insert"){
        bst.root = bst.insert(bst.root, value);
    }else if(operation == "remove"){
        bst.root = bst.remove(bst.root, value);
    }else if(operation == "search"){
        found = bst.search(bst.root, value);
    }

    // 3. Imprime o grafo resultante no formato de código DOT
    bst.printDOTcode();

    // 4. Imprime o 1º separador
    std::cout << "---" << std::endl;

    // 5. Imprime o array resultante da árvore em ordem
    std::vector<int> result_in_order;
    bst.inOrder(bst.root, result_in_order);

    if(result_in_order.empty()){
        std::cout << "empty" << std::endl;
    }else{
        for(size_t i = 0; i < result_in_order.size(); ++i){
            std::cout << result_in_order[i] << (i == result_in_order.size() - 1 ? "" : ",");
        }
        std::cout << std::endl;
    }

    // 6. Imprime o 2º separador
    std::cout << "---" << std::endl;

    // 7. Imprime o array resultante da árvore em pré-ordem
    std::vector<int> result_pre_order;
    bst.preOrder(bst.root, result_pre_order);
    
    if(result_pre_order.empty()){
        std::cout << "empty" << std::endl;
    }else{
        for(size_t i = 0; i < result_pre_order.size(); ++i){
            std::cout << result_pre_order[i] << (i == result_pre_order.size() - 1 ? "" : ",");
        }
        std::cout << std::endl;
    }

    // 8. Imprime o 3º separador
    std::cout << "---" << std::endl;

    // 9. Imprime o array resultante da árvore em pós-ordem
    std::vector<int> result_post_order;
    bst.postOrder(bst.root, result_post_order);

    if(result_post_order.empty()){
        std::cout << "empty" << std::endl;
    }else{
        for(size_t i = 0; i < result_post_order.size(); ++i){
            std::cout << result_post_order[i] << (i == result_post_order.size() - 1 ? "" : ",");
        }
        std::cout << std::endl;
    }

    // 10. Imprime o 4º separador
    std::cout << "---" << std::endl;

    // 11. Imprime a mensagem de feedback da operação de busca, se aplicável
    if(operation == "search"){
        if(found){
            std::cout << "Elemento \"" << value << "\" encontrado." << std::endl;
        }else{
            std::cout << "Elemento \"" << value << "\" não encontrado." << std::endl;
        }
    }

    return 0;
}