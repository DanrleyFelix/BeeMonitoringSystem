import pandas as pd
import matplotlib.pyplot as plt
from JsonManager import JsonManager


df = pd.read_json('data.json')

plt.plot(df['Time'], df['Beehive temperature'])
#plt.locator_params(axis='y', nbins=1)
plt.show()

