import streamlit as st
import subprocess

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

# 3. Define qual operação foi solicitada pelo usuário
operation = None
if insert_btn:
    operation = 'insert'
elif remove_btn:
    operation = 'remove'
elif search_btn:
    operation = 'search'

# 4. Lógica de comunicação com o C++
if operation:
    # 4.1. Prepara as string das lista atuais de elementos
    current_in_order_string = ','.join(map(str, st.session_state.in_order)) if st.session_state.in_order else ''
    current_pre_order_string = ','.join(map(str, st.session_state.pre_order)) if st.session_state.pre_order else ''
    current_post_order_string = ','.join(map(str, st.session_state.post_order)) if st.session_state.post_order else ''

    try:
        # 4.2. Invoca o programa C++
        result = subprocess.run(
            ['./bst', current_in_order_string, current_pre_order_string, current_post_order_string, operation, str(value)],
            capture_output=True,
            text=True,
            check=True
        )

        if operation == 'insert':
            st.toast(f'Elemento {value} inserido com sucesso.')
        elif operation == 'remove':
            st.toast(f'Elemento {value} removido com sucesso.')

        # 4.3. Divide a saída do programa C++ pelo separador
        parts = result.stdout.strip().split('---')
        # A saída do programa C++ é dividida em quatro partes: o código DOT e as três listas de elementos atualizadas.

        if len(parts) >= 5:
            st.session_state.dot_code = parts[0]
            in_order_string = parts[1].strip()
            pre_order_string = parts[2].strip()
            post_order_string = parts[3].strip()
            search_feedback = parts[4].strip()

            # 4.4. Atualiza as listas de elementos na memória da interface Streamlit
            if in_order_string == 'empty' or not in_order_string:
                st.session_state.in_order = []
            else:
                st.session_state.in_order = [int(x) for x in in_order_string.split(',')]

            if pre_order_string == 'empty' or not pre_order_string:
                st.session_state.pre_order = []
            else:
                st.session_state.pre_order = [int(x) for x in pre_order_string.split(',')]

            if post_order_string == 'empty' or not post_order_string:
                st.session_state.post_order = []
            else:
                st.session_state.post_order = [int(x) for x in post_order_string.split(',')]

            # 4.5. Se houver, exibe a mensagem de feedback da operação de busca
            if search_feedback:
                if "não encontrado" in search_feedback:
                    st.error(search_feedback)
                else:
                    st.success(search_feedback)

    except FileNotFoundError:
        st.error("O executável './bst' não foi encontrado. Compile o código C++ primeiro.")
    
    except Exception as e:
        st.error(f"Falha na execução do programa C++: {e}")

# 5. Renderiza a árvore
st.divider()
st.markdown('### Visualização da Árvore')
if st.session_state.pre_order and st.session_state.dot_code:
    st.graphviz_chart(st.session_state.dot_code)
else:
    st.info("A árvore está vazia. Insira elementos para visualizá-la.")

# 6. Exibe as listas de elementos em ordem, pré-ordem e pós-ordem
st.caption(f"Elementos em ordem: {sorted(st.session_state.in_order)}")
st.caption(f"Elementos em pré-ordem: {st.session_state.pre_order}")
st.caption(f"Elementos em pós-ordem: {list(reversed(sorted(st.session_state.post_order)))}")