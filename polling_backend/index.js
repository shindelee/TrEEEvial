const express = require("express");
const cors = require('cors');
const path = require('path');

const PORT = process.env.PORT || 3001;
const app = express();

app.use(cors({
    origin: '*'
}));

//app.use(express.static(path.resolve(__dirname, './updated2.0/build')));

app.use(cors({
    methods: ['GET','POST','DELETE','UPDATE','PUT','PATCH']
}));

app.get("/initial", (req, res) => {
    setTimeout(()=>{
        res.json(
            {
            "tableData33":[
            [0,2,0,0,1,0,0,0,0],
            [0,0,1,1,1,1,0,1,0],
            [0,0,0,0,0,1,0,1,0],
            [1,0,1,0,1,1,0,1,0],
            [1,0,1,0,1,1,0,1,0],
            [1,0,1,0,0,0,0,1,0],
            [1,0,1,1,1,1,0,1,0],
            [1,0,0,0,0,0,0,0,0],
            [1,1,1,1,1,0,0,0,0]
            ]
        })
    }, 1000);
});

app.get("/tableData33", (req, res) => {
    setTimeout(()=>{
        res.json(
            {
            "tableData33":[
            [2,0,0,0,1,0,0,0,0],
            [0,1,1,1,1,1,0,1,0],
            [0,0,0,0,0,1,0,1,0],
            [1,0,1,0,1,1,0,1,0],
            [1,0,1,0,1,1,0,1,0],
            [1,0,1,0,0,0,0,1,0],
            [1,0,1,1,1,1,0,1,0],
            [1,0,0,0,0,0,0,0,0],
            [1,1,1,1,1,0,0,0,0]
            ]
        })
    }, 1000);
});

app.get("/shortestPath", (req, res) => {
    setTimeout(()=>{
        res.json(
            {
            "shortestPath":[
            [2,0,0,0,1,0,0,0,0],
            [4,1,1,1,1,1,0,1,0],
            [4,4,0,0,0,1,0,1,0],
            [1,4,1,0,1,1,0,1,0],
            [1,4,1,0,1,1,0,1,0],
            [1,4,1,0,0,0,0,1,0],
            [1,4,1,1,1,1,0,1,0],
            [1,4,4,4,4,4,4,4,4],
            [1,1,1,1,1,0,0,0,4]
            ]
        })
    }, 1000);
});

app.get("/pollServer", (req, res) => {
    var d = new Date();
    const json_res = {
    "time" : [
        [Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4)],
        [Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4)],
        [Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4)],
        [Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4)],
        [Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4)],
        [Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4)],
        [Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4)],
        [Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4)],
        [Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4),Math.floor(Math.random() * 4)]
        ]
    };
    res.send(json_res);
    }); 

/*
app.get('*', (req, res) => {
    res.sendFile(path.resolve(__dirname, './updated2.0/build', 'index.html'));
});
*/

app.listen(PORT, () => {
 console.log(`Server listening on ${PORT}`);
});
