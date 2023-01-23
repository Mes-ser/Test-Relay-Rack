function setState(button_id)  {
    var text = document.getElementById(button_id).firstChild;
    var xhr = new XMLHttpRequest();
    if (text.data === "On") {
        console.log("state:1");
        xhr.open("POST", "/setState?state=0" , true);
        xhr.send();
    }else{
        console.log("state:0");
        xhr.open("POST", "/setState?state=1" , true);
        xhr.send();
    }
 }

function sendData(){
    var xhr = new XMLHttpRequest();
    var inputData = document.getElementById('test')
    var url = "/test?"+"text=" + inputData.value;
    xhr.open("POST", url, true);
    xhr.send();
    console.log(inputData.value)
    inputData.value = "";
}

var source = new EventSource('/events');

source.addEventListener('open', function(e) {
    console.log("Events Connected");
}, false);

source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
    }
}, false);

source.addEventListener('message', function(e) {
    console.log("message", e.data);
}, false);

source.addEventListener('state', function(e) {
    var button = document.getElementById("myButton").firstChild;
    if(e.data === "0"){
        button.data = "On";
    }else{
        button.data = "Off";
    }
    console.log("State", e.data);
}, false);