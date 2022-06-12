function sendData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      console.log("success");
    }
  };
  console.log(
    "https://" +
      document.getElementById("url_val").value +
      "/?to_serial=" +
      document.getElementById("text_val").value
  );
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
