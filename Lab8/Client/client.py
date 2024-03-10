import requests

resp = requests.post("http://localhost:8080/api/hello")

print(resp.text)