import pandas as pd
import numpy as np
import matplotlib as plt

def valueToFloat(value):
    """Converts the given value as float.
    value : the value which should be treated as a float
    """
    if (str('') == str(value).strip()):
        return np.NaN;
    try:
        return float(value.replace(',', '.'))
    except:
        return np.NaN 

# Create a series
pointTypes = pd.Series(['bus_stop', 'kiosk', 'traffic_signals'])
print unicode(pointTypes)
print

# Read the data, ignore errors, decimal=',' only works for the first coordinate
coordinateConverters = {'XCoord': lambda coordinate: valueToFloat(coordinate), 'YCoord': lambda coordinate: valueToFloat(coordinate)}
pointsData = pd.read_csv('C:/Data/OSM/ColognePoints.csv', converters=coordinateConverters, sep=';', encoding='utf-8', error_bad_lines=False)
print unicode(pointsData.head())
print

# Describe the data
print 'Columns and statistics'
print unicode(pointsData.columns)
print
print unicode(pointsData.describe())
print

# Group by TYPE
print unicode(pointsData.groupby('TYPE').size())
print
