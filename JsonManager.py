from os.path import isfile
from json import dump, load


data = {
    'Light state': [],
    'Humidity': [],
    'Temperature': [],
    'Sound activity': [],
    'Beehive humidity': [],
    'Beehive temperature': [],
    'Beehive Pressure': [],
    'Time': []
}


class JsonManager:

    def createJson(self, filepath, data, forceCreate=False):

        if not isfile(filepath) or forceCreate:
            with open(filepath,'w') as f:
                dump(data,f,indent=2,separators=(',',': '))

    def readJson(self, filepath):
        if isfile(filepath):
            with open(filepath, encoding="utf8") as f:
                data = load(f)
            return data
        else:
            return False

    def updateJson(self, filepath, data):
        with open(filepath, 'w', encoding="utf8") as f:
            dump(data, f, indent=2, separators=(',',': '))


jmanager = JsonManager()
jmanager.createJson('data.json', data)