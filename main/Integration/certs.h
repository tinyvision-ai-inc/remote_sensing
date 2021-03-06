#ifndef certs_h
#define certs_h

#include <pgmspace.h>


// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAICUz5AL0VLoyzE7SvMUjzF/JLqEMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTA4MDIxOTE1
MjdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDIhwJITR7RBXAiqdoT
vkI/dE4ozCgLSH7viCOrH5bD7D6eTAmpumAb8Nw4echcOaHpJVQXcH1ChPy557BL
upYSzCmvMJhA2IOtxcOGK198oZftzLt/FrLEG8nvibqKMPzGP/wF18ZvotxjAGQW
8cmNge1pJJ1FW4A8oskZX6yJgbC7oIEVYMaudy5Z0OirPUbsi38TroS49Z7pc96N
dRgYAGj1xbUTxGE/a8Uj8/DYB/svHJ3WEKvCxF4a7RrCdf9xg5bckq2aAXxfMogr
mA7m07zyQF9jUSuIdOtqzOZ9P+e4pMyFj0q6sZEmecfrXetRqu0WBWKHL6s0LGfJ
HyrvAgMBAAGjYDBeMB8GA1UdIwQYMBaAFDAV/0RBJQ5UxrLRUXFfq5mtY35BMB0G
A1UdDgQWBBQjdCUSXM5YNWiIsaGVrUt0FESQijAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEASjjONBiznRyiy8h7n3Jj8Vsr
wwyTaAhUlkuhbsv82EtbEyC/N8+HwrpvPCZwiNyZ9AmUKkwx7bd623UAcf5iouzM
qn49LaqxClWpmmp0n5L950AtPUzg2hIZYbuZvqsc9VhNxTwfnAfmOz402aKFLmpq
b2TJg8g0XSssiFhoqH045kG38/3EBip1ZC/yBobTPv2Xt5nXGusmyyMOYxIyG6HN
kGtv8Veyq9gOoQ18LSTuuuMAI0JIroqW0SstNI2ym/jCrhWSqzKV7CQYUfGFP5Qr
/N+Qu7UucrD8avHk6Av+bv1ea6V3G72WNsvfZpVVmb1Q0EhfwiGPJy6A2L3jqQ==
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAyIcCSE0e0QVwIqnaE75CP3ROKMwoC0h+74gjqx+Ww+w+nkwJ
qbpgG/DcOHnIXDmh6SVUF3B9QoT8ueewS7qWEswprzCYQNiDrcXDhitffKGX7cy7
fxayxBvJ74m6ijD8xj/8BdfGb6LcYwBkFvHJjYHtaSSdRVuAPKLJGV+siYGwu6CB
FWDGrncuWdDoqz1G7It/E66EuPWe6XPejXUYGABo9cW1E8RhP2vFI/Pw2Af7Lxyd
1hCrwsReGu0awnX/cYOW3JKtmgF8XzKIK5gO5tO88kBfY1EriHTraszmfT/nuKTM
hY9KurGRJnnH613rUartFgVihy+rNCxnyR8q7wIDAQABAoIBAQCrUBQWGrSqE8BC
DOO/HDrSpRUk642GXi/7iU4XoswVo523dPSIaS40kKhDI1+V0rLNUNOvlq2ch6q1
VAecGtKdSoiOH7DjLVYJvjuVQza3bgIy6UaBg63Dto3lM8AG0WUSJZW0lCv2Maeg
eLQJmhT77LUbr2kg2NQMeXHeWIs5iOw9UW4CEfuJ2HqzVDpNAOI525OyIKBam2d4
I4qa7ERwNlpV2lrIf93hjmZrRd0AGS9REeOH1OZI7zXEAhFVp90I94ypLR9bqbaI
+VqeRvYkeRqX2yG1/OLYnPfQYxIQB4jf3ZHqjV7Ko7zE90noHkUx2pmiX5fT55E3
v15dHhdhAoGBAOt5C61mHmD/MrBUYGbUo7Sqbm2ow6kRjoqE4pHRjzE/hh3L4JM9
EP8rS0cpi4QFvUmr4yyT25wlY4VaDxA3s4bVHi66npRe1ZoiqzWtHUAczTywBGt7
TxIitv+cR1eA0EztwR0AiUfeqXjI97+35VbpUXEYoxPTJ5VZnvfEx+//AoGBANoC
GW6Y6hklCXFX0vEh2bOkWdPAhN/o4SWPN9toH90Mq/lYcoPHUQH9PucbSUXHTbo/
yBikxVzzrJRa3l+WrJpliOBCD7aFKRWmLDdG1PQvKgS/JGvE74ujqULqxSOmdt7a
t32SKuiErfIADvcnT/ixH6a8kM/IRqqO3TSz18URAoGALNRducRwUi0jvFhEBi/D
fvjXjz8LCRwTlCGadQJ9jXW3leKjvcRpgPsAGOs66YaiM+1qkakQrVic6t1rcr34
Ctp2JZVX7Y1kgzTIfa6YnAUfuccVAFV4h05eoqlhF7KpdYvqMRhSCoHEp/8y06gy
HGrNG4gCPBr+NTvbvPYUzocCgYEAgAsTnN27QSF7tlJ61p2nG9e/2psMb+Z7tm+z
wu4ZKN+0cjL8r2F1ttFjDKH+Gzm/8VBc2fyM3tdP2aK1UTdSESzpg63JRM5ZHxoG
hkDX+sNoznjlj1aPy1hIpcD2IMY4sxOLCfm7se2brLNUFt3EHrEJggjmlONwfeSD
D91BMcECgYEAmS64n77D/0D7a0jl4dPS4QND3hJxTih+wiJJaixmSA6lt0noZ+bV
wrf1T9blkccNT8ULIupz7KOl9k3Ccus5ITwKH/7lL9tm6G42lbI07CTOXD1nBtyK
R/KP3pvI9yHoXCkNGce8jumamU65x+8reJXrxKu0+jR8COJ1r4227ok=
-----END RSA PRIVATE KEY-----
)KEY";

#endif
