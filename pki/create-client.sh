#/bin/sh

# Create dirs we need
mkdir -p client/$1

# Generate client key
openssl ecparam -genkey -name prime256v1 -out client/$1/client.key

# Generate client CSR
openssl req -new -SHA384 -key client/$1/client.key -nodes -out client/$1/client.csr

# Sign the CSR
openssl x509 -req -SHA384 -days 365 -in client/$1/client.csr -CA ca/ca.crt -CAkey ca/ca.key -CAcreateserial -out client/$1/client.crt