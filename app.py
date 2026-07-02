import streamlit as st
import subprocess

st.set_page_config(page_title='BST simulator', layout='centered')
st.title('Binary Search Tree Simulator')

# 1. Inicializa o armazenamento do estado da árvore
if 'elements' not in st.session_state:
    st.session_state.elements = []
if 'dot_code' not in st.session_state:
    st.session_state.dot_code = 'digraph G {}'

# 2. Controles de interface
st.markdown('### Opções')
value = st.number_input('Informe o valor do nó:', step=1, value=10)

col1, col2 = st.columns(2)
with col1:
    insert_btn = st.button('Inserir elemento', use_container_width=True)
with col2:
    remove_btn = st.button('Remover elemento', use_container_width=True)

# 3. Define qual operação foi solicitada pelo usuário
operation = None
if insert_btn:
    operation = 'insert'
elif remove_btn:
    operation = 'remove'

# 4. Lógica de comunicação com o C++
if operation:
    # 4.1. Prepara a string da lista atual de elementos
    current_string = ','.join(map(str, st.session_state.elements)) if st.session_state.elements else ''

    try:
        # 4.2. Invoca o programa C++
        result = subprocess.run(
            ['./bst', current_string, operation, str(value)],
            capture_output=True,
            text=True,
            check=True
        )

        # 4.3. Divide a saída do programa C++ pelo separador
        parts = result.stdout.strip().split('---')

        if len(parts) >= 2:
            st.session_state.dot_code = parts[0]
            new_array_string = parts[1].strip()
            
            # 4.4. Atualiza a lista de elementos na memória do Python
            if new_array_string == 'empty' or not new_array_string:
                st.session_state.elements = []
            else:
                st.session_state.elements = [int(x) for x in new_array_string.split(',')]

    except FileNotFoundError:
        st.error("O executável './arvore' não foi encontrado. Compile o código C++ primeiro.")
    
    except Exception as e:
        st.error(f"Falha na execução do programa C++: {e}")

# 5. Renderiza a árvore
st.divider()
st.markdown('### Visualização da Árvore')
if st.session_state.elements:
    st.graphviz_chart(st.session_state.dot_code)
else:
    st.info("A árvore está vazia. Insira elementos para visualizá-la.")

# 6. Exibe a lista de elementos (estado interno)
st.caption(f"Estado atual da lista no front-end: {st.session_state.elements}")