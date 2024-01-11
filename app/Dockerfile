FROM gcc:latest

# Install PostgreSQL development libraries
RUN apt-get update && \
    apt-get install -y \
    libpq-dev \
    iputils-ping \
    telnet \
    && rm -rf /var/lib/apt/lists/*

# Add a command to check if the libpq-fe.h file exists
RUN find / -name 'libpq-fe.h'

ENV CONTAINER_NAME="APP"

WORKDIR /usr/src/app
COPY src/ .
# RUN gcc -o app main.c -lpq -I/usr/include/postgresql
RUN make


# TEST #
# # Stage 1: Build Stage
# FROM gcc:latest as build-stage

# # Install PostgreSQL development libraries
# RUN apt-get update && \
#     apt-get install -y \
#     libpq-dev \
#     iputils-ping \
#     telnet \
#     && rm -rf /var/lib/apt/lists/*

# # Check if the libpq-fe.h file exists
# RUN find / -name 'libpq-fe.h'

# # Set working directory and copy source code
# WORKDIR /usr/src/app
# COPY src/ .
# RUN make

# # Stage 2: Runtime Stage
# FROM debian:bullseye

# # Install only the necessary libraries for runtime
# RUN apt-get update && apt-get install -y \
#     libpq5
# # Copy the compiled application from the build stage
# COPY --from=build-stage /usr/src/app/app /usr/local/bin/app

# # Set the command to run the application
# CMD ["app"]
