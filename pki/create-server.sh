#/bin/sh

# Generate client key
openssl ecparam -genkey -name prime256v1 -out server.key

# Generate client CSR
openssl req -new -SHA384 -key server.key -nodes -out server.csr

# Sign the CSR
openssl x509 -req -SHA384 -days 365 -in server.csr -CA ca/ca.crt -CAkey ca/ca.key -CAcreateserial -out server.crt