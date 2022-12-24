FROM python

WORKDIR /app

RUN apt-get install -y --no-install-recommends gcc git libssl-dev g++ make python3 && \
cd /tmp && \
git clone https://github.com/edenhill/librdkafka.git && \
cd librdkafka && \
./configure --prefix=/usr && \
make && \
make install && \
pip3 install --upgrade pip && \
pip3 install confluent-kafka

FROM bitnami/kafka

CMD ["/bin/sh"]