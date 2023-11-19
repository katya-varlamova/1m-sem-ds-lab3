import unittest
import json
import requests
import sys
HOST="127.0.0.1"
URL = "http://" + HOST + ":8060/api/v1/flights"


headers = {'Content-type': 'application/json', 'Accept': 'text/'}
class TestMain(unittest.TestCase):
  # positives
    
  def test_get_all(self):
    params = {"page" : 1, "size" : 1}
    fn = requests.get(URL, params=params).json()["items"][0]["flightNumber"]
    self.assertEqual(fn == "AFL031", True)
    
  def test_get_by_fn(self):
    fn = requests.get(URL + "/AFL031").json()["flightNumber"]
    self.assertEqual(fn == "AFL031", True)

if len(sys.argv) > 1:
  HOST = str(sys.argv[1])
  URL = "http://" + HOST + ":8000/api/v1/Flights"
  del sys.argv[1:]
unittest.main()
    
