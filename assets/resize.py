# resize all the images to 82.5x82.5
# and overwrite the original images

import os
from PIL import Image

def resize():
    for filename in os.listdir('assets'):
        if filename.endswith('.png'):
            img = Image.open('assets/' + filename)
            img = img.resize((82, 82), Image.ANTIALIAS)
            img.save('assets/' + filename)

if __name__ == '__main__':
    resize()