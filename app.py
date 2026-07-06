import streamlit as st
import subprocess
import json

st.set_page_config(page_title='BST simulator', layout='centered')
st.title('Binary Search Tree Simulator')

# 1. Inicializa o armazenamento do estado da árvore
if 'pre_order' not in st.session_state:
    st.session_state.in_order = []
    st.session_state.pre_order = []
    st.session_state.post_order = []
    # "pre_order" é a lista de elementos da árvore em pré-ordem, que será atualizada a cada operação.
    # Essa lista caracteriza a árvore lida em pré-ordem, de forma que a raíz é o primeiro elemento da lista e
    # os elementos subsequentes são os filhos da esquerda e da direita, respectivamente.
if 'dot_code' not in st.session_state:
    st.session_state.dot_code = 'digraph G {}'
    # O código DOT, ou linguagem DOT, é uma linguagem de descrição de grafos específica da função GraphViz.
    # De forma que, o programa C++ gera o código DOT da árvore a partir da lista de elementos em pré-ordem e 
    # a função GraphViz renderiza a árvore a partir desse código.

# 2. Prepara os botões de inserção, remoção e busca
st.markdown('### Funções')
value = st.number_input('Informe um valor numérico:', step=1, value=10)

col1, col2, col3 = st.columns(3)
with col1:
    insert_btn = st.button('Inserir elemento', use_container_width=True)
with col2:
    remove_btn = st.button('Remover elemento', use_container_width=True)
with col3:
    search_btn = st.button('Buscar elemento', use_container_width=True)

st.divider()

col4, col5, col6 = st.columns(3)
with col4:
    higher_value_btn = st.button('Retornar maior valor', use_container_width=True)
with col5:
    lowest_value_btn = st.button('Retornar menor valor', use_container_width=True)
with col6:
    sum_btn = st.button('Retornar soma dos valores', use_container_width=True)

col7, col8, col9 = st.columns(3)
with col7:
    average_btn = st.button('Retornar média dos valores', use_container_width=True)
with col8:
    leaves_btn = st.button('Retornar número de folhas', use_container_width=True)
with col9:
    height_btn = st.button('Retornar altura da árvore', use_container_width=True)

# 3. Define qual operação foi solicitada pelo usuário
operation = None
if insert_btn:
    operation = 'insert'
elif remove_btn:
    operation = 'remove'
elif search_btn:
    operation = 'search'
elif higher_value_btn:
    operation = 'higher_value'
elif lowest_value_btn:
    operation = 'lowest_value'
elif sum_btn:
    operation = 'sum'
elif average_btn:
    operation = 'average'
elif leaves_btn:
    operation = 'leaves'
elif height_btn:
    operation = 'height'

st.divider()

# 4. Lógica de comunicação com o C++
if operation:
    # 4.1. Prepara as string das lista atuais de elementos
    current_pre_order_string = ','.join(map(str, st.session_state.pre_order)) if st.session_state.pre_order else ''

    try:
        # 4.2. Invoca o programa C++
        result = subprocess.run(
            ['./bst', current_pre_order_string, operation, str(value)],
            capture_output=True,
            text=True,
            check=True
        )

        if operation == 'insert':
            st.toast(f'Elemento {value} inserido com sucesso.')
        elif operation == 'remove':
            st.toast(f'Elemento {value} removido com sucesso.')

        # 4.3. Processa a saída do programa C++, que é uma string JSON contendo o código DOT da árvore, 
        # as listas de elementos em ordem, pré-ordem e pós-ordem, bem como as mensagens de feedback.
        response = json.loads(result.stdout)

        if "dot_code" in response:
            st.session_state.dot_code = response["dot_code"]

            # 4.4. Atualiza as listas de elementos na memória da interface Streamlit
            st.session_state.in_order = response.get("in_order", [])
            st.session_state.pre_order = response.get("pre_order", [])
            st.session_state.post_order = response.get("post_order", [])

            # 4.5. Se houver, exibe a mensagem de feedback da operação de busca
            if "search_result" in response:
                if "não encontrado" in response["search_result"]:
                    st.error(response["search_result"])
                else:
                    st.success(response["search_result"])

    except FileNotFoundError:
        st.error("O executável './bst' não foi encontrado. Compile o código C++ primeiro.")
    
    except Exception as e:
        st.error(f"Falha na execução do programa C++: {e}")

# 5. Renderiza a árvore
st.markdown('### Visualização da Árvore')
if st.session_state.pre_order and st.session_state.dot_code:
    st.graphviz_chart(st.session_state.dot_code)
else:
    st.info("A árvore está vazia. Insira elementos para visualizá-la.")

# 6. Exibe as listas de elementos em ordem, pré-ordem e pós-ordem.
# Formata as listas removendo os colchetes e adicionando setas estilizadas
colA, colB, colC = st.columns(3)
with colA:
    st.info(f"Árvore em Ordem\n\n{" ➔ ".join(map(str, st.session_state.in_order))}")
with colB:
    st.info(f"Pré-ordem\n\n{" ➔ ".join(map(str, st.session_state.pre_order))}")
with colC:
    st.info(f"Pós-ordem\n\n{" ➔ ".join(map(str, st.session_state.post_order))}")