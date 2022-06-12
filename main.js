function sendData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      console.log("success");
    }
  };
  xhttp.open(
    "GET",
    "https://" +
      document.getElementById("url_val").innerHTML +
      "/?to_serial=" +
      document.getElementById("text_val").innerHTML,
    true
  );
  xhttp.send();
}
