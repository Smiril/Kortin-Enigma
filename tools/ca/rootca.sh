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
chmod 400 private/ca.key.pem

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

# INTERMEDIATE

# Prepare the intermediate directory
echo "Preparing intermediate directory..."
mkdir ~/root/ca/intermediate

cd ~/root/ca/intermediate
mkdir certs crl csr newcerts private
chmod 700 private
touch index.txt
echo 1000 > serial
echo 1000 > ~/root/ca/intermediate/crlnumber

# Prepare the intermediate configuration file
echo "Preparing intermediate configuration file..."
wget -O openssl.cnf 'https://jamielinux.com/docs/openssl-certificate-authority/_downloads/intermediate-config.txt'

# Create the intermediate key
echo "Creating intermediate key..."
cd ~/root/ca
openssl genrsa -aes256 \
    -out intermediate/private/intermediate.key.pem 4096
chmod 400 intermediate/private/intermediate.key.pem

# Create the intermediate certificate
# NOTE: can have same values as root certificate, EXCEPT for common name
echo "Creating intermediate certificate..."
cd ~/root/ca
openssl req -config intermediate/openssl.cnf -new -sha256 \
    -key intermediate/private/intermediate.key.pem \
    -out intermediate/csr/intermediate.csr.pem
openssl ca -config openssl.cnf -extensions v3_intermediate_ca \
    -days 3650 -notext -md sha256 \
    -in intermediate/csr/intermediate.csr.pem \
    -out intermediate/certs/intermediate.cert.pem
chmod 444 intermediate/certs/intermediate.cert.pem

# Verify the intermediate certificate
echo "Verifying intermediate certificate..."
openssl x509 -noout -text \
    -in intermediate/certs/intermediate.cert.pem
openssl verify -CAfile certs/valid-root-ca.pem \
    intermediate/certs/intermediate.cert.pem
pause

# Create the certificate chain file
echo "Creating certificate chain file..."
cat intermediate/certs/intermediate.cert.pem \
    certs/valid-root-ca.pem > intermediate/certs/ca-chain.cert.pem
chmod 444 intermediate/certs/ca-chain.cert.pem
