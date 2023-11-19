import unittest
import json
import requests
import sys
HOST="127.0.0.1"
URL = "http://" + HOST + ":8070/api/v1/tickets"

p1 = {"price" : 1500,
      "flightNumber" : "cba123"}
p2 = {"price" : 1000,
      "flightNumber" : "abc123"}


p1j = json.dumps(p1)
p2j = json.dumps(p2)

headers = {'Content-type': 'application/json', 'Accept': 'text/'}
class TestMain(unittest.TestCase):
  # positives
  def test_cr(self):
    headers = {"X-User-Name" : "katya"}
    uuid = requests.post(URL, headers=headers, data = p1j).json()["ticketUid"]
    ticket_check = requests.get(URL + "/" + uuid, headers=headers).json()
    
    self.assertEqual(ticket_check["price"] == p1["price"] and
                     ticket_check["flightNumber"] == p1["flightNumber"] and
                     uuid == ticket_check["ticketUid"] and
                     ticket_check["status"] == "PAID",
                     True)
    
  def test_cu(self):
    headers = {"X-User-Name" : "katya"}
    uuid = requests.post(URL, headers=headers, data = p1j).json()["ticketUid"]
    status_was = requests.get(URL + "/" + uuid, headers=headers).json()["status"]
    requests.patch(URL + "/" + uuid, headers=headers)
    status_now = requests.get(URL + "/" + uuid, headers=headers).json()["status"]
    
    self.assertEqual(status_was == "PAID" and status_now == "CANCELED",
                     True)
  def test_get_all(self):
    headers = {"X-User-Name" : "spec_header"}
    init_len = len(requests.get(URL, headers=headers).json())
    headers = {"X-User-Name" : "spec_header_2"}
    requests.post(URL, headers=headers, data = p2j)
    headers = {"X-User-Name" : "spec_header"}
    requests.post(URL, headers=headers, data = p1j)
    requests.post(URL, headers=headers, data = p2j)
    end_len = len(requests.get(URL, headers=headers).json())
    self.assertEqual(end_len - init_len == 2,
                     True)


if len(sys.argv) > 1:
  HOST = str(sys.argv[1])
  URL = "http://" + HOST + ":8000/api/v1/tickets"
  del sys.argv[1:]
unittest.main()
    
