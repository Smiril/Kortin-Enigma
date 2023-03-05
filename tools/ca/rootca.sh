#!/usr/bin/env bash
# Prepare the root directory
echo "Preparing root directory..."
mkdir ~/root
mkdir ~/root/ca

cd ~/root/ca
mkdir certs crl newcerts private
chmod 700 private
touch index.txt
echo 1000 > serial

# ROOT

# Prepare the root configuration file
echo "Preparing root configuration file..."
wget -O openssl.cnf 'https://jamielinux.com/docs/openssl-certificate-authority/_downloads/root-config.txt'

# Create the root key
echo "Creating root key..."
cd ~/root/ca
openssl genrsa -aes256 -out private/valid-root-cakey.pem 4096
chmod 400 private/valid-root-cakey.pem

# Create the root certificate
echo "Creating root certificate..."
cd ~/root/ca
openssl req -config openssl.cnf \
    -key private/valid-root-cakey.pem \
    -new -x509 -days 7300 -sha256 -extensions v3_ca \
    -out certs/valid-root-ca.pem
cp private/valid-root-cakey.pem /usr/local/share/enigma/valid-root-cakey.pem
cp certs/valid-root-ca.pem /usr/local/share/enigma/valid-root-ca.pem
chmod 444 /usr/local/share/enigma/valid-root-cakey.pem
chmod 444 /usr/local/share/enigma/valid-root-ca.pem
chmod 444 certs/valid-root-ca.pem

# Verify the root certificate
echo "Verifying root certificate..."
openssl x509 -noout -text -in certs/valid-root-ca.pem
pause
