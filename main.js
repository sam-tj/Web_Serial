function sendData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ADCValue").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "https://192.168.0.3/PDRS?param1=%22testing%22&param2=%22testing1%22", true);
  xhttp.send();
}
