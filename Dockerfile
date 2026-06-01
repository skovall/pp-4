FROM alpine:latest AS build
RUN apk add --no-cache g++ cmake make git
WORKDIR /build
COPY src/ ./src/
COPY CMakeLists.txt .
RUN cmake . && make app

FROM alpine:latest
RUN apk add --no-cache libstdc++ curl
WORKDIR /app
COPY --from=build /build/app .
EXPOSE 8080
HEALTHCHECK --interval=10s --timeout=5s --retries=3 CMD curl -f http://localhost:8080/health || exit 1
ENTRYPOINT ["./app"]
