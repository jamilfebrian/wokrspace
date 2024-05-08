// let SHEET_ID = "1DlS3m_wkLoZLWxiCB2YF3J6EnajFUtRyS9XN61GSNr0";
// let SHEET_TITTLE = "monitoring_data";
// let LAMP_TABLE_RANGE = "I3:N3";

// let FULL_URL = ("https://docs.google.com/spreadsheets/d/"+SHEET_ID+"/gviz/tq?sheet="+SHEET_TITTLE+"&range="+LAMP_TABLE_RANGE);

// fetch(FULL_URL)
// .then(res => res.text())
// .then(rep => {
//     let data =JSON.parse(rep.substr(47).slice(0,-2));
//     console.log(data.table.rows[0].c[0].v)

    
// })
var API = "https://script.google.com/macros/s/AKfycbx4CkU0f4JvSSu7LY89deNkQ1CNQQghtPSFr5PIRZNttBb9j8WAPXEFIF3W0x1RrFCX/exec";

function addData(){
    let obj = {
        todo : "halo"
    }
    fetch(API,{
        method:"POST",
        body:JSON.stringify(obj)
    })
    .then(res => res.text())
    .then(data => {
        alert(data)
    });
}

// addData();

function doPost(req){
    let data = JSON.parse(req.postData.contents)
    StyleSheet.appendRow(["row()", data.todo])
    return COntentService.createtextOutput("Data received")
}