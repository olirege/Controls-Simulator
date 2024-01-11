# Dockerfile for controller
FROM gcc:latest

# Install PostgreSQL development libraries
RUN apt-get update && \
    apt-get install -y \
    libpq-dev \
    iputils-ping \
    telnet \
    libcurl4-gnutls-dev \
    && rm -rf /var/lib/apt/lists/*

# Add a command to check if the libpq-fe.h file exists
RUN find / -name 'libpq-fe.h'
RUN find / -name 'curl.h'

ENV CONTAINER_NAME="CTRL"
ENV CONTAINER_NETWORK="something_new_default"
ENV DB_HOST="db"
ENV DB_USER="user"
ENV DB_PASS="password"
ENV DB_NAME="sysdb"
ENV DB_PORT="5432"
WORKDIR /usr/src/controller

# Copy the entire controller directory
COPY . .

# Compile the controller application
RUN make
# Command to run the controller application
CMD ["./controller"]
# Stage 1: Build environment

# FROM gcc:latest as builder

# # Install PostgreSQL development libraries
# RUN apt-get update && apt-get install -y \
#     libpq-dev \
#     libcurl4-gnutls-dev \
#     && rm -rf /var/lib/apt/lists/*

# # Set working directory
# WORKDIR /usr/src/controller

# # Copy source code
# COPY . .

# # Compile the controller application
# RUN make

# # Stage 2: Lightweight runtime environment
# FROM debian:buster-slim

# # Install runtime dependencies
# RUN apt-get update && apt-get install -y \
#     libpq5 \
#     libcurl4-gnutls

# # Set environment variable
# ENV CONTAINER_NAME="CTRL"

# # Set working directory
# WORKDIR /usr/src/controller

# # Copy the binary from the builder stage
# COPY --from=builder /usr/src/controller/controller /usr/src/controller/controller

# Command to run the controller application
CMD ["./controller"]
