FROM python

COPY ./ /app
WORKDIR /app
RUN pip install -r requirements.txt
ENV PYTHONUNBUFFERED=1
ENTRYPOINT ["python", "/app/consumer.py", "--host=rabbitmq", "--queue=cancel-bonus", "--service_host=bonus-service", "--service_port=8050"]
