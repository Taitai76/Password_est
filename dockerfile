# ---- build stage ----
FROM ubuntu:22.04 AS build
RUN apt-get update && apt-get install -y \
  build-essential cmake git libjsoncpp-dev uuid-dev openssl libssl-dev zlib1g-dev

# Build & install Drogon
RUN git clone --depth 1 https://github.com/drogonframework/drogon.git /drogon \
 && cd /drogon && mkdir build && cd build \
 && cmake .. -DCMAKE_BUILD_TYPE=Release \
 && make -j && make install

# Build your app
WORKDIR /app
COPY . .
RUN mkdir build && cd build \
 && cmake -DCMAKE_BUILD_TYPE=Release .. \
 && cmake --build .

# ---- runtime stage ----
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y libjsoncpp25 libssl3 zlib1g && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY --from=build /usr/local/lib /usr/local/lib
COPY --from=build /usr/local/include /usr/local/include
COPY --from=build /app/build/password_est /app/password_est
COPY --from=build /app/config.json /app/config.json
COPY --from=build /app/public /app/public
ENV PORT=8080
EXPOSE 8080
CMD ["/app/password_est"]
