import requests
import json
def publish_to_rabbitmq(username, password, routing_key, message):
    url = "http://rabbitmq:15672/api/exchanges/%2F/amq.default/publish"
    headers = {}#'Content-Type': 'application/json'}
    data = {
        "properties": {},
        "routing_key": routing_key,
        "payload": message,
        "payload_encoding": "string"
    }

    response = requests.post(url, auth=(username, password), data=json.dumps(data))

with open("message.txt") as f:
    publish_to_rabbitmq("guest", "guest", "cancel-bonus",f.readline())

