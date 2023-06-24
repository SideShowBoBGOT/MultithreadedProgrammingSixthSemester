from enum import Enum
from enum import auto
import requests
import json
import random

class AlgType(Enum):
    BLOCK_STRIPED = auto()
    FOX = auto()
    NATIVE = auto()

URL_MULTIPLY = 'http://localhost:8080/api/multiply'
URL_RANDOM_MULTIPLY = 'http://localhost:8080/api/randomMultiply'
HEADERS = {"Content-Type": "application/json; charset=utf-8"}

def genList(size: int):
    return [random.uniform(0, 1) for _ in range(size * size)]

def getMultiplyData(algType: AlgType, size: int, threadsNum: int) -> dict:
    return {
        'algType': algType.name,
        'first': {
            'rows': size,
            'cols': size,
            'values': genList(size)
            },
        'second': {
            'rows': size, 
            'cols': size,
            'values': genList(size)
            },
        'threadsNum': threadsNum
        }


response = requests.get(URL_MULTIPLY, headers=HEADERS, json=getMultiplyData(AlgType.BLOCK_STRIPED, 10, 2))
print(response.content)
