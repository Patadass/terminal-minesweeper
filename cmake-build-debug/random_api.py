import requests
import json

API_KEY = '9699714f-321b-402f-9cd1-b2f126036e55'
url = f'https://api.random.org/json-rpc/4/invoke'

file = open("python_input.txt")
contents = file.readlines()
_n = contents[0]
_height = contents[1]
_width = contents[2]
file.close()

_height = int(_height)
_width = int(_width)
_height -= 1
_width -= 1
raw_data_height = {
    "jsonrpc": "2.0",
    "method": "generateIntegers",
    "params": {
        "apiKey": API_KEY,
        "n": int(_n),
        "min": 0,
        "max": int(_height),
        "replacement": True
    },
    "id": 42
}
raw_data_width = {
     "jsonrpc": "2.0",
    "method": "generateIntegers",
    "params": {
        "apiKey": API_KEY,
        "n": int(_n),
        "min": 0,
        "max": int(_width),
        "replacement": True
    },
    "id": 43
}

headers = {'Content-type': 'application/json', 'Content-Length': '200', 'Accept': 'application/json'}
data_height = json.dumps(raw_data_height)
data_width = json.dumps(raw_data_width)
response_height = requests.post(
    url=url,
    data=data_height,
    headers=headers
)
response_width = requests.post(
    url=url,
    data=data_width,
    headers=headers
)

x = json.loads(response_height.text)
y = json.loads(response_width.text)


file = open('api_response.txt', 'w')
file.write(str(response_height.status_code) + '\n')
if response_height.status_code == 200 and response_width.status_code == 200:
    file.write(str(response_height.text) + '\n')
    file.write(str(response_width.text))
file.close()

if response_height.status_code == 200 and not ('error' in x) and not('error' in y):
    contents = []
    for num in x['result']['random']['data']:
            contents.append(str(num))
    i = 0
    for num in y['result']['random']['data']:
         contents[i] += ';'+str(num)
         i+=1
    file = open('random_cords.txt', 'w')
    for line in contents:
         file.write(line + '\n')
    file.close()
