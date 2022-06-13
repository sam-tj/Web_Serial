# Web Serial

Tired of uploading website data to ESP32 everytime there is a small change on webpage or justs a CSS fix? Maybe forgot to add function to JS file??

This method allows hosting website on a local server or a cloud server like Github. The website connects to the local device via local IP address and allows data transmission through it.

---

### About

This is an example project which allows to send data from server hosted websites (like Github) to Local network devices.
The code can be modified according to application. It can also be implemented to get data from devices and publish it to server hosted websites.

[Demo Video](https://raw.githubusercontent.com/sam-tj/Web_Serial/master/demo.mp4 "Demo Video")

### Why is this better?

- Update website files which might include HTML, CSS, JS, images or other types
- No storage issue as website is hosted on server
- More free memory of ESP32
- Better performance of website with multiple pages and animations
- Improves speed of ESP32 due to reduced load on memory
- ...

### Where can this be used?

It can be used in almost all project which need a web portal or a web server. Some of my projects can also be ported with this method.

- https://sam-tj.github.io/MoverProject/data/
- https://www.instructables.com/Pocket-ECG-Monitor/

---

### List of components used:

- [ESP 32](https://www.az-delivery.de/en/products/esp32-developmentboard "ESP 32")

---

### Libraries Required

You need to download the following:

- ESP32 HTTPS Server by Frank Hessel : https://github.com/fhessel/esp32_https_server ( Can be downloaded Library Manager)

---

### Tutorial

1. Get the code
2. Get the libraries
3. Attach everything
4. Upload the code
5. Open page with local ip once to accept the certificate. This is important as it is user generated certificate and is not certified from authorities
6. Use same IP at https://sam-tj.github.io/Web_Serial/ and send text to print on serial monitor.
   🔥🔥🔥🔥🔥

---

### References

---

- https://github.com/fhessel/esp32_https_server
- http://sandboxelectronics.com/?p=165
- https://github.com/fhessel/esp32_https_server/issues/55#issuecomment-549375480
- https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
- https://arduino.stackexchange.com/questions/56365/arduino-convert-stdstring-to-string
