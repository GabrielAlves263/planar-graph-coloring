import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
csv_filename = os.path.join(script_dir, 'resultados_benchmark_10000_reps.csv')

def dataframe_to_png(df, title, filename):
   
    df_formatted = df.copy()
    for col in df_formatted.columns:
        if np.issubdtype(df_formatted[col].dtype, np.number) and col != 'N (Vértices)':
            df_formatted[col] = df_formatted[col].apply(lambda x: f"{x:,.2f}")

    fig, ax = plt.subplots(figsize=(8, 2.5))
    ax.axis('tight')
    ax.axis('off')
    
    table = ax.table(
        cellText=df_formatted.values,
        colLabels=df_formatted.columns,
        cellLoc='center',
        loc='center'
    )
    table.auto_set_font_size(False)
    table.set_fontsize(12)
    table.scale(1.2, 1.2)

    plt.title(title, fontsize=16, pad=20)
    
    plt.savefig(filename, dpi=300, bbox_inches='tight', pad_inches=0.1)
    print(f"Tabela guardada como '{filename}'")
    plt.close()

try:
    df = pd.read_csv(csv_filename)

    df_bruta = df[df['Algoritmo'] == 'Forca Bruta'].copy()
    
    pivot_bruta_ns = df_bruta.pivot(index='N', columns='Representacao', values='Tempo_Medio_ns').reset_index()
    pivot_bruta_ns.columns = ['N (Vértices)', 'Lista (ns)', 'Matriz (ns)']
    
    print("\n--- Tabela de Força Bruta (nanossegundos) ---")
    print(pivot_bruta_ns.to_string(index=False))
    
    dataframe_to_png(pivot_bruta_ns, 'Desempenho do Algoritmo de Força Bruta (nanossegundos)', 'tabela_forca_bruta_ns.png')

    df_grundy = df[df['Algoritmo'] == 'Grundy'].copy()
    
    df_grundy['Tempo_Medio_ms'] = df_grundy['Tempo_Medio_ns'] / 1_000_000
    pivot_grundy = df_grundy.pivot(index='N', columns='Representacao', values='Tempo_Medio_ms').reset_index()
    pivot_grundy.columns = ['N (Vértices)', 'Lista (ms)', 'Matriz (ms)']
    
    print("\n--- Tabela de Grundy (milissegundos) ---")
    print(pivot_grundy.to_string(float_format='%.2f', index=False))
    
    dataframe_to_png(pivot_grundy, 'Desempenho da Heurística de Grundy (milissegundos)', 'tabela_grundy_ms.png')

except FileNotFoundError:
    print(f"Erro: O ficheiro '{csv_filename}' não foi encontrado.")
except Exception as e:
    print(f"Ocorreu um erro: {e}")
