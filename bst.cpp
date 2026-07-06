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

        int higher(node *n){
            if(n == nullptr)return 0;

            if(n->right == nullptr && n->left == nullptr)return n->value;

            if(n->right != nullptr && n->left != nullptr)return std::max(higher(n->right), higher(n->left));
            else if(n->right != nullptr)return higher(n->right);
            else return higher(n->left);
        }

        int lowest(node *n){
            if(n == nullptr)return 0;

            if(n->right == nullptr && n->left == nullptr)return n->value;

            if(n->right != nullptr && n->left != nullptr)return std::min(lowest(n->right), lowest(n->left));
            else if(n->right != nullptr)return lowest(n->right);
            else return lowest(n->left);
        }

        int sum(node *n){
            if(n == nullptr)return 0;

            return n->value + sum(n->left) + sum(n->right);
        }

        int count(node *n){
            if(n == nullptr)return 0;

            return 1 + count(n->left) + count(n->right);
        }

        int average(node *n){
            if(n == nullptr)return 0;

            int s = sum(n);
            int c = count(n);

            return c > 0 ? s / c : 0;
        }

        int leaves(node *n){
            if(n == nullptr)return 0;

            if(n->left == nullptr && n->right == nullptr)return 1;

            return leaves(n->left) + leaves(n->right);
        }

        int height(node *n){
            if(n == nullptr)return 0;

            int left_height = height(n->left);
            int right_height = height(n->right);

            return std::max(left_height, right_height) + 1;
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
    int higher, lowest, sum, count, average, leaves, height;

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
    if (operation == "insert")       bst.root = bst.insert(bst.root, value);
    else if (operation == "remove")  bst.root = bst.remove(bst.root, value);
    else if (operation == "search")  found = bst.search(bst.root, value);
    else if (operation == "higher")  higher = bst.higher(bst.root);
    else if (operation == "lowest")  lowest = bst.lowest(bst.root);
    else if (operation == "sum")     sum = bst.sum(bst.root);
    else if (operation == "count")   count = bst.count(bst.root);
    else if (operation == "average") average = bst.average(bst.root);
    else if (operation == "leaves")  leaves = bst.leaves(bst.root);
    else if (operation == "height")  height = bst.height(bst.root);
    else {
        // default case: operação inválida
        std::cout << defaultOutput() << std::endl;
        
        return 0;
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
    if(operation == "search")response["search_result"] = found ? "Elemento " + std::to_string(value) + " encontrado." : "Elemento " + std::to_string(value) + " não encontrado.";
    else if(operation == "higher")response["higher_result"] = "Maior elemento da árvore: " + std::to_string(higher);
    else if(operation == "lowest")response["lowest_result"] = "Menor elemento da árvore: " + std::to_string(lowest);
    else if(operation == "sum")response["sum_result"] = "Soma dos elementos da árvore: " + std::to_string(sum);
    else if(operation == "count")response["count_result"] = "Quantidade de elementos na árvore: " + std::to_string(count);
    else if(operation == "average")response["average_result"] = "Média dos elementos da árvore: " + std::to_string(average);
    else if(operation == "leaves")response["leaves_result"] = "Quantidade de folhas na árvore: " + std::to_string(leaves);
    else if(operation == "height")response["height_result"] = "Altura da árvore: " + std::to_string(height);

    std::string json_output = response.dump();
    std::cout << json_output << std::endl;

    return 0;
}