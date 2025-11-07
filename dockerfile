# ---- build stage (has Drogon preinstalled) ----
FROM drogonframework/drogon:latest AS build
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
COPY --from=build /app/build/password_est /app/password_est
COPY --from=build /app/config.json /app/config.json
COPY --from=build /app/public /app/public
ENV LD_LIBRARY_PATH=/usr/local/lib
ENV PORT=8080
EXPOSE 8080
CMD ["/app/password_est"]
