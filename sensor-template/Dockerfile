# Dockerfile for sensor-template
FROM gcc:latest

# Install PostgreSQL development libraries
RUN apt-get update && \
    apt-get install -y \
    iputils-ping \
    telnet \
    && rm -rf /var/lib/apt/lists/*

ENV SENSOR_NAME=sensor_dflt
ENV CONTAINER_NETWORK="something_new_default"
WORKDIR /usr/src/sensor

# Copy sensor source code
COPY . .

# Compile the sensor application
RUN gcc -o sensor sensor.c sockets/sockets.c

# Command to run the sensor application
CMD ["./sensor"]
