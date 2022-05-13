from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split
from PIL import Image
from glob import glob
from numpy import array
from random import shuffle
from sklearn.metrics import accuracy_score,confusion_matrix,classification_report


def dataset(test_size=0.20):

	path = r'C:\Users\Danrl\Desktop\Danrley\TCC\Arduino sensors\monitoringSystem\bees'
	files = glob(f'{path}\\*.jpg')
	shuffle(files)
	newSize = (48, 48)
	X = []
	y = []
	for imgSource in files:
		img = Image.open(imgSource)
		img2arr = array(img.resize(newSize))
		X.append(img2arr)
		if 'bee_0' in imgSource:
			y.append(0)
		else:
			y.append(1)
	x_train, x_test, y_train, y_test = train_test_split(array(X), array(y), test_size=test_size, random_state=0)
	return array(x_train), array(y_train), array(x_test), array(y_test)

x_train, y_train, x_test, y_test = dataset()
x_train = x_train/255.0
x_test = x_test/255.0
nsamples, nx, ny, nrgb = x_train.shape
x_train2 = x_train.reshape((nsamples, nx*ny*nrgb))
nsamples, nx, ny, nrgb = x_test.shape
x_test2 = x_test.reshape((nsamples, nx*ny*nrgb))

model = DecisionTreeClassifier(random_state=0)
model.fit(x_train2, y_train.ravel())
y_pred = model.predict(x_test2)
print(classification_report(y_pred, y_test))
print(accuracy_score(y_pred, y_test))
print(confusion_matrix(y_pred, y_test))