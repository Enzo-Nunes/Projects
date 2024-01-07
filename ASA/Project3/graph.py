import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.font_manager as fm
from sklearn.linear_model import LinearRegression

# Let's first extract the complete data again to ensure accuracy.
data_full_graphs = {
    'N': [1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000],
    'P': [50, 100, 150, 200, 250, 350, 450, 550, 650, 750, 1000, 1250, 1500, 2000, 3000],
    'N+P': [1050, 2100, 3150, 4200, 5250, 6350, 7450, 8550, 9650, 10750, 12000, 13250, 14500, 16000, 18000],
    '3N+2P+1': [3101, 6201, 9301, 12401, 15501, 18701, 21901, 25101, 28301, 31501, 35001, 38501, 42001, 46001, 51001],
    'T/s': [0.06, 0.17, 0.36, 0.61, 0.92, 1.31, 1.74, 2.19, 2.74, 3.34, 3.89, 4.48, 5.2, 5.76, 6.06]
}

# Create a DataFrame
df_full_graphs = pd.DataFrame(data_full_graphs)

# Linear Regression for N+P
X_np = df_full_graphs['N+P'].values.reshape(-1, 1)
y = df_full_graphs['T/s'].values
model_np = LinearRegression()
model_np.fit(X_np, y)
X_np_fit = np.linspace(X_np.min(), X_np.max(), 1000).reshape(-1, 1)
y_np_fit = model_np.predict(X_np_fit)
r2_np = model_np.score(X_np, y)

# Linear Regression for 3N+2P+1
X_3n2p1 = df_full_graphs['3N+2P+1'].values.reshape(-1, 1)
model_3n2p1 = LinearRegression()
model_3n2p1.fit(X_3n2p1, y)
X_3n2p1_fit = np.linspace(X_3n2p1.min(), X_3n2p1.max(), 1000).reshape(-1, 1)
y_3n2p1_fit = model_3n2p1.predict(X_3n2p1_fit)
r2_3n2p1 = model_3n2p1.score(X_3n2p1, y)

font = fm.FontProperties(family='Times New Roman', size=12)

# Create the first graph for N+P
plt.figure(figsize=(10, 5))
plt.scatter(df_full_graphs['N+P'], df_full_graphs['T/s'], color='blue', label='Dados')
plt.plot(X_np_fit, y_np_fit, color='red', linewidth=2, label='Regressão Linear')
plt.title('Tempo de Execução em função de N+P')
plt.xlabel('Parâmetros do Problema (N+P)')
plt.ylabel('Tempo de Execução/s')
plt.grid(True)
plt.legend()
plt.text(12000, 1.6, f'y = {model_np.coef_[0]:.4f}x + {model_np.intercept_:.4f}', fontproperties=font, fontsize=12, bbox=dict(facecolor='white', alpha=0.5))
plt.text(12000, 1, f'R² = {r2_np:.4f}', fontproperties=font, fontsize=12, bbox=dict(facecolor='white', alpha=0.5))
plt.tight_layout()
plot_path_np_r2 = 'execution_time_vs_NP_with_r2.png'
plt.savefig(plot_path_np_r2)
plt.close()

# Create the second graph for 3N+2P+1
plt.figure(figsize=(10, 5))
plt.scatter(df_full_graphs['3N+2P+1'], df_full_graphs['T/s'], color='blue', label='Dados')
plt.plot(X_3n2p1_fit, y_3n2p1_fit, color='red', linewidth=2, label='Regressão Linear')
plt.title('Tempo de Execução em função de 3N+2P+1')
plt.xlabel('Tamanho do Programa Linear (3N+2P+1)')
plt.ylabel('Tempo de Execução/s')
plt.grid(True)
plt.legend()
plt.text(30000, 1.6, f'y = {model_3n2p1.coef_[0]:.4f}x + {model_3n2p1.intercept_:.4f}', fontsize=12, bbox=dict(facecolor='white', alpha=0.5))
plt.text(30000, 1, f'R² = {r2_3n2p1:.4f}', fontsize=12, bbox=dict(facecolor='white', alpha=0.5))
plt.tight_layout()
plot_path_3n2p1_r2 = 'execution_time_vs_3N2P1_with_r2.png'
plt.savefig(plot_path_3n2p1_r2)
plt.close()

(plot_path_np_r2, r2_np, plot_path_3n2p1_r2, r2_3n2p1)
