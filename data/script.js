rowWidth = 4;
bulbs = rowWidth**2;
let checkStates = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];


String.prototype.replaceAt = function(index, replacement) {
    return this.substr(0, index) + replacement + this.substr(index + replacement.length);
}

window.onload = function(){
    var body = document.getElementsByTagName('body')[0];
    table = document.createElement("table");
    table.setAttribute('class', 'table');
    table.setAttribute("id", "hrtable");
    body.appendChild(table);
    var nrow = 0;
    var x = 0;

    for(i = 1; i <= bulbs; i++) {
        if(x>rowWidth-1){
            nrow++;
            x=0;
        }
        x++;
        var checkbox = document.createElement('input'); 
        checkbox.type = "checkbox";
        checkbox.value = i; 
        checkbox.id = "myCheckbox";
        checkbox.addEventListener('change', (event) => {
            var checkID = event.target.value - 1;
            if (event.target.checked) {
                checkStates[checkID] = 0;
                event.target.parentElement.className = "cell active";
                
            } else {
                checkStates[checkID] = 1;
                event.target.parentElement.className = "cell";
            }
            console.log(checkStates);
        })
        addCheckboxToTable(checkbox, table, nrow)
    }
}

function togColor(button_id) {
    console.log("clicked! " + button_id);
    var button = document.getElementById(button_id);  
    if (button.getAttribute("class") !== "tbutton active") {
        button.className = "tbutton active";       

    }
    else if (button.getAttribute("class") === " tbutton active") {
        button.className = "tbutton"; 
    }
}

function setBulbs(){
    var statesArray = checkStates.join("");
    console.log("setBulbs: "+ statesArray);
    var xhr = new XMLHttpRequest();
    var url = "/test?"+"states=" + statesArray;
    xhr.open("POST", url, true);
    xhr.send();
}

function addCheckboxToTable(checkbox, table, nrow) {
    var cell = document.createElement("td");
    var row = table.rows[nrow];
    if(!row) {
      row = document.createElement("tr");
      table.appendChild(row);
    }
    cell.textContent = checkbox.value;
    cell.className = "cell";
    cell.appendChild(checkbox);
    row.appendChild(cell);
}