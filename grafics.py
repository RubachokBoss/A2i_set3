import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
df = pd.read_csv('sorting_results.csv')
plt.rcParams['font.family'] = 'DejaVu Sans'
plt.figure(figsize=(10, 6))

merge_data = df[(df['ArrayType'] == 'Random') & (df['Algorithm'] == 'MergeSort')]
plt.plot(merge_data['Size'], merge_data['TimeMicroseconds'], 
         label='Обычный Merge Sort', linewidth=2, color='blue')

hybrid_data = df[(df['ArrayType'] == 'Random') & (df['Algorithm'] == 'HybridSort') & (df['Threshold'] == 20)]
plt.plot(hybrid_data['Size'], hybrid_data['TimeMicroseconds'], 
         label='Гибридный (threshold=20)', linewidth=2, color='red')

plt.xlabel('Размер массива')
plt.ylabel('Время (микросекунды)')
plt.title('Сравнение алгоритмов сортировки')
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('sorting_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

plt.figure(figsize=(10, 6))

thresholds = [5, 10, 20, 30, 50]
colors = ['green', 'orange', 'red', 'purple', 'brown']

for i, threshold in enumerate(thresholds):
    threshold_data = df[(df['ArrayType'] == 'Random') & (df['Algorithm'] == 'HybridSort') & (df['Threshold'] == threshold)]
    plt.plot(threshold_data['Size'], threshold_data['TimeMicroseconds'], 
             label=f'Порог = {threshold}', linewidth=2, color=colors[i])

plt.xlabel('Размер массива')
plt.ylabel('Время (микросекунды)')
plt.title('Влияние порога на скорость гибридной сортировки')
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('threshold_impact.png', dpi=300, bbox_inches='tight')
plt.show()

plt.figure(figsize=(10, 6))

array_types = ['Random', 'Reversed', 'AlmostSorted']
colors = ['blue', 'red', 'green']
labels = ['Случайный', 'Обратный', 'Почти отсорт.']

for i, array_type in enumerate(array_types):
    type_data = df[(df['ArrayType'] == array_type) & (df['Algorithm'] == 'HybridSort') & (df['Threshold'] == 20)]
    plt.plot(type_data['Size'], type_data['TimeMicroseconds'], 
             label=labels[i], linewidth=2, color=colors[i])

plt.xlabel('Размер массива')
plt.ylabel('Время (микросекунды)')
plt.title('Гибридная сортировка на разных типах массивов')
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('array_types.png', dpi=300, bbox_inches='tight')
plt.show()