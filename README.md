## Binary Search Tree Simulator

Mini-projeto de simulador de Árvore Binária de Busca em C++, com interface visual baseada no _framework_ [Streamlit](https://streamlit.io/) do Python.

### Sumário

1. [Preparação](#preparação)
    1. [Dependências](#dependências)
    2. [Compilação](#compilação)
2. [Execução](#execução)
3. [Tecnologias](#tecnologias)
4. [Créditos](#créditos)

### Preparação

#### Dependências

##### C++

Instale o compilador GNU/GCC e a biblioteca [JSON for Modern C++](https://github.com/nlohmann/json):

```
    sudo apt update
    sudo apt install build-essential
    sudo apt install nlohmann-json3.12-dev
```

##### Python

Instale o Python, bem como o gerenciador de pacotes "pip" e o ambiente virtual "venv" do Python:

```
    sudo apt update
    sudo apt install python3 python3-pip python3-venv
```

Inicialize e acesse o ambiente virtual do Python e instale o Streamlit:

```
    python3 -m venv .venv
    source .venv/bin/activate
    pip install streamlit
```
> O Streamlit só estará disponível dentro do ambiente virtual e, portanto, deve ser acessado por `source .venv/bin/activate` para toda execução do projeto.

#### Compilação

Compile o código C++ da implementação da Árvore Binária de Busca (BST): `g++ bst.cpp -o bst`.

### Execução

Inicie a interface visual através de `streamlit run app.py`.

### Tecnologias

A Árvore Binária de Busca (BST) é implementada em C++ de acordo com o paradigma de programação recursiva. A cada chamada de função, a interface Python executa o código compilado com três parâmetros principais: Os elementos da árvore em pré-ordem, o tipo da função (e. g. inserção, remoção, busca) e o valor do elemento alterado. 

De forma que, a árvore é reconstruída a partir do _array_ em pré-ordem e a função escolhida é executada para o valor informado. Finalizando, o código C++ imprime na saída padrão o arquivo JSON com o grafo da árvore (em código DOT) e outras informações relevantes, tais como o _array_ da árvore em ordem, pré-ordem e pós-ordem, bem como mensagens de _feedback_.

### Créditos

Mini-projeto desenvolvido por Rafael Renó Corrêa como atividade extra da disciplina de Algoritmos e Estruturas de Dados durante mestrado na UNIFEI, 2026.