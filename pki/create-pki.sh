#/bin/sh

# Create dirs we need
mkdir -p ca/

# Generate CA key
openssl ecparam -genkey -name prime256v1 -out ca/ca.key

# Generate a self signed CA
openssl req -x509 -new -SHA384 -nodes -key ca/ca.key -days 36500 -out ca/ca.crt