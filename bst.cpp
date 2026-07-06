#include <iostream>
#include <vector>
#include <string>

// middleware para serialização e desserialização de JSON
#include <nlohmann/json.hpp>
using json = nlohmann::json;

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

        bool search(node *n, int v){
            if(n == nullptr)return false;

            if(v == n->value)return true;
            else if(v < n->value)return search(n->left, v);
            else return search(n->right, v);
        }

        std::string generateDOTconnections(node *n){
            if(n == nullptr) return "";

            // Caso nó folha
            if(n->left == nullptr && n->right == nullptr)
                return "    " + std::to_string(n->value) + ";\n";

            // Caso nó com filhos
            std::string connections = "";

            if(n->left != nullptr){
                connections += "    " + std::to_string(n->value) + " -> " + std::to_string(n->left->value) + ";\n";
                connections += generateDOTconnections(n->left);
            }
            
            if(n->right != nullptr){
                connections += "    " + std::to_string(n->value) + " -> " + std::to_string(n->right->value) + ";\n";
                connections += generateDOTconnections(n->right);
            }

            return connections;
        }

        std::string printDOTcode(){
            std::string dot_code = "digraph G {\n";

            // Configurações de estilo do grafo
                // A. Fundo transparente (visualização compatível com temas claros e escuros)
            dot_code += "    bgcolor=\"transparent\";\n";
                // B. Vértices preenchidos com egyptian blue, borda com charcoal e texto com offwhite
            dot_code += "    node [style=filled, fillcolor=\"#1434A4\", color=\"#333333\", fontcolor=\"#f9f9f9\", fontname=\"Helvetica\", shape=circle];\n";
                // C. Arestas com cor charcoal, setas em forma de V
            dot_code += "    edge [color=\"#333333\", arrowhead=vee];\n";
            
            dot_code += generateDOTconnections(root);
            
            dot_code += "}\n";

            return dot_code;
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
};

std::string defaultOutput(){
    return "digraph G {}";
}

int main(int argc, char **argv){
    if(argc < 3){
        std::cout << defaultOutput() << std::endl;

        return 0;
    }

    std::string pre_order_string = argv[1];
    std::string operation = argv[2];
    int value = std::stoi(argv[3]);
    BinarySearchTree bst;
    json response;
    bool found;

    // 1. Reconstrói a árvore a partir do array original em pré-ordem
    if(pre_order_string != "empty" && !pre_order_string.empty()){
        std::string token;
        size_t pos = 0;

        while((pos = pre_order_string.find(',')) != std::string::npos){
            token = pre_order_string.substr(0, pos);
            if(!token.empty())
                bst.root = bst.insert(bst.root, std::stoi(token));
            pre_order_string.erase(0, pos + 1);        
        }

        if(!pre_order_string.empty())
            bst.root = bst.insert(bst.root, std::stoi(pre_order_string));
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
    response["dot_code"] = bst.printDOTcode();

    // 4. Imprime o array resultante da árvore em ordem, pré-ordem e pós-ordem
    std::vector<int> in_order, pre_order, post_order;
    bst.inOrder(bst.root, in_order);
    bst.preOrder(bst.root, pre_order);
    bst.postOrder(bst.root, post_order);
    response["in_order"] = in_order;
    response["pre_order"] = pre_order;
    response["post_order"] = post_order;

    // 5. Imprime a mensagem de feedback da operação de busca, se aplicável
    if(operation == "search"){
        response["search_result"] = found ? "Elemento " + std::to_string(value) + " encontrado." : "Elemento " + std::to_string(value) + " não encontrado.";
    }

    std::string json_output = response.dump();
    std::cout << json_output << std::endl;

    return 0;
}