import pandas as pd
import csv

# Ler o arquivo CSV
df = pd.read_csv('C:\\Users\\ritch\\Desktop\\Google-Playstore.csv')  # Substitua 'arquivo.csv' pelo caminho do seu arquivo CSV

# Verificar duplicatas na coluna desejada (por exemplo, 'id_pacote')
#coluna_para_verificar_duplicatas = 'App Id'
#duplicatas = df.duplicated(subset=[coluna_para_verificar_duplicatas], keep=False)
 
# Exibir as duplicatas
#print("Linhas duplicadas:")
#print(df[duplicatas])

colunas_desejadas = ['App Name', 'App Id', 'Rating', 'Installs']  # Substitua pelos nomes reais das colunas desejadas

# Filtrar o DataFrame para manter apenas as colunas desejadas
df_filtrado = df[colunas_desejadas]

#Especificar a coluna pela qual você deseja ordenar
coluna_para_ordenar = 'App Id'  

# Ordenar o DataFrame com base na coluna especificada
df_ordenado = df_filtrado.sort_values(by=coluna_para_ordenar)


# Salvar o DataFrame ordenado em um arquivo de texto
arquivo_saida = 'C:\\Users\\ritch\\Desktop\\DadosTDE.txt'  # Nome do arquivo de saída

def csv_to_text(dataframe, output_txt):
    with open(output_txt, 'w', encoding='utf-8') as txtfile:
        for index, row in dataframe.iterrows():
            # Transforma os dados da linha em uma lista de strings
            dados_formatados = [str(coluna) if coluna else ' ' for coluna in row]
            # Junta os dados com ',' e escreve a linha no arquivo de texto
            txtfile.write('¨'.join(dados_formatados) + '\n')


# Chama a função para converter o CSV para texto
csv_to_text(df_ordenado, arquivo_saida)

print(f'O arquivo CSV foi convertido para {arquivo_saida} com sucesso.')

