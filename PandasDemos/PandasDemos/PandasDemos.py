import pandas as pd
import numpy as np
import os
#import matplotlib as plt

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
print(pointTypes)
print

# Read the data, ignore errors, decimal=',' only works for the first coordinate
coordinateConverters = {'Longitude': lambda coordinate: valueToFloat(coordinate), 'Latitude': lambda coordinate: valueToFloat(coordinate)}
colNames = ['GeonameId','Name','ASCIIName','AlternateNames',
'Latitude','Longitude','FeatureClass','FeatureCode',
'CountryCode','AlternateCountryCodes','Admin1Code','Admin2Code',
'Admin3Code','Admin4Code','Population','Elevation','DEM','Timezone','ModificationDate']
pointsData = pd.read_csv(os.path.abspath('../../Data/allCountries_extract.txt'), header=None, names=colNames, converters=coordinateConverters, sep='\t', encoding='utf-8', error_bad_lines=False)
print(pointsData.head())
print

# Describe the data
print('Columns and statistics')
print(pointsData.columns)
print
print(pointsData.describe())
print

# Group by TYPE
print(pointsData.groupby('CountryCode').size())
print
