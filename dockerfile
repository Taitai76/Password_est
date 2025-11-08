# ---- build stage (Drogon preinstalled) ----
FROM drogonframework/drogon:latest AS build
WORKDIR /app
COPY . .
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build -j

# ---- runtime stage (also Drogon image) ----
FROM drogonframework/drogon:latest AS runtime
WORKDIR /app
COPY --from=build /app/build/password_est /app/password_est
COPY --from=build /app/config.json /app/config.json
COPY --from=build /app/public /app/public
ENV PORT=8080
EXPOSE 8080
CMD ["/app/password_est"]
