import os
import pandas as pd
import matplotlib.pyplot as plt

script_dir = os.path.dirname(os.path.abspath(__file__))
csv_filename = os.path.join(script_dir, 'resultados_benchmark_10000_reps.csv')

try:
    df = pd.read_csv(csv_filename)
    df_grundy = df[df['Algoritmo'] == 'Grundy'].copy()

    if df_grundy.empty:
        print("Nenhum dado encontrado para o algoritmo 'Grundy' no arquivo.")
    else:
        df_grundy['Tempo_Medio_ms'] = df_grundy['Tempo_Medio_ns'] / 1_000_000
        
        df_pivot_grundy = df_grundy.pivot(index='N', columns='Representacao', values='Tempo_Medio_ms')

        plt.style.use('seaborn-v0_8-whitegrid')
        fig, ax = plt.subplots(figsize=(10, 6))
        df_pivot_grundy['Lista'].plot(kind='line', marker='o', ax=ax, label='Lista de Adjacência', linestyle='--')
        df_pivot_grundy['Matriz'].plot(kind='line', marker='x', ax=ax, label='Matriz de Adjacência', linestyle='-')

        ax.set_title('Desempenho do Algoritmo Heurístico de Grundy por Representação', fontsize=16, pad=20)
        ax.set_xlabel('Tamanho do Grafo (N)', fontsize=12)
        ax.set_ylabel('Tempo Médio (ms)', fontsize=12) # Eixo Y em milissegundos
        ax.set_xticks(df_pivot_grundy.index)

        ax.get_yaxis().set_major_formatter(
            plt.FuncFormatter(lambda x, p: format(int(x), ',')))

        ax.legend(title='Representação do Grafo')
        ax.grid(True, which="both", ls="--")

        plt.tight_layout()

        output_filename = 'grundy_comparativo.png'
        plt.savefig(output_filename, dpi=300)
        
        print(f"Gráfico '{output_filename}' foi gerado e salvo com sucesso.")

except FileNotFoundError:
    print(f"Erro: O arquivo '{csv_filename}' não foi encontrado. Verifique se ele está no mesmo diretório que o script.")
except Exception as e:
    print(f"Ocorreu um erro: {e}")
