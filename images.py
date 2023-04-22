import tensorflow as tf
import numpy as np

# Carregar o conjunto de dados CIFAR-10
(x_train, y_train), (x_test, y_test) = tf.keras.datasets.cifar10.load_data()

# Definir as classes
class_names = ['airplane', 'automobile', 'bird', 'cat', 'deer',
               'dog', 'frog', 'horse', 'ship', 'truck']

# Inicializar um array numpy vazio para armazenar todas as imagens
IMG_DATA = np.zeros((1000, 32*32*3), dtype=np.uint8)

# Loop pelas classes
for i in range(10):
    # Selecionar as imagens correspondentes à classe
    images = x_train[y_train.flatten() == i][:100]
    # Adicionar as imagens ao array principal
    IMG_DATA[i*100:(i+1)*100] = images.reshape(100, -1)

# Transformar o array em uma lista de listas
IMG_DATA = IMG_DATA.tolist()

# Salvar o array de imagens em um arquivo de cabeçalho C
with open('cifar10_images.h', 'w') as f:
    f.write("unsigned char IMG_DATA[1000][3072] = {\n")
    for i, img in enumerate(IMG_DATA):
        f.write("{")
        for j, pixel in enumerate(img):
            if j < len(img)-1:
                f.write(f"{pixel}, ")
            else:
                f.write(f"{pixel}")
        if i < len(IMG_DATA)-1:
            f.write("},\n")
        else:
            f.write("}\n")
    f.write("};\n")