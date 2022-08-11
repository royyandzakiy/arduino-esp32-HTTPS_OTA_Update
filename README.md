# ESP32 HTTPS OTA Update

Implementing the HTTPS OTA Update on ESP32

<not yet working, still a work in progress>

using anonfiles.com as free file hosting

## PREP:
- decide what file host you will use
- acquire the server certificate, follow [this tutorial](https://medium.com/@sanghviyash6/how-to-get-ca-certificate-of-any-server-using-google-chrome-e8db3e4d3fcf#6c8f)
- copy the certificate and change the *server_certificate variable in <main>.ino

## STEPS:
- upload file to anonfiles.com
- acquire file url
- change vaiable *url in <main>.ino
