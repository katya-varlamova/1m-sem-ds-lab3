import unittest
import json
import requests
import sys
HOST="127.0.0.1"
URL = "http://" + HOST + ":8050/api/v1"
p1 = {"price" : 1500,
      "ticketUid" : "049161bb-badd-4fa8-9d90-87c9a82b0665",
      "username" : "Test Max",
      "paidFromBalance" : True}

p2 = {"price" : 1500,
      "ticketUid" : "049161bb-badd-4fa8-9d90-87c9a82b0666",
      "username" : "Test Max",
      "paidFromBalance" : False}

p3 = {"price" : 140,
      "ticketUid" : "049161bb-badd-4fa8-9d90-87c9a82b0667",
      "username" : "Test Max",
      "paidFromBalance" : True}
p1j = json.dumps(p1)
p2j = json.dumps(p2)
p3j = json.dumps(p3)

headers = {'Content-type': 'application/json', 'Accept': 'text/'}
class TestMain(unittest.TestCase):
  # positives
    
  def test_get_balance(self):
    headers = {"X-User-Name" : "Test Max"}
    self.assertEqual(len(requests.get(URL + "/balance", headers=headers).json()) == 3, True)
     
  def test_do_purchase1(self):
    ans = requests.patch(URL + "/purchase", data=p1j).json()
    self.assertEqual(ans["balance"] == 0 and  ans["status"] == 'GOLD' and ans["paidByMoney"] == 0 and ans['paidByBonuses'] == 1500, True)
    
    ans = requests.patch(URL + "/purchase", data=p2j).json()
    self.assertEqual(ans["balance"] == 150 and  ans["status"] == 'GOLD' and ans["paidByMoney"] == 1500 and ans['paidByBonuses'] == 0, True)

    ans = requests.patch(URL + "/purchase", data=p3j).json()
    self.assertEqual(ans["balance"] == 10 and  ans["status"] == 'GOLD' and ans["paidByMoney"] == 0 and ans['paidByBonuses'] == 140, True)

    headers = {"X-User-Name" : "Test Max", "ticket_uid" : "049161bb-badd-4fa8-9d90-87c9a82b0666"}
    self.assertEqual(requests.patch(URL + "/return", headers=headers).status_code == 200, True)
    
    headers = {"X-User-Name" : "Test Max", "ticket_uid" : "049161bb-badd-4fa8-9d90-87c9a82b0667"}
    self.assertEqual(requests.patch(URL + "/return", headers=headers).status_code == 200, True)
    
    headers = {"X-User-Name" : "Test Max", "ticket_uid" : "049161bb-badd-4fa8-9d90-87c9a82b0665"}
    self.assertEqual(requests.patch(URL + "/return", headers=headers).status_code == 200, True)
      
if len(sys.argv) > 1:
  HOST = str(sys.argv[1])
  URL = "http://" + HOST + ":8000/api/v1"
  del sys.argv[1:]
unittest.main()
    
