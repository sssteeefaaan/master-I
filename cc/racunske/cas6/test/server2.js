const http = require("http");

http.get("http://node-server-container:8090", async (res)=> {
    if (res.statusCode == 200) {
        console.log("Success!")
        res.on('data', (chunk)=>{
            console.log(chunk.toString())
        })
    }else{
        console.log("Failed!", res.statusMessage)
    }
});