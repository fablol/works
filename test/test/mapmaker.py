import MyEngine
import random

def make_map():
    the_map = MyEngine.GetMapInstance()
    n = 10
    for i in range(n):
        for j in range(10 - i, 10 + i - 1):
            the_map.SetPixel(j, i, ord('*'))