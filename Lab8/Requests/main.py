
import requests
import json
import random
import time
from enum import Enum, auto

URL_CLIENT_MULTIPLY = 'http://localhost:8080/api/clientMultiply'
URL_SERVER_MULTIPLY = 'http://localhost:8080/api/serverMultiply'
HEADERS = {"Content-Type": "application/json; charset=utf-8"}

class AlgType(Enum):
    BLOCK_STRIPED = auto()
    FOX = auto()
    NATIVE = auto()


def gen_list(size: int):
    return [random.uniform(0, 1) for _ in range(size * size)] 
    
def client_multiply(algType: AlgType, size: int, threadsNum: int):
    data = {
        'algType': algType.name,
        'first': {
            'rows': size,
            'cols': size,
            'values': gen_list(size)
            },
        'second': {
            'rows': size, 
            'cols': size,
            'values': gen_list(size)
            },
        'threadsNum': threadsNum
        }
    return requests.get(URL_CLIENT_MULTIPLY, headers=HEADERS, json=data)
    
def server_multiply(algType: AlgType, size: int, threadsNum: int):
    data = {
        'algType': algType.name,
        'rows': size,
        'cols': size,
        'threadsNum': threadsNum
        }
    return requests.get(URL_SERVER_MULTIPLY, headers=HEADERS, json=data) 


sizes = [s for s in range(50, 750, 50)]
threads = [s for s in range(2, 5)]
funcs = [client_multiply, server_multiply]

with open('log.log', 'w') as file:
    for f in funcs:
        for s in sizes:
            for t in threads:
                nanoseconds = time.time_ns()
                response = f(AlgType.BLOCK_STRIPED, s, t)
                result = json.loads(response.content.decode())
                nanoseconds = time.time_ns() - nanoseconds
                nanoseconds += result['nanoseconds']
                
            
            



