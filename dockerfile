# ---- build stage ----
FROM ubuntu:22.04 AS build
RUN apt-get update && apt-get install -y \
  build-essential cmake git pkg-config ca-certificates \
  libjsoncpp-dev uuid-dev openssl libssl-dev zlib1g-dev

# Drogon + Trantor (with submodules)
RUN git clone --branch v1.9.11 --depth 1 --recursive https://github.com/drogonframework/drogon.git /drogon \
 && cmake -S /drogon -B /drogon/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF -DBUILD_ORM=OFF -DBUILD_CTL=OFF \
 && cmake --build /drogon/build -j \
 && cmake --install /drogon/build

WORKDIR /app
COPY . .
RUN mkdir build && cd build \
 && cmake -DCMAKE_BUILD_TYPE=Release .. \
 && cmake --build .

# ---- runtime stage ----
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
  ca-certificates \
  # runtime libs needed by your binary + Drogon/Trantor:
  libc-ares2 \
  libssl3 \
  zlib1g \
  libuuid1 \
  libjsoncpp24 \
  && rm -rf /var/lib/apt/lists/*
WORKDIR /app
# copy Drogon libs (installed to /usr/local/lib during build)
COPY --from=build /usr/local/lib /usr/local/lib
# your app + assets
COPY --from=build /app/build/password_est /app/password_est
COPY --from=build /app/config.json /app/config.json
COPY --from=build /app/public /app/public
ENV LD_LIBRARY_PATH=/usr/local/lib
ENV PORT=8080
EXPOSE 8080
CMD ["/app/password_est"]
