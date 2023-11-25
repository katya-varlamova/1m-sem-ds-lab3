import argparse
import sys
import os
import pika
import signal
import requests
from time import sleep

example_usage = '''====EXAMPLE USAGE=====

Connect to remote rabbitmq host
--user=guest --password=guest --host=192.168.1.200

Specify exchange and queue name
--exchange=myexchange --queue=myqueue
'''

ap = argparse.ArgumentParser(description="RabbitMQ producer",
                             epilog=example_usage,
                             formatter_class=argparse.RawDescriptionHelpFormatter)
ap.add_argument('--user',default="username",help="username e.g. 'guest'")
ap.add_argument('--password',default="password",help="password e.g. 'pass'")
ap.add_argument('--host',default="rabbitmq",help="rabbitMQ host, defaults to localhost")
ap.add_argument('--port',type=int,default=5672,help="rabbitMQ port, defaults to 5672")
ap.add_argument('--exchange',default="",help="name of exchange to use, empty means default")
ap.add_argument('--queue',default="cancel-bonus",help="name of default queue, defaults to 'testqueue'")
ap.add_argument('--routing-key',default="testqueue",help="routing key, defaults to 'testqueue'")
ap.add_argument('--service-host',default="bonus-service",help="api gateway host")
ap.add_argument('--service-port',default="8050",help="api gateway port")
args = ap.parse_args()

def queue_callback(channel, method, properties, body):
    print(method)
    print(properties)
    if len(method.exchange):
        print("from exchange '{}': {}".format(method.exchange,body.decode('UTF-8')))
    else:
        print("from queue {}: {}".format(method.routing_key,body.decode('UTF-8')))

    try:
        resp = requests.delete(f"http://{args.api_gateway_host}:{args.api_gateway_port}/api/v1/rental/{body.decode('UTF-8')}", headers={"X-User-Name":method.routing_key})
        resp.raise_for_status()
    except requests.exceptions.RequestException:
        channel.basic_publish(exchange='', routing_key=method.routing_key, body=body)
    finally:
        channel.basic_ack(delivery_tag=method.delivery_tag) 
    # channel.basic_ack(method.delivery_tag)
    sleep(1)
    # channel.basic_nack(method.delivery_tag)
    # channel.basic_reject(method.delivery_tag)

def signal_handler(signal,frame):
    print("\nCTRL-C handler, cleaning up rabbitmq connection and quitting")
    connection.close()
    sys.exit(0)



# connect to RabbitMQ
credentials = pika.PlainCredentials(args.user, args.password )
connection = pika.BlockingConnection(pika.ConnectionParameters(args.host, args.port, '/', credentials ))
channel = connection.channel()

# while True:
# 
#     method_frame, header_frame, body = channel.basic_get(args.queue)
#     if method_frame:
#         queue_callback(channel, method_frame, header_frame, body)
#     
#     sleep(1)


channel.basic_consume(queue=args.queue, on_message_callback=queue_callback, auto_ack=False)

# capture CTRL-C
signal.signal(signal.SIGINT, signal_handler)
#
channel.start_consuming()
