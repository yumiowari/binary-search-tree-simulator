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

        void printDOT(){
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

        // percurso em pré-ordem para manter a estrutura original ao reconstruir a árvore
        void collectElements(node *n, std::vector<int> &V){
            if(n == nullptr)return;

            V.push_back(n->value);
            collectElements(n->left, V);
            collectElements(n->right, V);
        }
};

void defaultOutput(){
    std::cout << "digraph G {}" << std::endl << "---" << std::endl;
}

int main(int argc, char **argv){
    if(argc < 4){
        defaultOutput();

        return 0;
    }

    std::string original_array = argv[1];
    std::string operation = argv[2];
    int value = std::stoi(argv[3]);

    BinarySearchTree bst;

    // 1. Reconstrói a árvore a partir do array original
    if(original_array != "empty" && !original_array.empty()){
        std::string token;
        size_t pos = 0;

        while((pos = original_array.find(',')) != std::string::npos){
            token = original_array.substr(0, pos);
            if(!token.empty())
                bst.root = bst.insert(bst.root, std::stoi(token));
            original_array.erase(0, pos + 1);        
        }

        if(!original_array.empty())
            bst.root = bst.insert(bst.root, std::stoi(original_array));
    }

    // 2. Aplica a operação de inserção ou remoção
    if(operation == "insert"){
        bst.root = bst.insert(bst.root, value);
    }else if(operation == "remove"){
        bst.root = bst.remove(bst.root, value);
    }

    // 3. Imprime o grafo resultante no formato DOT
    bst.printDOT();

    // 4. Imprime o separador
    std::cout << "---" << std::endl;

    // 5. Imprime o array resultante da árvore
    std::vector<int> result_array;
    bst.collectElements(bst.root, result_array);
    
    if(result_array.empty()){
        std::cout << "empty" << std::endl;
    }else{
        for(size_t i = 0; i < result_array.size(); ++i){
            std::cout << result_array[i] << (i == result_array.size() - 1 ? "" : ",");
        }
        std::cout << std::endl;
    }

    return 0;
}