import requests
import json


response = requests.get('http://localhost:8080/api', params={'rows': 3, 'cols': 3})
print(response.content)